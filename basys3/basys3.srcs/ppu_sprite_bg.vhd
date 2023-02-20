library ieee;
library work;

use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;
use work.ppu_consts.all;

entity ppu_sprite_bg is port(
	-- inputs
	CLK : in std_logic; -- pipeline clock
	RESET : in std_logic; -- reset clock counter
	OE : in std_logic; -- output enable (of CIDX)
	X : in std_logic_vector(PPU_POS_H_WIDTH-1 downto 0); -- current screen pixel x
	Y : in std_logic_vector(PPU_POS_V_WIDTH-1 downto 0); -- current screen pixel y
	
	-- aux inputs
	BG_SHIFT_X : in std_logic_vector(PPU_POS_H_WIDTH-1 downto 0);
	BG_SHIFT_Y : in std_logic_vector(PPU_POS_V_WIDTH-1 downto 0);

	-- used memory blocks
	BAM_ADDR : out std_logic_vector(PPU_BAM_ADDR_WIDTH-1 downto 0);
	BAM_DATA : in std_logic_vector(PPU_BAM_DATA_WIDTH-1 downto 0);
	TMM_ADDR : out std_logic_vector(PPU_TMM_ADDR_WIDTH-1 downto 0);
	TMM_DATA : in std_logic_vector(PPU_TMM_DATA_WIDTH-1 downto 0);

	-- outputs
	CIDX : out std_logic_vector(PPU_PALETTE_CIDX_WIDTH-1 downto 0)); -- output color
end ppu_sprite_bg;

architecture Behavioral of ppu_sprite_bg is
	component ppu_sprite_transform port(
		XI : in std_logic_vector(PPU_SPRITE_POS_H_WIDTH-1 downto 0); -- pixel position relative to tile
		YI : in std_logic_vector(PPU_SPRITE_POS_V_WIDTH-1 downto 0); -- pixel position relative to tile
		FLIP_H, FLIP_V : in std_logic; -- flip sprite
		XO : out std_logic_vector(PPU_SPRITE_POS_H_WIDTH-1 downto 0); -- new pixel position relative to tile
		YO : out std_logic_vector(PPU_SPRITE_POS_V_WIDTH-1 downto 0)); -- new pixel position relative to tile
	end component;

	signal O_BAM_ADDR : std_logic_vector(PPU_BAM_ADDR_WIDTH-1 downto 0) := (others => '0');
	signal I_BAM_DATA : std_logic_vector(PPU_BAM_DATA_WIDTH-1 downto 0);
	signal O_TMM_ADDR : std_logic_vector(PPU_TMM_ADDR_WIDTH-1 downto 0) := (others => '0');
	signal I_TMM_DATA : std_logic_vector(PPU_TMM_DATA_WIDTH-1 downto 0);

	signal FLIP_H, FLIP_V : std_logic := '0';
	signal TMM_IDX : std_logic_vector(PPU_TILE_INDEX_WIDTH-1 downto 0) := (others => '0');

	alias BAM_DATA_COL_IDX is I_BAM_DATA(12 downto 10);
	signal TMM_DATA_PAL_IDX : std_logic_vector(PPU_PALETTE_COLOR_WIDTH-1 downto 0);
	signal O_CIDX : std_logic_vector(PPU_PALETTE_CIDX_WIDTH-1 downto 0) := (others => '0');

	type states is (PL_BAM_ADDR, PL_BAM_DATA, PL_TMM_ADDR, PL_TMM_DATA);
	signal state, next_state : states := PL_BAM_ADDR;

	-- docs/architecture.md#background-attribute-memory
	alias BAM_DATA_FLIP_H is I_BAM_DATA(14);
	alias BAM_DATA_FLIP_V is I_BAM_DATA(13);
	alias BAM_DATA_TILE_IDX is I_BAM_DATA(9 downto 0);

	signal PIXEL_ABS_X, PIXEL_ABS_Y : integer := 0; -- absolute pixel position (relative to BG canvas instead of viewport)
	signal TILE_IDX_X, TILE_IDX_Y : integer := 0; -- background canvas tile grid xy
	signal TILE_PIXEL_IDX_X, TILE_PIXEL_IDX_Y : integer := 0; -- xy position of pixel within tile
	signal TILE_PIXEL_IDX : integer := 0; -- index of pixel within tile
	signal TRANS_TILE_PIDX_X, TRANS_TILE_PIDX_Y : integer := 0; -- transformed xy position of pixel within tile
	signal TILEMAP_WORD_OFFSET : integer := 0;
	signal TRANSFORM_XI, TRANSFORM_XO : std_logic_vector(PPU_SPRITE_POS_H_WIDTH-1 downto 0);
	signal TRANSFORM_YI, TRANSFORM_YO : std_logic_vector(PPU_SPRITE_POS_V_WIDTH-1 downto 0);
	signal PIXEL_BIT_OFFSET : integer := 0;
