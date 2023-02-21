library ieee;
library unisim;
library work;

use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use unisim.vcomponents.all;
use work.ppu_consts.all;

entity ppu_sprite_transform_tb is
end ppu_sprite_transform_tb;

architecture behavioral of ppu_sprite_transform_tb is
	component ppu_sprite_transform port( -- flip sprites
		XI : in std_logic_vector(PPU_SPRITE_POS_H_WIDTH-1 downto 0); -- pixel position relative to tile
		YI : in std_logic_vector(PPU_SPRITE_POS_V_WIDTH-1 downto 0); -- pixel position relative to tile
		FLIP_H, FLIP_V : in std_logic; -- flip sprite
		XO : out std_logic_vector(PPU_SPRITE_POS_H_WIDTH-1 downto 0); -- new pixel position relative to tile
		YO : out std_logic_vector(PPU_SPRITE_POS_V_WIDTH-1 downto 0)); -- new pixel position relative to tile
	end component;

	signal XI : std_logic_vector(PPU_SPRITE_POS_H_WIDTH-1 downto 0);
	signal YI : std_logic_vector(PPU_SPRITE_POS_V_WIDTH-1 downto 0);
	signal FLIP_H, FLIP_V : std_logic := '0';
	signal XO : std_logic_vector(PPU_SPRITE_POS_H_WIDTH-1 downto 0);
	signal YO : std_logic_vector(PPU_SPRITE_POS_V_WIDTH-1 downto 0);
begin
	uut : component ppu_sprite_transform
		port map(
			XI => XI,
			YI => YI,
			FLIP_H => FLIP_H,
			FLIP_V => FLIP_V,
			XO => XO,
			YO => YO);

	tb : process
	begin
		XI <= std_logic_vector(to_unsigned(4, PPU_SPRITE_POS_H_WIDTH));
		YI <= std_logic_vector(to_unsigned(6, PPU_SPRITE_POS_V_WIDTH));
		wait for 5 ns;

		XI <= std_logic_vector(to_unsigned(2, PPU_SPRITE_POS_H_WIDTH));
		YI <= std_logic_vector(to_unsigned(14, PPU_SPRITE_POS_V_WIDTH));
		wait for 5 ns;

		FLIP_H <= '1';
		wait for 5 ns;

		FLIP_V <= '1';
		wait for 5 ns;

		XI <= std_logic_vector(to_unsigned(6, PPU_SPRITE_POS_H_WIDTH));
		YI <= std_logic_vector(to_unsigned(8, PPU_SPRITE_POS_V_WIDTH));
		wait for 5 ns;

		FLIP_H <= '0';
		wait for 5 ns;

		XI <= std_logic_vector(to_unsigned(2, PPU_SPRITE_POS_H_WIDTH));
		YI <= std_logic_vector(to_unsigned(14, PPU_SPRITE_POS_V_WIDTH));
		wait for 5 ns;

		wait; -- stop for simulator
	end process;
end;
