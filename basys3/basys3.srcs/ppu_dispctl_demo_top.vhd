library ieee;
library work;

use ieee.std_logic_1164.all;
use work.ppu_consts.all;

entity ppu_dispctl_demo is port(
	CLK100 : in std_logic; -- system clock
	RESET : in std_logic; -- global (async) system reset
	R,G,B : out std_logic_vector(PPU_COLOR_OUTPUT_DEPTH-1 downto 0);
	VSYNC, HSYNC : out std_logic); -- vblank for synchronization
end ppu_dispctl_demo;

architecture Behavioral of ppu_dispctl_demo is
	component ppu_dispctl port( -- display controller
		CLK : in std_logic; -- system clock
		RESET : in std_logic;

		X : out std_logic_vector(PPU_POS_H_WIDTH-1 downto 0); -- tiny screen pixel x
		Y : out std_logic_vector(PPU_POS_V_WIDTH-1 downto 0); -- tiny screen pixel y
		RI,GI,BI : in std_logic_vector(PPU_COLOR_OUTPUT_DEPTH-1 downto 0); -- color in
		PREADY : in std_logic; -- current pixel ready (pixel color is stable)

		RO,GO,BO : out std_logic_vector(PPU_COLOR_OUTPUT_DEPTH-1 downto 0); -- VGA color out
		NVSYNC, NHSYNC : out std_logic; -- VGA sync out
		THBLANK, TVBLANK : out std_logic); -- tiny sync signals
	end component;
begin
	display_controller : component ppu_dispctl port map(
		CLK => CLK100,
		RESET => RESET,
		PREADY => '1',
		X => open,
		Y => open,
		RI => (others => '1'),
		GI => (others => '0'),
		BI => (others => '1'),
		RO => R,
		GO => G,
		BO => B,
		NVSYNC => VSYNC,
		NHSYNC => HSYNC,
		TVBLANK => open,
		THBLANK => open);
end Behavioral;
