library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;
use work.ppu_consts.all;

entity ppu_dispctl is port(
	SYSCLK : in std_logic; -- system clock
	RESET : in std_logic;

	X : out std_logic_vector(PPU_POS_H_WIDTH-1 downto 0); -- tiny screen pixel x
	Y : out std_logic_vector(PPU_POS_V_WIDTH-1 downto 0); -- tiny screen pixel y
	RI,GI,BI : in std_logic_vector(PPU_COLOR_OUTPUT_DEPTH-1 downto 0); -- color in
	BWEN : in std_logic; -- scanline buffer write enable

	RO,GO,BO : out std_logic_vector(PPU_COLOR_OUTPUT_DEPTH-1 downto 0); -- VGA color out
	NVSYNC, NHSYNC : out std_logic; -- VGA sync out
	THBLANK, TVBLANK : out std_logic := '0'; -- tiny sync signals
	ACTIVE : out std_logic); -- screen currently active (currently same for tiny/native, TODO: offset tiny for first scanline)
end ppu_dispctl;

architecture Behavioral of ppu_dispctl is
	component ppu_dispctl_pixclk is port ( 
		npxclk : out std_logic;
		tpxclk : out std_logic;
		reset : in std_logic;
		sysclk : in std_logic);
	end component;
	component ppu_dispctl_slbuf port( -- scanline buffer
		clka : in std_logic;
		wea : in std_logic_vector(0 to 0);
		addra : in std_logic_vector(PPU_DISPCTL_SLBUF_ADDR_WIDTH-1 downto 0);
		dina : in std_logic_vector(PPU_RGB_COLOR_OUTPUT_DEPTH-1 downto 0);
		clkb : in std_logic;
		addrb : in std_logic_vector(PPU_DISPCTL_SLBUF_ADDR_WIDTH-1 downto 0);
		doutb : out std_logic_vector(PPU_RGB_COLOR_OUTPUT_DEPTH-1 downto 0));
	end component;
	signal NPIXCLK, TPIXCLK : std_logic;
	signal NHCOUNT, NVCOUNT : unsigned(PPU_VGA_SIGNAL_PIXEL_WIDTH-1 downto 0) := (others => '0');
	signal ADDR_I, ADDR_O : std_logic_vector(PPU_DISPCTL_SLBUF_ADDR_WIDTH-1 downto 0);
	signal DATA_I, DATA_O : std_logic_vector(PPU_RGB_COLOR_OUTPUT_DEPTH-1 downto 0);
	signal T_POS_X : unsigned(PPU_SCREEN_T_POS_X_WIDTH-1 downto 0) := (others => '0'); -- real tiny x position
	signal T_POS_Y : unsigned(PPU_SCREEN_T_POS_Y_WIDTH-1 downto 0) := (others => '0'); -- real tiny y position
	signal U_POS_X : unsigned(PPU_SCREEN_T_POS_X_WIDTH-1 downto 0) := (others => '0'); -- upscaled tiny x position
	signal U_POS_Y : unsigned(PPU_SCREEN_T_POS_Y_WIDTH-1 downto 0) := (others => '0'); -- upscaled tiny y position
	signal N_POS_X : unsigned(PPU_SCREEN_N_POS_X_WIDTH-1 downto 0) := (others => '0'); -- native x position
	signal N_POS_Y : unsigned(PPU_SCREEN_N_POS_Y_WIDTH-1 downto 0) := (others => '0'); -- native y position

	signal NACTIVE, NHACTIVE, NVACTIVE : std_logic := '0';
	signal TACTIVE, THACTIVE, TVACTIVE : std_logic := '0';
