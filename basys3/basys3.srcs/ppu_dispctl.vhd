library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;
use work.ppu_consts.all;

entity ppu_dispctl is port(
	CLK : in std_logic; -- system clock
	RESET : in std_logic;

	X : out std_logic_vector(PPU_POS_H_WIDTH-1 downto 0); -- tiny screen pixel x
	Y : out std_logic_vector(PPU_POS_V_WIDTH-1 downto 0); -- tiny screen pixel y
	RI,GI,BI : in std_logic_vector(PPU_COLOR_OUTPUT_DEPTH-1 downto 0); -- color in
	PREADY : in std_logic; -- current pixel ready (pixel color is stable)

	RO,GO,BO : out std_logic_vector(PPU_COLOR_OUTPUT_DEPTH-1 downto 0); -- VGA color out
	NVSYNC, NHSYNC : out std_logic; -- VGA sync out
	THBLANK, TVBLANK : out std_logic); -- tiny sync signals
end ppu_dispctl;

architecture Behavioral of ppu_dispctl is
	component ppu_dispctl_slbuf port( -- scanline buffer
		clka : in std_logic;
		wea : in std_logic_vector(0 to 0);
		addra : in std_logic_vector(PPU_DISPCTL_SLBUF_ADDR_WIDTH-1 downto 0);
		dina : in std_logic_vector(PPU_RGB_COLOR_OUTPUT_DEPTH-1 downto 0);
		clkb : in std_logic;
		rstb : in std_logic;
		addrb : in std_logic_vector(PPU_DISPCTL_SLBUF_ADDR_WIDTH-1 downto 0);
		doutb : out std_logic_vector(PPU_RGB_COLOR_OUTPUT_DEPTH-1 downto 0);
		rsta_busy : out std_logic;
		rstb_busy : out std_logic);
	end component;
	signal CLK25 : unsigned(1 downto 0) := (others => '0'); -- clock divider (100_000_000/4)
	signal PCOUNT, HCOUNT, VCOUNT : unsigned(PPU_VGA_SIGNAL_PIXEL_WIDTH-1 downto 0) := (others => '0');
	signal ADDR_I, ADDR_O : std_logic_vector(PPU_DISPCTL_SLBUF_ADDR_WIDTH-1 downto 0);
	signal DATA_I, DATA_O : std_logic_vector(PPU_RGB_COLOR_OUTPUT_DEPTH-1 downto 0);
	signal T_POS_X : unsigned(PPU_SCREEN_T_POS_X_WIDTH-1 downto 0) := (others => '0');
	signal T_POS_Y : unsigned(PPU_SCREEN_T_POS_Y_WIDTH-1 downto 0) := (others => '0');
	signal N_POS_X : unsigned(PPU_SCREEN_N_POS_X_WIDTH-1 downto 0) := (others => '0');
	signal N_POS_Y : unsigned(PPU_SCREEN_N_POS_Y_WIDTH-1 downto 0) := (others => '0');
	signal ACTIVE, HACTIVE, VACTIVE : std_logic := '0';
begin
	DATA_I <= RI & GI & BI;
	ADDR_I <= std_logic_vector(resize(T_POS_X, ADDR_I'length)) when T_POS_Y(0) = '0' else std_logic_vector(resize(T_POS_X, ADDR_I'length) + PPU_SCREEN_WIDTH);

	X <= std_logic_vector(T_POS_X);
	Y <= std_logic_vector(T_POS_Y);

	RO <= DATA_O(11 downto 8);
	GO <= DATA_O(7 downto 4);
	BO <= DATA_O(3 downto 0);

	HCOUNT <= PCOUNT mod PPU_VGA_H_TOTAL;
	VCOUNT <= PCOUNT / PPU_VGA_H_TOTAL mod PPU_VGA_V_TOTAL;

	HACTIVE <= '1' when
		(HCOUNT >  (PPU_VGA_H_PORCH_BACK)) and
		(HCOUNT <= (PPU_VGA_H_PORCH_BACK + PPU_VGA_H_ACTIVE)) else '0';
	VACTIVE <= '1' when
		(VCOUNT >  (PPU_VGA_V_PORCH_BACK)) and
		(VCOUNT <= (PPU_VGA_V_PORCH_BACK + PPU_VGA_V_ACTIVE)) else '0';
	ACTIVE <= HACTIVE and VACTIVE;
	
	NVSYNC <= '1' when
		(VCOUNT >  (PPU_VGA_V_PORCH_BACK + PPU_VGA_V_ACTIVE)) and
		(VCOUNT <= (PPU_VGA_V_PORCH_BACK + PPU_VGA_V_ACTIVE + PPU_VGA_V_SYNC)) else '0';
	NHSYNC <= '1' when VACTIVE = '1' and
		(HCOUNT >  (PPU_VGA_H_PORCH_BACK + PPU_VGA_H_ACTIVE)) and
		(HCOUNT <= (PPU_VGA_H_PORCH_BACK + PPU_VGA_H_ACTIVE + PPU_VGA_H_SYNC)) else '0';
	
	N_POS_X <= resize(HCOUNT - PPU_VGA_H_PORCH_BACK, N_POS_X'length);
	N_POS_Y <= resize(VCOUNT - PPU_VGA_V_PORCH_BACK, N_POS_Y'length);

	T_POS_X <= resize(N_POS_X / 2, T_POS_X'length);
	T_POS_Y <= resize(N_POS_Y / 2, T_POS_Y'length);

	scanline_buffer : component ppu_dispctl_slbuf port map(
		clka => CLK,
		wea => (others => PREADY),
		addra => ADDR_I,
		dina => DATA_I,
		clkb => CLK,
		rstb => RESET,
		addrb => ADDR_O,
		doutb => DATA_O,
		rsta_busy => open,
		rstb_busy => open);

	process(CLK, RESET)
	begin
		if RESET = '1' then
			CLK25 <= (others => '0');
		elsif rising_edge(CLK) then
			CLK25 <= CLK25 + 1;
		end if;
	end process;

	process(CLK25(1), RESET)
		variable V_PCOUNT : unsigned(PPU_VGA_SIGNAL_PIXEL_WIDTH-1 downto 0) := (others => '0');
	begin
		PCOUNT <= V_PCOUNT;
		if RESET = '1' then
			V_PCOUNT := (others => '0');
		elsif rising_edge(CLK25(1)) then
			V_PCOUNT := V_PCOUNT + 1;
			if V_PCOUNT = PPU_VGA_SIGNAL_PIXEL_IDX_MAX then
				V_PCOUNT := (others => '0');
			end if;
		end if;
	end process;
end Behavioral;
