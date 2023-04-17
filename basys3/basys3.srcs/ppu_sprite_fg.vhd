library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;
use work.ppu_consts.all;
use work.ppu_pceg_consts.all;

entity ppu_sprite_fg is -- foreground sprite
	generic (
		IDX : natural := 0); -- sprite index number
	port(
		-- inputs
		CLK : in std_logic; -- system clock
		RESET : in std_logic; -- reset internal memory and clock counters
		PL_STAGE : in ppu_sprite_fg_pl_state; -- pipeline stage
		PL_HIT : in ppu_sprite_fg_hit_pl_state;
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
	component ppu_two_lut port( -- PPU tilemap word offset LUT (divide 8-bit number by 5)
		pidx : in integer range 0 to 255; -- dividend
		two : out integer range 0 to 255); -- result
	end component;

	-- TMM in/out temp + registers
	signal T_TMM_ADDR, R_TMM_ADDR : std_logic_vector(PPU_TMM_ADDR_WIDTH-1 downto 0) := (others => '0');
	signal T_TMM_DATA, R_TMM_DATA : std_logic_vector(PPU_TMM_DATA_WIDTH-1 downto 0) := (others => '0');

	-- auxiliary signals (temp variables)
	signal T_CIDX : std_logic_vector(PPU_PALETTE_CIDX_WIDTH-1 downto 0) := (others => '0'); -- output color buffer/register
	signal INT_FAM : std_logic_vector(31 downto 0);

	-- FAM aliases (docs/architecture.md#foreground-attribute-memory)
	alias FAM_REG_FLIP_H is INT_FAM(31); -- Flip horizontally
	alias FAM_REG_FLIP_V is INT_FAM(30); -- Flip vertically
	alias FAM_REG_POS_H is INT_FAM(29 downto 21); -- horizontal position (offset by -16)
	alias FAM_REG_POS_V is INT_FAM(20 downto 13); -- vertical position (offset by -16)
	alias FAM_REG_PAL_IDX is INT_FAM(12 downto 10); -- Palette index for tile
	alias FAM_REG_TILE_IDX is INT_FAM(9 downto 0); -- Tilemap index

	signal SPRITE_ACTIVE : std_logic := '0'; -- is pixel in bounding box of sprite
	signal PIXEL_ABS_X, PIXEL_ABS_Y : integer := 0; -- absolute pixel position (relative to FG canvas instead of viewport)
	signal PIXEL_BIT_OFFSET : integer := 0; -- pixel index within word of TMM
	signal TILE_PIDX_X, TRANS_TILE_PIDX_X : unsigned(PPU_SPRITE_POS_H_WIDTH-1 downto 0) := (others => '0'); -- xy position of pixel within tile (local tile coords)
	signal TILE_PIDX_Y, TRANS_TILE_PIDX_Y : unsigned(PPU_SPRITE_POS_V_WIDTH-1 downto 0) := (others => '0'); -- xy position of pixel within tile (local tile coords)
	signal TRANS_TILE_PIDX : integer range 0 to 255 := 0; -- index of pixel within tile (reading order)
	signal TILEMAP_WORD : unsigned(PPU_TMM_ADDR_WIDTH-1 downto 0) := (others => '0');
	signal TILEMAP_WORD_OFFSET : integer range 0 to 255 := 0; -- word offset from tile start address in TMM
	signal TMM_DATA_COL_IDX : std_logic_vector(PPU_PALETTE_COLOR_WIDTH-1 downto 0); -- color of palette
	
	signal TMM_ADDR_EN : boolean := false;
begin
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

	-- CIDX combination
	T_CIDX <= FAM_REG_PAL_IDX & TMM_DATA_COL_IDX;
	-- output drivers
	CIDX <= T_CIDX when OE = '1' else (others => 'Z');
	-- TMM memory
	T_TMM_DATA <= TMM_DATA;

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
	TRANS_TILE_PIDX <= integer(PPU_SPRITE_WIDTH) * to_integer(TRANS_TILE_PIDX_Y) + to_integer(TRANS_TILE_PIDX_X);
	TILEMAP_WORD <= resize(unsigned(FAM_REG_TILE_IDX) * PPU_SPRITE_WORD_COUNT, TILEMAP_WORD'length); -- TMM sprite starting word
	div: component ppu_two_lut port map(pidx => TRANS_TILE_PIDX, two => TILEMAP_WORD_OFFSET);
	-- TILEMAP_WORD_OFFSET <= TRANS_TILE_PIDX / PPU_PIXELS_PER_TILE_WORD; -- word offset from starting word of sprite
	PIXEL_BIT_OFFSET <= TRANS_TILE_PIDX mod PPU_PIXELS_PER_TILE_WORD; -- pixel bit offset

	inaccurate_occlusion_shims: if IDX >= PPU_ACCURATE_FG_SPRITE_COUNT generate
		-- state machine for synchronizing pipeline stages
	begin
		HIT <= (SPRITE_ACTIVE) when PL_HIT = PL_HIT_INACCURATE else
					 (SPRITE_ACTIVE and (or TMM_DATA_COL_IDX)) when PL_HIT = PL_HIT_ACCURATE else '0';
		-- only fetch if OE is high, and during the second pipeline stage
		TMM_ADDR <= R_TMM_ADDR when OE = '1' and TMM_ADDR_EN else (others => 'Z');
		T_TMM_ADDR <= std_logic_vector(TILEMAP_WORD + to_unsigned(TILEMAP_WORD_OFFSET, PPU_TMM_ADDR_WIDTH)); -- TMM address

		-- TMM DATA
		with PIXEL_BIT_OFFSET select
			TMM_DATA_COL_IDX <= R_TMM_DATA(2 downto 0) when 0,
													R_TMM_DATA(5 downto 3) when 1,
													R_TMM_DATA(8 downto 6) when 2,
													R_TMM_DATA(11 downto 9) when 3,
													R_TMM_DATA(14 downto 12) when 4,
													(others => '0') when others;

		-- rising edge process (read/write)
		process(CLK, RESET)
		begin
			if RESET = '1' then
				R_TMM_DATA <= (others => '0');
			elsif OE = '0' then
				null; -- don't read/write if current sprite is not the top sprite
			elsif falling_edge(CLK) then
				case PL_STAGE is
					when PL_FG_TMM_DATA =>
						R_TMM_DATA <= T_TMM_DATA;
					when others => null;
				end case;
			end if;
		end process;
	end generate;
	-- falling edge process (TMM ADDR master control)
	process(CLK, RESET)
	begin
		if RESET = '1' then
			TMM_ADDR_EN <= false;

			R_TMM_ADDR <= (others => '0');
		elsif rising_edge(CLK) then
			TMM_ADDR_EN <= true when PL_STAGE = PL_FG_TMM_ADDR else false;

			R_TMM_ADDR <= T_TMM_ADDR;
		end if;
	end process;

	accurate_occlusion_logic: if IDX < PPU_ACCURATE_FG_SPRITE_COUNT generate
		-- TMM cache lines
		signal TMM_CACHE_WEN, TMM_CACHE_UPDATE_TURN : std_logic := '0';
		signal TMM_CACHE_DATA : std_logic_vector(PPU_TMM_DATA_WIDTH-1 downto 0) := (others => '0');
		signal TMM_CACHE_ADDR : std_logic_vector(PPU_TMM_ADDR_WIDTH-1 downto 0) := (others => '0');
		signal TMM_CACHE : std_logic_vector((PPU_SPRITE_WORD_COUNT * PPU_TMM_DATA_WIDTH)-1 downto 0);
	begin
		HIT <= SPRITE_ACTIVE and (or TMM_DATA_COL_IDX);

		-- palette color at pixel
		TMM_DATA_COL_IDX <= TMM_CACHE(TRANS_TILE_PIDX * integer(PPU_PALETTE_COLOR_WIDTH) + integer(PPU_PALETTE_COLOR_WIDTH)-1 downto TRANS_TILE_PIDX * integer(PPU_PALETTE_COLOR_WIDTH));

		TMM_ADDR <= T_TMM_ADDR when TMM_CACHE_UPDATE_TURN else (others => 'Z');

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

		-- fetch machine, should do the following (offset data read by one clock -> propagation/lookup delay):
		-- CLK[53 * IDX + 0] (addr = 0)
		-- CLK[53 * IDX + 1] (addr = 1, read data[0])
		-- CLK[53 * IDX + 2] (addr = 2, read data[1]), etc
		-- a full tile is 52 words, but since the offset is 1 clock, a total copy takes 53 clock cycles
		process(CLK, RESET, FETCH)
			constant TMM_FETCH_CLK_RANGE_BEGIN : natural := PPU_TMM_CACHE_FETCH_C_COUNT * IDX; -- fetch CLK count for copying this module's sprite from TMM
			variable TMM_FETCH_CTR : unsigned(PPU_TMM_CACHE_FETCH_A_WIDTH-1 downto 0) := (others => '0'); -- CLK counter while FETCH=1
			variable TMM_FETCH_CTR_REL : unsigned(PPU_TMM_CACHE_FETCH_A_WIDTH-1 downto 0) := (others => '0'); -- CLK counter relative for sprite[IDX]
		begin
			if RESET = '1' or FETCH = '0' then
				TMM_FETCH_CTR := (others => '0');
				TMM_FETCH_CTR_REL := (others => '0');
				TMM_CACHE_WEN <= '0';
				TMM_CACHE_UPDATE_TURN <= '0';
			elsif rising_edge(CLK) then
				TMM_FETCH_CTR := TMM_FETCH_CTR + 1;
				TMM_FETCH_CTR_REL := TMM_FETCH_CTR - TMM_FETCH_CLK_RANGE_BEGIN;

				if TMM_FETCH_CTR >= TMM_FETCH_CLK_RANGE_BEGIN and
					 TMM_FETCH_CTR < (TMM_FETCH_CLK_RANGE_BEGIN + PPU_TMM_CACHE_FETCH_C_COUNT) then
					TMM_CACHE_UPDATE_TURN <= '1';
					if TMM_FETCH_CTR_REL < PPU_TMM_CACHE_FETCH_C_COUNT - 1 then -- calculate address until second to last clock
						T_TMM_ADDR <= std_logic_vector(resize(TMM_FETCH_CTR - IDX, T_TMM_ADDR'length)); -- -IDX to correct for each fetch cycle taking 1 extra clock cycle
						TMM_CACHE_ADDR <= std_logic_vector(resize(TMM_FETCH_CTR_REL - 1, TMM_CACHE_ADDR'length));
					end if;

					if TMM_FETCH_CTR_REL > 0 then -- read offset
						TMM_CACHE_DATA <= T_TMM_DATA;
						TMM_CACHE_WEN <= '1';
					end if;
				else
					TMM_CACHE_WEN <= '0';
					TMM_CACHE_UPDATE_TURN <= '0';
				end if;
			end if;
		end process;
	end generate;
end Behavioral;
