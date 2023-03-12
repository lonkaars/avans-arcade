library ieee;
library work;

use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;
use work.ppu_consts.all;

entity ppu_dispctl_demo is port(
	CLK100 : in std_logic; -- system clock
	RESET : in std_logic; -- global (async) system reset
	R,G,B : out std_logic_vector(PPU_COLOR_OUTPUT_DEPTH-1 downto 0);
	VSYNC, HSYNC : out std_logic); -- vblank for synchronization
end ppu_dispctl_demo;

architecture Behavioral of ppu_dispctl_demo is
	component ppu_dispctl port( -- display controller
		SYSCLK : in std_logic; -- system clock
		RESET : in std_logic;

		X : out std_logic_vector(PPU_POS_H_WIDTH-1 downto 0); -- tiny screen pixel x
		Y : out std_logic_vector(PPU_POS_V_WIDTH-1 downto 0); -- tiny screen pixel y
		RI,GI,BI : in std_logic_vector(PPU_COLOR_OUTPUT_DEPTH-1 downto 0); -- color in
		PREADY : in std_logic; -- current pixel ready (pixel color is stable)

		RO,GO,BO : out std_logic_vector(PPU_COLOR_OUTPUT_DEPTH-1 downto 0); -- VGA color out
		NVSYNC, NHSYNC : out std_logic; -- VGA sync out
		THBLANK, TVBLANK : out std_logic); -- tiny sync signals
	end component;
	component ppu_dispctl_test_img port ( 
		clka : in std_logic;
		addra : in std_logic_vector (16 downto 0);
		douta : out std_logic_vector (11 downto 0));
	end component;
	signal ADDR : std_logic_vector (16 downto 0);
	signal DATA : std_logic_vector (11 downto 0);
	signal X : std_logic_vector(PPU_POS_H_WIDTH-1 downto 0);
	signal Y : std_logic_vector(PPU_POS_V_WIDTH-1 downto 0);

	alias DATA_R is DATA(11 downto 8);
	alias DATA_G is DATA(7 downto 4);
	alias DATA_B is DATA(3 downto 0);
begin
	ADDR <= std_logic_vector(resize(unsigned(X) + unsigned(Y) * to_unsigned(PPU_SCREEN_WIDTH, ADDR'length), ADDR'length));

	test_img : component ppu_dispctl_test_img port map(
		clka => CLK100,
		addra => ADDR,
		douta => DATA);

	display_controller : component ppu_dispctl port map(
		SYSCLK => CLK100,
		RESET => RESET,
		PREADY => '1',
		X => X,
		Y => Y,
		RI => DATA_R,
		GI => DATA_G,
		BI => DATA_B,
		RO => R,
		GO => G,
		BO => B,
		NVSYNC => VSYNC,
		NHSYNC => HSYNC,
		TVBLANK => open,
		THBLANK => open);
end Behavioral;
