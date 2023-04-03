library ieee;
library work;

use ieee.std_logic_1164.all;
use work.ppu_consts.all;
use work.ppu_pceg_consts.all;

entity ppu is port(
	CLK100 : in std_logic; -- system clock
	RESET : in std_logic; -- global (async) system reset
	WEN : in std_logic; -- PPU VRAM write enable
	ADDR : in std_logic_vector(PPU_RAM_BUS_ADDR_WIDTH-1 downto 0); -- PPU VRAM ADDR
	DATA : in std_logic_vector(PPU_RAM_BUS_DATA_WIDTH-1 downto 0);
	R,G,B : out std_logic_vector(PPU_COLOR_OUTPUT_DEPTH-1 downto 0);
	VSYNC, HSYNC : out std_logic; -- VGA sync out
	VBLANK, HBLANK : out std_logic; -- vblank and hblank for synchronization
	RESOUT : out std_logic); -- reset out
end ppu;

architecture Behavioral of ppu is
	component ppu_pceg port(
		CLK : in std_logic; -- system clock
		RESET : in std_logic; -- async reset
		SPRITE_BG : out ppu_sprite_bg_pl_state := PL_BG_IDLE; -- sprite info fetch + sprite pixel fetch
		SPRITE_FG : out ppu_sprite_fg_pl_state := PL_FG_IDLE; -- sprite pixel fetch
		DONE : out std_logic; -- last pipeline stage done
		READY : out std_logic); -- rgb buffer propagation ready
	end component;
	component ppu_addr_dec port( -- address decoder
		WEN : in std_logic; -- EXT write enable
		TMM_WEN,
		BAM_WEN,
		FAM_WEN,
		PAL_WEN,
		AUX_WEN : out std_logic; -- write enable MUX
		ADDR : in std_logic_vector(PPU_RAM_BUS_ADDR_WIDTH-1 downto 0); -- address in
		TMM_ADDR : out std_logic_vector(PPU_TMM_ADDR_WIDTH-1 downto 0);
		BAM_ADDR : out std_logic_vector(PPU_BAM_ADDR_WIDTH-1 downto 0);
		FAM_ADDR : out std_logic_vector(PPU_FAM_ADDR_WIDTH-1 downto 0);
		PAL_ADDR : out std_logic_vector(PPU_PAL_ADDR_WIDTH-1 downto 0);
		AUX_ADDR : out std_logic_vector(PPU_AUX_ADDR_WIDTH-1 downto 0));
	end component;
	component ppu_bam port( -- BAM block memory
		clka : in std_logic;
		wea : in std_logic_vector(0 to 0);
		addra : in std_logic_vector(PPU_BAM_ADDR_WIDTH-1 downto 0);
		dina : in std_logic_vector(PPU_BAM_DATA_WIDTH-1 downto 0);
		clkb : in std_logic;
		rstb : in std_logic;
		addrb : in std_logic_vector(PPU_BAM_ADDR_WIDTH-1 downto 0);
		doutb : out std_logic_vector(PPU_BAM_DATA_WIDTH-1 downto 0);
		rsta_busy : out std_logic;
		rstb_busy : out std_logic);
	end component;
	component ppu_tmm port( -- TMM block memory
		clka : in std_logic;
		wea : in std_logic_vector(0 to 0);
		addra : in std_logic_vector(PPU_TMM_ADDR_WIDTH-1 downto 0);
		dina : in std_logic_vector(PPU_TMM_DATA_WIDTH-1 downto 0);
		clkb : in std_logic;
		rstb : in std_logic;
		addrb : in std_logic_vector(PPU_TMM_ADDR_WIDTH-1 downto 0);
		doutb : out std_logic_vector(PPU_TMM_DATA_WIDTH-1 downto 0);
		rsta_busy : out std_logic;
		rstb_busy : out std_logic);
	end component;
	component ppu_aux port(
		CLK : in std_logic; -- system clock
		RESET : in std_logic; -- reset memory

		-- internal memory block (AUX)
		AUX_WEN : in std_logic; -- VRAM AUX write enable
		AUX_ADDR : in std_logic_vector(PPU_AUX_ADDR_WIDTH-1 downto 0); -- VRAM AUX address
		AUX_DATA : in std_logic_vector(PPU_AUX_DATA_WIDTH-1 downto 0); -- VRAM AUX data
		
		-- aux outputs
		BG_SHIFT_X : out std_logic_vector(PPU_POS_H_WIDTH-1 downto 0);
		BG_SHIFT_Y : out std_logic_vector(PPU_POS_V_WIDTH-1 downto 0);
		FG_FETCH : out std_logic;
		RESOUT : out std_logic);
	end component;
	component ppu_sprite_bg port( -- background sprite
		-- inputs
		CLK : in std_logic; -- system clock
		RESET : in std_logic; -- reset clock counter
		PL_STAGE : in ppu_sprite_bg_pl_state; -- pipeline stage
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
	end component;
	component ppu_sprite_fg -- foreground sprite
		generic (
			IDX : natural := 0);
		port(
			-- inputs
			CLK : in std_logic; -- system clock
			RESET : in std_logic; -- reset internal memory and clock counters
			PL_STAGE : in ppu_sprite_fg_pl_state; -- pipeline stage
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
	end component;
	component ppu_comp port( -- compositor
		OE : in std_logic; -- global output enable (screen active)
		FG_HIT : in std_logic_vector(PPU_FG_SPRITE_COUNT-1 downto 0); -- foreground hit array
		BG_EN : out std_logic; -- background enable output
		FG_EN : out std_logic_vector(PPU_FG_SPRITE_COUNT-1 downto 0)); -- foreground enable output
	end component;
	component ppu_plut port( -- palette lookup table
		CLK : in std_logic; -- system clock
		CIDX : in std_logic_vector(PPU_PALETTE_CIDX_WIDTH-1 downto 0); -- color in
		RESET : in std_logic;

		-- internal memory block (AUX)
		PAL_WEN : in std_logic; -- VRAM PAL write enable
		PAL_ADDR : in std_logic_vector(PPU_PAL_ADDR_WIDTH-1 downto 0); -- VRAM PAL address
		PAL_DATA : in std_logic_vector(PPU_PAL_DATA_WIDTH-1 downto 0); -- VRAM PAL data
		
		R,G,B : out std_logic_vector(PPU_COLOR_OUTPUT_DEPTH-1 downto 0)); -- VGA color out
	end component;
	component ppu_dispctl port(
		SYSCLK : in std_logic; -- system clock
		RESET : in std_logic;

		X : out std_logic_vector(PPU_POS_H_WIDTH-1 downto 0); -- tiny screen pixel x
		Y : out std_logic_vector(PPU_POS_V_WIDTH-1 downto 0); -- tiny screen pixel y
		RI,GI,BI : in std_logic_vector(PPU_COLOR_OUTPUT_DEPTH-1 downto 0); -- color in
		PREADY : in std_logic; -- current pixel ready (pixel color is stable)

		RO,GO,BO : out std_logic_vector(PPU_COLOR_OUTPUT_DEPTH-1 downto 0); -- VGA color out
		NVSYNC, NHSYNC : out std_logic; -- VGA sync out
		THBLANK, TVBLANK : out std_logic; -- tiny sync signals
		ACTIVE : out std_logic); -- screen currently active (currently same for tiny/native, TODO: offset tiny for first scanline)
	end component;

	-- signals
	signal SYSCLK, SYSRST : std_logic; -- system clock and reset
	signal PL_DONE, PL_READY : std_logic; -- pipeline stages
	signal PL_SPRITE_BG : ppu_sprite_bg_pl_state;
	signal PL_SPRITE_FG : ppu_sprite_fg_pl_state;
	signal TMM_WEN, BAM_WEN, FAM_WEN, PAL_WEN, AUX_WEN : std_logic;
	signal TMM_W_ADDR, TMM_R_ADDR : std_logic_vector(PPU_TMM_ADDR_WIDTH-1 downto 0); -- read/write TMM addr (dual port)
	signal BAM_W_ADDR, BAM_R_ADDR : std_logic_vector(PPU_BAM_ADDR_WIDTH-1 downto 0); -- read/write BAM addr (dual port)
	signal TMM_R_DATA : std_logic_vector(PPU_TMM_DATA_WIDTH-1 downto 0); -- internal read TMM data
	signal BAM_R_DATA : std_logic_vector(PPU_BAM_DATA_WIDTH-1 downto 0); -- internal read BAM data
	signal FAM_W_ADDR : std_logic_vector(PPU_FAM_ADDR_WIDTH-1 downto 0); -- write only FAM addr
	signal PAL_W_ADDR : std_logic_vector(PPU_PAL_ADDR_WIDTH-1 downto 0); -- write only PAL addr
	signal AUX_W_ADDR : std_logic_vector(PPU_AUX_ADDR_WIDTH-1 downto 0); -- write only AUX addr
	signal CIDX : std_logic_vector(PPU_PALETTE_CIDX_WIDTH-1 downto 0);
	signal BG_EN : std_logic;
	signal FG_EN, FG_HIT : std_logic_vector(PPU_FG_SPRITE_COUNT-1 downto 0);
	signal X : std_logic_vector(PPU_POS_H_WIDTH-1 downto 0); -- current screen pixel x
	signal Y : std_logic_vector(PPU_POS_V_WIDTH-1 downto 0); -- current screen pixel y
	signal UR,UG,UB : std_logic_vector(PPU_COLOR_OUTPUT_DEPTH-1 downto 0); -- palette lookup output RGB
	signal BG_SHIFT_X : std_logic_vector(PPU_POS_H_WIDTH-1 downto 0);
	signal BG_SHIFT_Y : std_logic_vector(PPU_POS_V_WIDTH-1 downto 0);
	signal NVSYNC, NHSYNC, THBLANK, TVBLANK : std_logic;
	signal ACTIVE : std_logic;
	signal PCEG_RESET : std_logic;
	signal FG_FETCH, FG_FETCH_S, FG_FETCH_R : std_logic := '0';
begin
	SYSCLK <= CLK100;
	SYSRST <= RESET;

	VSYNC <= NVSYNC;
	HSYNC <= NHSYNC;

	PCEG_RESET <= SYSRST or (not ACTIVE);
	VBLANK <= TVBLANK;
	HBLANK <= THBLANK;

	pipeline_clock_edge_generator : component ppu_pceg port map(
		CLK => SYSCLK,
		RESET => PCEG_RESET,
		SPRITE_FG => PL_SPRITE_FG,
		SPRITE_BG => PL_SPRITE_BG,
		DONE => PL_DONE,
		READY => PL_READY);

	address_decoder : component ppu_addr_dec port map(
		WEN => WEN,
		ADDR => ADDR,
		TMM_ADDR => TMM_W_ADDR,
		BAM_ADDR => BAM_W_ADDR,
		FAM_ADDR => FAM_W_ADDR,
		PAL_ADDR => PAL_W_ADDR,
		AUX_ADDR => AUX_W_ADDR,
		TMM_WEN => TMM_WEN,
		BAM_WEN => BAM_WEN,
		FAM_WEN => FAM_WEN,
		PAL_WEN => PAL_WEN,
		AUX_WEN => AUX_WEN);

	background_attribute_memory : component ppu_bam port map(
		clka => SYSCLK,
		wea => (others => BAM_WEN),
		addra => BAM_W_ADDR,
		dina => DATA(PPU_BAM_DATA_WIDTH-1 downto 0),
		clkb => SYSCLK,
		rstb => SYSRST,
		addrb => BAM_R_ADDR,
		doutb => BAM_R_DATA,
		rsta_busy => open,
		rstb_busy => open);
	tilemap_memory : component ppu_tmm port map(
		clka => SYSCLK,
		wea => (others => TMM_WEN),
		addra => TMM_W_ADDR,
		dina => DATA(PPU_TMM_DATA_WIDTH-1 downto 0),
		clkb => SYSCLK,
		rstb => SYSRST,
		addrb => TMM_R_ADDR,
		doutb => TMM_R_DATA,
		rsta_busy => open,
		rstb_busy => open);

	aux : component ppu_aux port map(
		CLK => SYSCLK,
		RESET => SYSRST,
		AUX_WEN => AUX_WEN,
		AUX_ADDR => AUX_W_ADDR,
		AUX_DATA => DATA(PPU_AUX_DATA_WIDTH-1 downto 0),
		BG_SHIFT_X => BG_SHIFT_X,
		BG_SHIFT_Y => BG_SHIFT_Y,
		FG_FETCH => FG_FETCH_S,
		RESOUT => RESOUT);

	-- set/reset TODO: reset on scanline 1 (buffer VBLANK to check edge)
	process(SYSCLK)
	begin
		if SYSRST = '1' then
			FG_FETCH <= '0';
		elsif rising_edge(SYSCLK) then
			if FG_FETCH_S = '1' then
				FG_FETCH <= '1';
			elsif FG_FETCH_R = '1' then
				FG_FETCH <= '0';
			end if;
		end if;
	end process;

	background_sprite : component ppu_sprite_bg port map(
		CLK => SYSCLK,
		RESET => SYSRST,
		PL_STAGE => PL_SPRITE_BG,
		OE => BG_EN,
		X => X,
		Y => Y,
		BG_SHIFT_X => BG_SHIFT_X,
		BG_SHIFT_Y => BG_SHIFT_Y,
		BAM_ADDR => BAM_R_ADDR,
		BAM_DATA => BAM_R_DATA,
		TMM_ADDR => TMM_R_ADDR,
		TMM_DATA => TMM_R_DATA,
		CIDX => CIDX);

	foreground_sprites : for FG_IDX in 0 to PPU_FG_SPRITE_COUNT-1 generate
		foreground_sprite : component ppu_sprite_fg
			generic map( IDX => FG_IDX )
			port map(
				CLK => SYSCLK,
				RESET => SYSRST,
				PL_STAGE => PL_SPRITE_FG,
				OE => FG_EN(FG_IDX),
				X => X,
				Y => Y,
				FETCH => FG_FETCH,
				VBLANK => TVBLANK,
				FAM_WEN => FAM_WEN,
				FAM_ADDR => FAM_W_ADDR,
				FAM_DATA => DATA(PPU_FAM_DATA_WIDTH-1 downto 0),
				TMM_ADDR => TMM_R_ADDR,
				TMM_DATA => TMM_R_DATA,
				CIDX => CIDX,
				HIT => FG_HIT(FG_IDX));
	end generate;

	compositor : component ppu_comp port map( -- compositor
		OE => ACTIVE,
		FG_HIT => FG_HIT,
		BG_EN => BG_EN,
		FG_EN => FG_EN);

	palette_lookup : component ppu_plut port map( -- palette lookup table
		CLK => SYSCLK,
		CIDX => CIDX,
		RESET => SYSRST,
		PAL_WEN => PAL_WEN,
		PAL_ADDR => PAL_W_ADDR,
		PAL_DATA => DATA(PPU_PAL_DATA_WIDTH-1 downto 0),
		R => UR,
		G => UG,
		B => UB);

	display_controller : component ppu_dispctl port map(
		SYSCLK => SYSCLK,
		RESET => SYSRST,
		PREADY => PL_READY,
		X => X,
		Y => Y,
		RI => UR,
		GI => UG,
		BI => UB,
		RO => R,
		GO => G,
		BO => B,
		NVSYNC => NVSYNC,
		NHSYNC => NHSYNC,
		TVBLANK => TVBLANK,
		THBLANK => THBLANK,
		ACTIVE => ACTIVE);
end Behavioral;
