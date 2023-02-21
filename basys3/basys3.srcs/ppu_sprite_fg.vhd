library ieee;
library work;

use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;
use work.ppu_consts.all;

-- TODO: add input stable / output stable pipeline stages if this doesn't work with propagation delays
entity ppu_sprite_fg is -- foreground sprite
	generic (
		IDX : natural := 0); -- sprite index number
	port(
		-- inputs
		CLK : in std_logic; -- system clock
		PL_CLK : in std_logic; -- pipeline clock
		RESET : in std_logic; -- reset internal memory and clock counters
		OE : in std_logic; -- output enable (of CIDX)
		X : in std_logic_vector(PPU_POS_H_WIDTH-1 downto 0); -- current screen pixel x
		Y : in std_logic_vector(PPU_POS_V_WIDTH-1 downto 0); -- current screen pixel y
		FETCH : in std_logic; -- fetch sprite data from TMM

		-- internal memory block (FAM)
		FAM_WEN : in std_logic; -- VRAM FAM write enable
		FAM_ADDR : in std_logic_vector(PPU_FAM_ADDR_WIDTH-1 downto 0); -- VRAM fam address
		FAM_DATA : in std_logic_vector(PPU_FAM_DATA_WIDTH-1 downto 0); -- VRAM fam data

		-- used memory blocks
		TMM_ADDR : out std_logic_vector(PPU_TMM_ADDR_WIDTH-1 downto 0);
		TMM_DATA : in std_logic_vector(PPU_TMM_DATA_WIDTH-1 downto 0);

		-- outputs
		CIDX : out std_logic_vector(PPU_PALETTE_CIDX_WIDTH-1 downto 0); -- output color
		HIT : out std_logic); -- current pixel is not transparent
end ppu_sprite_fg;