begin
	-- scanline buffer data in
	DATA_I <= RI & GI & BI;
	ADDR_I <= std_logic_vector(resize(T_POS_X, ADDR_I'length)) when T_POS_Y(0) = '0' else std_logic_vector(resize(T_POS_X, ADDR_I'length) + PPU_SCREEN_WIDTH);

	T_POS_Y <= U_POS_Y;

	X <= std_logic_vector(T_POS_X) when NACTIVE = '1' else (others => '0');
	Y <= std_logic_vector(T_POS_Y) when NACTIVE = '1' else (others => '0');

	U_POS_X <= resize(N_POS_X(N_POS_X'length-1 downto 1), U_POS_X'length);
	U_POS_Y <= resize(N_POS_Y(N_POS_Y'length-1 downto 1), U_POS_Y'length);

	-- scanline buffer data out
	ADDR_O <= std_logic_vector(resize(U_POS_X, ADDR_I'length)) when U_POS_Y(0) = '1' else std_logic_vector(resize(U_POS_X, ADDR_I'length) + PPU_SCREEN_WIDTH);
	RO <= DATA_O(11 downto 8) when NACTIVE = '1' else (others => '0');
	GO <= DATA_O(7 downto 4) when NACTIVE = '1' else (others => '0');
	BO <= DATA_O(3 downto 0) when NACTIVE = '1' else (others => '0');

	-- tiny VCOUNT and HCOUNT
	process(TPIXCLK, NPIXCLK, RESET)
		variable TMP_T_POS_X : unsigned(PPU_SCREEN_T_POS_X_WIDTH-1 downto 0) := (others => '0');
		variable TMP_NHCOUNT, TMP_NVCOUNT : unsigned(PPU_VGA_SIGNAL_PIXEL_WIDTH-1 downto 0) := (others => '0');
		variable TMP_NHACTIVE, TMP_NVACTIVE : std_logic := '0';
		variable TMP_NHSYNC, TMP_NVSYNC : std_logic := '0';
		variable TMP_NACTIVE : std_logic := '0';
	begin
		if RESET = '1' then
			TMP_NHCOUNT := (others => '0');
			TMP_NVCOUNT := (others => '0');
			TMP_NHACTIVE := '0';
			TMP_NVACTIVE := '0';
			TMP_NVSYNC := '0';
			TMP_NHSYNC := '0';
		end if;
		-- native (+upscaled) VCOUNT and HCOUNT
		if rising_edge(NPIXCLK) then
			-- sync write (needs to be here to happen on rising edge)
			NVCOUNT <= TMP_NVCOUNT;
			NHCOUNT <= TMP_NHCOUNT;
			NHACTIVE <= TMP_NHACTIVE;
			NVACTIVE <= TMP_NVACTIVE;
			NACTIVE <= TMP_NACTIVE;
			NVSYNC <= TMP_NVSYNC;
			NHSYNC <= TMP_NHSYNC;
			N_POS_X <= resize(TMP_NHCOUNT - PPU_VGA_H_PORCH_BACK, N_POS_X'length) when TMP_NHACTIVE = '1' else (others => '0');
			N_POS_Y <= resize(TMP_NVCOUNT - PPU_VGA_V_PORCH_BACK, N_POS_Y'length) when TMP_NVACTIVE = '1' else (others => '0');

			-- horizontal count (pixel)
			TMP_NHCOUNT := TMP_NHCOUNT + 1;
			if TMP_NHCOUNT >= PPU_VGA_H_TOTAL then
				TMP_NHCOUNT := (others => '0');

				-- vertical count (scanline)
				TMP_NVCOUNT := TMP_NVCOUNT + 1;
				if TMP_NVCOUNT >= PPU_VGA_V_TOTAL then
					TMP_NVCOUNT := (others => '0');
				end if;

				-- vertical display area (active)
				if TMP_NVCOUNT = PPU_VGA_V_PORCH_BACK then TMP_NVACTIVE := '1'; end if;
				if TMP_NVCOUNT = PPU_VGA_V_PORCH_BACK + PPU_VGA_V_ACTIVE then TMP_NVACTIVE := '0'; end if;

				-- vertical sync period
				if TMP_NVCOUNT = PPU_VGA_V_PORCH_BACK + PPU_VGA_V_ACTIVE then TMP_NVSYNC := '1'; end if;
				if TMP_NVCOUNT = PPU_VGA_V_PORCH_BACK + PPU_VGA_V_ACTIVE + PPU_VGA_V_SYNC then TMP_NVSYNC := '0'; end if;
			end if;

			-- horizontal display area (active)
			if TMP_NHCOUNT = PPU_VGA_H_PORCH_BACK then TMP_NHACTIVE := '1'; end if;
			if TMP_NHCOUNT = PPU_VGA_H_PORCH_BACK + PPU_VGA_H_ACTIVE then TMP_NHACTIVE := '0'; end if;
			TMP_NACTIVE := TMP_NHACTIVE and TMP_NVACTIVE;

			-- horizontal sync period
			if TMP_NHCOUNT = PPU_VGA_H_PORCH_BACK + PPU_VGA_H_ACTIVE then TMP_NHSYNC := '1'; end if;
			if TMP_NHCOUNT = PPU_VGA_H_PORCH_BACK + PPU_VGA_H_ACTIVE + PPU_VGA_H_SYNC then TMP_NHSYNC := '0'; end if;
		end if;

		if rising_edge(TPIXCLK) then
			T_POS_X <= TMP_T_POS_X;

			if TMP_NACTIVE = '1' then
				TMP_T_POS_X := TMP_T_POS_X + 1;
				if TMP_T_POS_X >= PPU_SCREEN_WIDTH then
					TMP_T_POS_X := (others => '0');
				end if;
			end if;
		end if;
	end process;
	ACTIVE <= NACTIVE;

	scanline_buffer : component ppu_dispctl_slbuf port map(
		clka => SYSCLK,
		wea => (others => BWEN),
		addra => ADDR_I,
		dina => DATA_I,
		clkb => SYSCLK,
		addrb => ADDR_O,
		doutb => DATA_O);

	pixel_clock: component ppu_dispctl_pixclk port map(
    sysclk => SYSCLK,
    reset => RESET,
    npxclk => NPIXCLK,
    tpxclk => TPIXCLK);

	THBLANK <= (not NHACTIVE) and NVACTIVE and nor(T_POS_X);
	TVBLANK <= not NVACTIVE;
	--THBLANK <= '0';
	--TVBLANK <= '0';
end Behavioral;