begin
	-- CIDX tri-state driver
	CIDX <= O_CIDX when OE = '1' else (others => 'Z');

	-- internal line separators
	FLIP_H <= BAM_DATA_FLIP_H;
	FLIP_V <= BAM_DATA_FLIP_V;
	O_CIDX <= BAM_DATA_COL_IDX & TMM_DATA_PAL_IDX;

	-- BAM ADDR
	PIXEL_ABS_X <= to_integer(unsigned(X)) + to_integer(unsigned(BG_SHIFT_X));
	PIXEL_ABS_Y <= to_integer(unsigned(Y)) + to_integer(unsigned(BG_SHIFT_Y));
	TILE_IDX_X <= PIXEL_ABS_X / 16;
	TILE_IDX_Y <= PIXEL_ABS_Y / 16;
	TILE_PIXEL_IDX_X <= PIXEL_ABS_X - TILE_IDX_X * 16;
	TILE_PIXEL_IDX_Y <= PIXEL_ABS_Y - TILE_IDX_Y * 16;
	O_BAM_ADDR <= std_logic_vector(to_unsigned((TILE_IDX_Y * integer(PPU_BG_CANVAS_TILES_H)) + TILE_IDX_X, PPU_BAM_ADDR_WIDTH));

	-- BAM DATA + FAM ADDR
	TRANSFORM_XI <= std_logic_vector(to_unsigned(TILE_PIXEL_IDX_X, PPU_SPRITE_POS_H_WIDTH));
	TRANSFORM_YI <= std_logic_vector(to_unsigned(TILE_PIXEL_IDX_Y, PPU_SPRITE_POS_V_WIDTH));
	transform: component ppu_sprite_transform port map(
		XI => TRANSFORM_XI,
		YI => TRANSFORM_YI,
		FLIP_H => FLIP_H,
		FLIP_V => FLIP_V,
		XO => TRANSFORM_XO,
		YO => TRANSFORM_YO);
	TRANS_TILE_PIDX_X <= to_integer(unsigned(TRANSFORM_XO));
	TRANS_TILE_PIDX_Y <= to_integer(unsigned(TRANSFORM_YO));

	TILE_PIXEL_IDX <= integer(PPU_SPRITE_WIDTH) * TRANS_TILE_PIDX_Y + TRANS_TILE_PIDX_X;
	TILEMAP_WORD_OFFSET <= TILE_PIXEL_IDX / PPU_PIXELS_PER_TILE_WORD;
	PIXEL_BIT_OFFSET <= TILE_PIXEL_IDX mod PPU_PIXELS_PER_TILE_WORD;

	O_TMM_ADDR <= std_logic_vector(to_unsigned(PPU_SPRITE_PIXELS_PER_WORD * to_integer(unsigned(BAM_DATA_TILE_IDX)) + TILEMAP_WORD_OFFSET, PPU_TMM_ADDR_WIDTH));

	-- TMM DATA
	with PIXEL_BIT_OFFSET select
		TMM_DATA_PAL_IDX <= TMM_DATA(2 downto 0) when 0,
		                    TMM_DATA(5 downto 3) when 1,
		                    TMM_DATA(8 downto 6) when 2,
		                    TMM_DATA(11 downto 9) when 3,
		                    TMM_DATA(14 downto 12) when 4,
		                    (others => '0') when others;

	-- state machine (pipeline stage counter)
	fsm: process(CLK, RESET)
	begin
		if RESET = '1' then
			state <= PL_BAM_ADDR;
		elsif rising_edge(CLK) then
			state <= next_state;
		end if;
	end process;

	-- sync read/write
	process(state)
	begin
		next_state <= state;
		case state is
			when PL_BAM_ADDR =>
				next_state <= PL_BAM_DATA;
				BAM_ADDR <= O_BAM_ADDR;

			when PL_BAM_DATA =>
				next_state <= PL_TMM_ADDR;
				I_BAM_DATA <= BAM_DATA;
				
			when PL_TMM_ADDR =>
				next_state <= PL_TMM_DATA;
				TMM_ADDR <= O_TMM_ADDR;

			when PL_TMM_DATA =>
				next_state <= PL_BAM_ADDR;
				I_TMM_DATA <= TMM_DATA;

				
		end case;
	end process;

end Behavioral;