architecture Behavioral of ppu_sprite_fg is
	component ppu_sprite_transform port(
		XI : in std_logic_vector(PPU_SPRITE_POS_H_WIDTH-1 downto 0); -- pixel position relative to tile
		YI : in std_logic_vector(PPU_SPRITE_POS_V_WIDTH-1 downto 0); -- pixel position relative to tile
		FLIP_H, FLIP_V : in std_logic; -- flip sprite
		XO : out std_logic_vector(PPU_SPRITE_POS_H_WIDTH-1 downto 0); -- new pixel position relative to tile
		YO : out std_logic_vector(PPU_SPRITE_POS_V_WIDTH-1 downto 0)); -- new pixel position relative to tile
	end component;
	component er_ram -- exposed register RAM
		generic(
			ADDR_W : natural := PPU_FAM_ADDR_WIDTH; -- ADDR line width
			DATA_W : natural := PPU_FAM_DATA_WIDTH; -- DATA line width
			ADDR_LOW : natural := IDX*2; -- starting address
			ADDR_RANGE : natural := 2); -- amount of valid addresses after ADDR_LOW
		port(
			CLK : in std_logic; -- clock
			RST : in std_logic; -- async memory clear
			WEN : in std_logic; -- write enable
			ADDR : in std_logic_vector(ADDR_W-1 downto 0); -- address line
			DATA : in std_logic_vector(DATA_W-1 downto 0); -- data input
			REG : out std_logic_vector((ADDR_RANGE*DATA_W)-1 downto 0)); -- exposed register output
	end component;

	-- FAM and TMM in/out temp + registers
	signal T_TMM_ADDR, R_TMM_ADDR : std_logic_vector(PPU_TMM_ADDR_WIDTH-1 downto 0) := (others => '0');
	signal T_TMM_DATA, R_TMM_DATA : std_logic_vector(PPU_TMM_DATA_WIDTH-1 downto 0) := (others => '0');

	-- state machine for synchronizing pipeline stages
	type states is (PL_TMM_ADDR, PL_TMM_DATA);
	signal state : states := PL_TMM_ADDR;

	-- auxiliary signals (temp variables)
	signal T_CIDX : std_logic_vector(PPU_PALETTE_CIDX_WIDTH-1 downto 0) := (others => '0'); -- output color buffer/register
	signal INT_FAM : std_logic_vector(31 downto 0);

	-- FAM aliases (docs/architecture.md#foreground-attribute-memory)
	alias FAM_REG_FLIP_H is INT_FAM(31); -- Flip horizontally
	alias FAM_REG_FLIP_V is INT_FAM(30); -- Flip vertically
	alias FAM_REG_POS_H is INT_FAM(29 downto 21); -- horizontal position (offset by -16)
	alias FAM_REG_POS_V is INT_FAM(20 downto 13); -- vertical position (offset by -16)
	alias FAM_REG_COL_IDX is INT_FAM(12 downto 10); -- Palette index for tile
	alias FAM_REG_TILE_IDX is INT_FAM(9 downto 0); -- Tilemap index

	-- signal PIXEL_ABS_X, PIXEL_ABS_Y : integer := 0; -- absolute pixel position (relative to FG canvas instead of viewport)
	-- signal TILE_IDX_X, TILE_IDX_Y : integer := 0; -- background canvas tile grid xy
	-- signal TILE_PIXEL_IDX_X, TILE_PIXEL_IDX_Y : integer := 0; -- xy position of pixel within tile (local tile coords)
	-- signal TRANS_TILE_PIDX_X, TRANS_TILE_PIDX_Y : integer := 0; -- transformed xy position of pixel within tile
	-- signal TRANS_TILE_PIXEL_IDX : integer := 0; -- index of pixel within tile (reading order)
	-- signal TRANSFORM_XI, TRANSFORM_XO : std_logic_vector(PPU_SPRITE_POS_H_WIDTH-1 downto 0); -- transform inputs/outputs (x axis)
	-- signal TRANSFORM_YI, TRANSFORM_YO : std_logic_vector(PPU_SPRITE_POS_V_WIDTH-1 downto 0); -- transform inputs/outputs (y axis)
	-- signal TILEMAP_WORD_OFFSET : integer := 0; -- word offset from tile start address in TMM
	-- signal PIXEL_BIT_OFFSET : integer := 0; -- pixel index within word of TMM
	signal TMM_DATA_PAL_IDX : std_logic_vector(PPU_PALETTE_COLOR_WIDTH-1 downto 0); -- color of palette
begin
	-- output drivers
	CIDX <= T_CIDX when OE = '1' else (others => 'Z');
	TMM_ADDR <= R_TMM_ADDR;
	T_TMM_DATA <= TMM_DATA;
	-- CIDX combination
	T_CIDX <= FAM_REG_COL_IDX & TMM_DATA_PAL_IDX;

	-- FAM memory
	FAM : component er_ram port map(
		CLK => CLK,
		RST => RESET,
		WEN => FAM_WEN,
		ADDR => FAM_ADDR,
		DATA => FAM_DATA,
		REG => INT_FAM);

	-- -- -- FAM address calculations
	-- -- PIXEL_ABS_X <= to_integer(unsigned(X)) + to_integer(unsigned(FG_SHIFT_X));
	-- -- PIXEL_ABS_Y <= to_integer(unsigned(Y)) + to_integer(unsigned(FG_SHIFT_Y));
	-- -- TILE_IDX_X <= PIXEL_ABS_X / 16;
	-- -- TILE_IDX_Y <= PIXEL_ABS_Y / 16;
	-- -- TILE_PIXEL_IDX_X <= PIXEL_ABS_X - TILE_IDX_X * 16;
	-- -- TILE_PIXEL_IDX_Y <= PIXEL_ABS_Y - TILE_IDX_Y * 16;
	-- -- T_FAM_ADDR <= std_logic_vector(to_unsigned((TILE_IDX_Y * integer(PPU_FG_CANVAS_TILES_H)) + TILE_IDX_X, PPU_FAM_ADDR_WIDTH));

	-- -- -- FAM data dependant calculations
	-- -- TRANSFORM_XI <= std_logic_vector(to_unsigned(TILE_PIXEL_IDX_X, PPU_SPRITE_POS_H_WIDTH));
	-- -- TRANSFORM_YI <= std_logic_vector(to_unsigned(TILE_PIXEL_IDX_Y, PPU_SPRITE_POS_V_WIDTH));
	-- -- transform: component ppu_sprite_transform port map(
	-- -- 	XI => TRANSFORM_XI,
	-- -- 	YI => TRANSFORM_YI,
	-- -- 	FLIP_H => FAM_DATA_FLIP_H,
	-- -- 	FLIP_V => FAM_DATA_FLIP_V,
	-- -- 	XO => TRANSFORM_XO,
	-- -- 	YO => TRANSFORM_YO);
	-- -- TRANS_TILE_PIDX_X <= to_integer(unsigned(TRANSFORM_XO));
	-- -- TRANS_TILE_PIDX_Y <= to_integer(unsigned(TRANSFORM_YO));

	-- -- TRANS_TILE_PIXEL_IDX <= integer(PPU_SPRITE_WIDTH) * TRANS_TILE_PIDX_Y + TRANS_TILE_PIDX_X;
	-- -- TILEMAP_WORD_OFFSET <= TRANS_TILE_PIXEL_IDX / PPU_PIXELS_PER_TILE_WORD;
	-- -- PIXEL_BIT_OFFSET <= TRANS_TILE_PIXEL_IDX mod PPU_PIXELS_PER_TILE_WORD;

	-- -- T_TMM_ADDR <= std_logic_vector(to_unsigned(PPU_SPRITE_PIXELS_PER_WORD * to_integer(unsigned(FAM_DATA_TILE_IDX)) + TILEMAP_WORD_OFFSET, PPU_TMM_ADDR_WIDTH));

	-- -- -- TMM DATA
	-- -- with PIXEL_BIT_OFFSET select
	-- -- 	TMM_DATA_PAL_IDX <= R_TMM_DATA(2 downto 0) when 0,
	-- -- 	                    R_TMM_DATA(5 downto 3) when 1,
	-- -- 	                    R_TMM_DATA(8 downto 6) when 2,
	-- -- 	                    R_TMM_DATA(11 downto 9) when 3,
	-- -- 	                    R_TMM_DATA(14 downto 12) when 4,
	-- -- 	                    (others => '0') when others;

	-- state machine (pipeline stage counter) + sync r/w
	process(CLK, RESET)
	begin
		if RESET = '1' then
			-- reset state
			state <= PL_TMM_ADDR;
			-- reset internal pipeline registers
			R_TMM_ADDR <= (others => '0');
			R_TMM_DATA <= (others => '0');
		elsif rising_edge(CLK) then
			case state is
				when PL_TMM_ADDR =>
					state <= PL_TMM_DATA;
					R_TMM_ADDR <= T_TMM_ADDR;
				when PL_TMM_DATA =>
					state <= PL_TMM_ADDR;
					R_TMM_DATA <= T_TMM_DATA;
			end case;
		end if;
	end process;
end Behavioral;
