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
	PREADY : in std_logic; -- current pixel ready (pixel color is stable)

	RO,GO,BO : out std_logic_vector(PPU_COLOR_OUTPUT_DEPTH-1 downto 0); -- VGA color out
	NVSYNC, NHSYNC : out std_logic; -- VGA sync out
	THBLANK, TVBLANK : out std_logic); -- tiny sync signals
end ppu_dispctl;

architecture Behavioral of ppu_dispctl is
	component ppu_dispctl_pixclk is port ( 
		clk_out1 : out std_logic;
		clk_out2 : out std_logic;
		reset : in std_logic;
		clk_in1 : in std_logic);
	end component;
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
	signal NPIXCLK, TPIXCLK : std_logic;
	signal NHCOUNT, NVCOUNT,
	       THCOUNT, TVCOUNT: unsigned(PPU_VGA_SIGNAL_PIXEL_WIDTH-1 downto 0) := (others => '0');
	signal ADDR_I, ADDR_O : std_logic_vector(PPU_DISPCTL_SLBUF_ADDR_WIDTH-1 downto 0);
	signal DATA_I, DATA_O : std_logic_vector(PPU_RGB_COLOR_OUTPUT_DEPTH-1 downto 0);
	signal T_POS_X : unsigned(PPU_SCREEN_T_POS_X_WIDTH-1 downto 0) := (others => '0'); -- real tiny x position
	signal T_POS_Y : unsigned(PPU_SCREEN_T_POS_Y_WIDTH-1 downto 0) := (others => '0'); -- real tiny y position
	signal U_POS_X : unsigned(PPU_SCREEN_T_POS_X_WIDTH-1 downto 0) := (others => '0'); -- upscaled tiny x position
	signal U_POS_Y : unsigned(PPU_SCREEN_T_POS_Y_WIDTH-1 downto 0) := (others => '0'); -- upscaled tiny y position
	signal N_POS_X : unsigned(PPU_SCREEN_N_POS_X_WIDTH-1 downto 0) := (others => '0'); -- native x position
	signal N_POS_Y : unsigned(PPU_SCREEN_N_POS_Y_WIDTH-1 downto 0) := (others => '0'); -- native y position
	signal TMP_X : std_logic_vector(PPU_POS_H_WIDTH-1 downto 0) := (others => '0'); -- tiny screen pixel x
	signal TMP_Y : std_logic_vector(PPU_POS_V_WIDTH-1 downto 0) := (others => '0'); -- tiny screen pixel y
	signal TMP_RO, TMP_GO, TMP_BO : std_logic_vector(PPU_COLOR_OUTPUT_DEPTH-1 downto 0) := (others => '0'); -- VGA color out
	signal TMP_THBLANK, TMP_TVBLANK : std_logic := '0'; -- tiny sync signals

	signal NACTIVE, NHACTIVE, NVACTIVE : std_logic := '0';
	signal TACTIVE, THACTIVE, TVACTIVE : std_logic := '0';
