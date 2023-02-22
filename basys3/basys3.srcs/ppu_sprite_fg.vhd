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
		VBLANK : in std_logic; -- fetch during vblank

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
		XI : in unsigned(PPU_SPRITE_POS_H_WIDTH-1 downto 0); -- pixel position relative to tile
		YI : in unsigned(PPU_SPRITE_POS_V_WIDTH-1 downto 0); -- pixel position relative to tile
		FLIP_H, FLIP_V : in std_logic; -- flip sprite
		XO : out unsigned(PPU_SPRITE_POS_H_WIDTH-1 downto 0); -- new pixel position relative to tile
		YO : out unsigned(PPU_SPRITE_POS_V_WIDTH-1 downto 0)); -- new pixel position relative to tile
	end component;
	component er_ram -- exposed register RAM
		generic(
			ADDR_W : natural := 2; -- ADDR line width
			DATA_W : natural := 2; -- DATA line width
			ADDR_LOW : natural := 16#0000#; -- starting address
			ADDR_RANGE : natural := 16#0002#); -- amount of valid addresses after ADDR_LOW
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

	signal SPRITE_ACTIVE : std_logic := '0'; -- is pixel in bounding box of sprite
	signal PIXEL_ABS_X, PIXEL_ABS_Y : integer := 0; -- absolute pixel position (relative to FG canvas instead of viewport)
	signal TILE_PIDX_X, TRANS_TILE_PIDX_X : unsigned(PPU_SPRITE_POS_H_WIDTH-1 downto 0) := (others => '0'); -- xy position of pixel within tile (local tile coords)
	signal TILE_PIDX_Y, TRANS_TILE_PIDX_Y : unsigned(PPU_SPRITE_POS_V_WIDTH-1 downto 0) := (others => '0'); -- xy position of pixel within tile (local tile coords)
	signal TRANS_TILE_PIXEL_IDX : integer := 0; -- index of pixel within tile (reading order)
	signal TILEMAP_WORD_OFFSET : integer := 0; -- word offset from tile start address in TMM
	signal TMM_DATA_PAL_IDX : std_logic_vector(PPU_PALETTE_COLOR_WIDTH-1 downto 0); -- color of palette

	-- TMM cache
	signal TMM_CACHE_WEN : std_logic := '0';
	signal TMM_CACHE_DATA : std_logic_vector(PPU_TMM_DATA_WIDTH-1 downto 0) := (others => '0');
	signal TMM_CACHE_ADDR : std_logic_vector(PPU_TMM_ADDR_WIDTH-1 downto 0) := (others => '0');
	signal TMM_CACHE : std_logic_vector((PPU_SPRITE_WORD_COUNT * PPU_TMM_DATA_WIDTH)-1 downto 0);
begin
	-- output drivers
	CIDX <= T_CIDX when OE = '1' else (others => 'Z');
	TMM_ADDR <= R_TMM_ADDR;
	T_TMM_DATA <= TMM_DATA;
	-- CIDX combination
	T_CIDX <= FAM_REG_COL_IDX & TMM_DATA_PAL_IDX;

	-- FAM memory
	FAM : component er_ram
		generic map(
			ADDR_W => PPU_FAM_ADDR_WIDTH,
			DATA_W => PPU_FAM_DATA_WIDTH,
			ADDR_LOW => IDX*2,
			ADDR_RANGE => 2)
		port map(
			CLK => CLK,
			RST => RESET,
			WEN => FAM_WEN,
			ADDR => FAM_ADDR,
			DATA => FAM_DATA,
			REG => INT_FAM);

	-- pixel position within bounding box of sprite
	SPRITE_ACTIVE <= '1' when ((unsigned(X) + 16) >= unsigned(FAM_REG_POS_H)) and
	                          ((unsigned(X) + 16) < (unsigned(FAM_REG_POS_H) + to_unsigned(PPU_SPRITE_WIDTH, PPU_POS_H_WIDTH))) and
	                          ((unsigned(Y) + 16) >= unsigned(FAM_REG_POS_V)) and
	                          ((unsigned(Y) + 16) < (unsigned(FAM_REG_POS_V) + to_unsigned(PPU_SPRITE_HEIGHT, PPU_POS_V_WIDTH))) else '0';

	-- (sprite local) pixel coords
	TILE_PIDX_X <= resize(unsigned(X) + 16 - resize(unsigned(FAM_REG_POS_H), TILE_PIDX_X'length), TILE_PIDX_X'length);
	TILE_PIDX_Y <= resize(unsigned(Y) + 16 - resize(unsigned(FAM_REG_POS_V), TILE_PIDX_Y'length), TILE_PIDX_Y'length);

	-- transform local coords
	transform: component ppu_sprite_transform port map(
		XI => TILE_PIDX_X,
		YI => TILE_PIDX_Y,
		FLIP_H => FAM_REG_FLIP_H,
		FLIP_V => FAM_REG_FLIP_V,
		XO => TRANS_TILE_PIDX_X,
		YO => TRANS_TILE_PIDX_Y);

	-- pixel index
	TRANS_TILE_PIXEL_IDX <= integer(PPU_SPRITE_WIDTH) * to_integer(TRANS_TILE_PIDX_Y) + to_integer(TRANS_TILE_PIDX_X);
	-- palette color at pixel
	TMM_DATA_PAL_IDX <= TMM_CACHE(TRANS_TILE_PIXEL_IDX * integer(PPU_PALETTE_COLOR_WIDTH) + integer(PPU_PALETTE_COLOR_WIDTH)-1 downto TRANS_TILE_PIXEL_IDX * integer(PPU_PALETTE_COLOR_WIDTH));
	-- if pixel in sprite hitbox and TMM_DATA_PAL_IDX > 0
	HIT <= SPRITE_ACTIVE and (nor TMM_DATA_PAL_IDX);

	-- FETCH LOGIC BELOW

	-- TTM cache
	ttm_cache : component er_ram
		generic map(
			ADDR_W => PPU_TMM_ADDR_WIDTH,
			DATA_W => PPU_TMM_DATA_WIDTH,
			ADDR_LOW => 0,
			ADDR_RANGE => PPU_SPRITE_WORD_COUNT)
		port map(
			CLK => CLK,
			RST => RESET,
			WEN => TMM_CACHE_WEN,
			ADDR => TMM_CACHE_ADDR,
			DATA => TMM_CACHE_DATA,
			REG => TMM_CACHE);

	TILEMAP_WORD_OFFSET <= TRANS_TILE_PIXEL_IDX / PPU_PIXELS_PER_TILE_WORD; -- word offset from starting word of sprite
	T_TMM_ADDR <= std_logic_vector(to_unsigned(PPU_SPRITE_WORD_COUNT * to_integer(unsigned(FAM_REG_TILE_IDX)) + TILEMAP_WORD_OFFSET, PPU_TMM_ADDR_WIDTH)); -- TMM address


	-- state machine (pipeline stage counter) + sync r/w
	process(CLK, RESET)
		constant TMM_FETCH_CLK_RANGE_BEGIN : natural := PPU_TMM_CACHE_FETCH_C_COUNT * IDX;
		variable TMM_FETCH_CTR : unsigned(PPU_TMM_CACHE_FETCH_A_WIDTH-1 downto 0) := (others => '0');
		variable TMM_FETCH_CTR_REL : unsigned(PPU_TMM_CACHE_FETCH_A_WIDTH-1 downto 0) := (others => '0');
	begin
		if RESET = '1' then
			-- reset state
			state <= PL_TMM_ADDR;
			-- reset internal pipeline registers
			R_TMM_ADDR <= (others => '0');
			R_TMM_DATA <= (others => '0');
		elsif rising_edge(CLK) then
			TMM_FETCH_CTR := (others => '0') when FETCH = '0' else TMM_FETCH_CTR + 1;
			TMM_FETCH_CTR_REL := TMM_FETCH_CTR - TMM_FETCH_CLK_RANGE_BEGIN;

			if FETCH = '1' and TMM_FETCH_CTR >= TMM_FETCH_CLK_RANGE_BEGIN and TMM_FETCH_CTR < (TMM_FETCH_CLK_RANGE_BEGIN + PPU_TMM_CACHE_FETCH_C_COUNT) then
				TMM_CACHE_WEN <= '1';
				R_TMM_DATA <= T_TMM_DATA;
				T_TMM_ADDR <= R_TMM_ADDR;
			else
				TMM_CACHE_WEN <= '0';
			end if;
		end if;
	end process;
end Behavioral;
