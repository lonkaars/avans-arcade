library ieee;
library work;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.ppu_consts.all;

entity ppu_sprite_transform is port( -- flip sprites
	XI : in std_logic_vector(PPU_SPRITE_POS_H_WIDTH-1 downto 0); -- pixel position relative to tile
	YI : in std_logic_vector(PPU_SPRITE_POS_V_WIDTH-1 downto 0); -- pixel position relative to tile
	FLIP_H, FLIP_V : in std_logic; -- flip sprite
	XO : out std_logic_vector(PPU_SPRITE_POS_H_WIDTH-1 downto 0); -- new pixel position relative to tile
	YO : out std_logic_vector(PPU_SPRITE_POS_V_WIDTH-1 downto 0)); -- new pixel position relative to tile
end ppu_sprite_transform;

architecture Behavioral of ppu_sprite_transform is
	signal FLIPPED_X : std_logic_vector(PPU_SPRITE_POS_H_WIDTH-1 downto 0);
	signal FLIPPED_Y : std_logic_vector(PPU_SPRITE_POS_V_WIDTH-1 downto 0);
begin
	FLIPPED_X <= std_logic_vector(to_unsigned(PPU_SPRITE_WIDTH-1 - to_integer(unsigned(XI)), PPU_SPRITE_POS_H_WIDTH));
	FLIPPED_Y <= std_logic_vector(to_unsigned(PPU_SPRITE_HEIGHT-1 - to_integer(unsigned(YI)), PPU_SPRITE_POS_V_WIDTH));

	XO <= FLIPPED_X when FLIP_H = '1' else XI;
	YO <= FLIPPED_Y when FLIP_V = '1' else YI;
end Behavioral;