begin
	-- native (TODO: +upscaled) VCOUNT and HCOUNT
	process(NPIXCLK, RESET)
		variable TMP_NHCOUNT, TMP_NVCOUNT : unsigned(PPU_VGA_SIGNAL_PIXEL_WIDTH-1 downto 0) := (others => '0');
		variable TMP_NHACTIVE, TMP_NVACTIVE : std_logic := '0';
		variable TMP_NHSYNC, TMP_NVSYNC : std_logic := '0';
	begin
		NVCOUNT <= TMP_NVCOUNT;
		NHCOUNT <= TMP_NHCOUNT;
		NHACTIVE <= TMP_NHACTIVE;
		NVACTIVE <= TMP_NVACTIVE;
		NACTIVE <= TMP_NHACTIVE and TMP_NVACTIVE;
		NVSYNC <= TMP_NVSYNC;
		NHSYNC <= TMP_NHSYNC;
		N_POS_X <= resize(TMP_NHCOUNT - PPU_VGA_H_PORCH_BACK, N_POS_X'length) when TMP_NHACTIVE = '1' else (others => '0');
		N_POS_Y <= resize(TMP_NVCOUNT - PPU_VGA_V_PORCH_BACK, N_POS_Y'length) when TMP_NVACTIVE = '1' else (others => '0');

		if RESET = '1' then
			TMP_NHCOUNT := (others => '0');
			TMP_NVCOUNT := (others => '0');
			TMP_NHACTIVE := '0';
			TMP_NVACTIVE := '0';
			TMP_NVSYNC := '0';
			TMP_NHSYNC := '0';
		elsif rising_edge(NPIXCLK) then
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

			-- horizontal sync period
			if TMP_NHCOUNT = PPU_VGA_H_PORCH_BACK + PPU_VGA_H_ACTIVE then TMP_NHSYNC := '1'; end if;
			if TMP_NHCOUNT = PPU_VGA_H_PORCH_BACK + PPU_VGA_H_ACTIVE + PPU_VGA_H_SYNC then TMP_NHSYNC := '0'; end if;
		end if;
	end process;

	-- tiny VCOUNT and HCOUNT
	process(TPIXCLK, RESET)
		variable TMP_THCOUNT, TMP_TVCOUNT: unsigned(PPU_VGA_SIGNAL_PIXEL_WIDTH-1 downto 0) := (others => '0');
	begin
		TVCOUNT <= TMP_TVCOUNT;
		THCOUNT <= TMP_THCOUNT;
		if RESET = '1' then
			TMP_THCOUNT := (others => '0');
			TMP_TVCOUNT := (others => '0');
		elsif rising_edge(TPIXCLK) then
			TMP_THCOUNT := TMP_THCOUNT + 1;
			if TMP_THCOUNT >= PPU_VGA_H_TOTAL then
				TMP_THCOUNT := (others => '0');

				TMP_TVCOUNT := TMP_TVCOUNT + 1;
				if TMP_TVCOUNT >= PPU_VGA_V_TOTAL then
					TMP_TVCOUNT := (others => '0');
				end if;
			end if;
		end if;
	end process;

	-- T_POS_X <= resize(THCOUNT - (PPU_VGA_H_PORCH_BACK / 4), T_POS_X'length) when N_POS_Y(0) = to_unsigned(PPU_VGA_V_PORCH_BACK, 1)(0) else
	--            resize(THCOUNT - ((PPU_VGA_H_PORCH_BACK + PPU_VGA_H_BLANK) / 4), T_POS_X'length); -- divide tiny x equally over two native scanlines
	-- T_POS_Y <= resize(TVCOUNT - (PPU_VGA_V_PORCH_BACK / 2), T_POS_Y'length);

	-- DATA_I <= RI & GI & BI;
	-- ADDR_I <= std_logic_vector(resize(T_POS_X, ADDR_I'length)) when T_POS_Y(0) = '0' else std_logic_vector(resize(T_POS_X, ADDR_I'length) + PPU_SCREEN_WIDTH);

	-- TMP_X <= std_logic_vector(T_POS_X) when NACTIVE = '1' else (others => '0');
	-- TMP_Y <= std_logic_vector(T_POS_Y) when NACTIVE = '1' else (others => '0');

	U_POS_X <= resize(N_POS_X(N_POS_X'length-1 downto 1), U_POS_X'length);
	U_POS_Y <= resize(N_POS_Y(N_POS_Y'length-1 downto 1), U_POS_Y'length);
	-- U_POS_X <= resize(N_POS_X / 2, U_POS_X'length);
	-- U_POS_Y <= resize(N_POS_Y / 2, U_POS_Y'length);

	ADDR_O <= std_logic_vector(resize(U_POS_X, ADDR_I'length)) when U_POS_Y(0) = '0' else std_logic_vector(resize(U_POS_X, ADDR_I'length) + PPU_SCREEN_WIDTH);
	TMP_RO <= DATA_O(11 downto 8) when NACTIVE = '1' else (others => '0');
	TMP_GO <= DATA_O(7 downto 4) when NACTIVE = '1' else (others => '0');
	TMP_BO <= DATA_O(3 downto 0) when NACTIVE = '1' else (others => '0');

	scanline_buffer : component ppu_dispctl_slbuf port map(
		clka => SYSCLK,
		wea => (others => PREADY),
		addra => ADDR_I,
		dina => DATA_I,
		clkb => SYSCLK,
		rstb => RESET,
		addrb => ADDR_O,
		doutb => DATA_O,
		rsta_busy => open,
		rstb_busy => open);

	pixel_clock: component ppu_dispctl_pixclk port map(
    clk_in1 => SYSCLK,
    reset => RESET,
    clk_out1 => NPIXCLK,
    clk_out2 => TPIXCLK);

	process(NPIXCLK, RESET)
	begin
		if RESET = '1' then
			X <= (others => '0');
			Y <= (others => '0');
			RO <= (others => '0');
			GO <= (others => '0');
			BO <= (others => '0');
			THBLANK <= '0';
			TVBLANK <= '0';
		elsif rising_edge(NPIXCLK) then
			X <= TMP_X;
			Y <= TMP_Y;
			RO <= TMP_RO;
			GO <= TMP_GO;
			BO <= TMP_BO;
			THBLANK <= TMP_THBLANK;
			TVBLANK <= TMP_TVBLANK;
		end if;
	end process;
end Behavioral;
