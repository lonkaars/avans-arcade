library ieee;
use ieee.std_logic_1164.all;
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
	signal RGB_COLOR : std_logic_vector(PPU_RGB_COLOR_OUTPUT_DEPTH-1 downto 0);
begin
	RGB_COLOR <= RI & GI & BI;

	scanline_buffer : component ppu_dispctl_slbuf port map(
		clka => CLK,
		wea => (others => PREADY),
		addra => (others => '0'),
		dina => RGB_COLOR,
		clkb => CLK,
		rstb => RESET,
		addrb => (others => '0'),
		doutb => open,
		rsta_busy => open,
		rstb_busy => open);
end Behavioral;
