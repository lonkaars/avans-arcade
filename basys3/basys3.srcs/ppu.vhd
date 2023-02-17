library ieee;
use ieee.std_logic_1164.all;
--use ieee.numeric_std.all;

entity ppu is
	port(
		CLK100: in std_logic; -- system clock
		RESET: in std_logic; -- global (async) system reset
		EN: in std_logic; -- PPU VRAM enable (enable ADDR and DATA tri-state drivers)
		WEN: in std_logic; -- PPU VRAM write enable
		ADDR: in std_logic_vector(15 downto 0); -- PPU VRAM ADDR
		DATA: in std_logic_vector(15 downto 0);
		R,G,B: out std_logic_vector(3 downto 0);
		NVSYNC, NHSYNC: out std_logic; -- native VGA out
    TVSYNC, TVBLANK, THSYNC, THBLANK: out std_logic); -- tiny VGA out
end ppu;

architecture Behavioral of ppu is
	constant PPU_FG_SPRITE_COUNT: natural := 1; -- amount of foreground sprites
	constant PPU_COLOR_OUTPUT_DEPTH: natural := 4; -- VGA output channel depth
  constant PPU_PALETTE_IDX_WIDTH: natural := 3; -- palette index width (within sprite)
  constant PPU_PALETTE_WIDTH: natural := 3; -- palette index width (palette table)
  constant PPU_PALETTE_CIDX_WIDTH: natural := PPU_PALETTE_IDX_WIDTH + PPU_PALETTE_WIDTH; -- global palette index width
	constant PPU_PIPELINE_STAGE_COUNT: natural := 5; -- amount of pipeline clock edges to generate
	constant PPU_TMM_ADDR_WIDTH: natural := 16;
	constant PPU_TMM_DATA_WIDTH: natural := 16;
	constant PPU_BAM_ADDR_WIDTH: natural := 11;
	constant PPU_BAM_DATA_WIDTH: natural := 15;
	constant PPU_FAM_ADDR_WIDTH: natural := 8;
	constant PPU_FAM_DATA_WIDTH: natural := 16;
	constant PPU_PAL_ADDR_WIDTH: natural := 6;
	constant PPU_PAL_DATA_WIDTH: natural := 12;
	constant PPU_AUX_ADDR_WIDTH: natural := 2;
	constant PPU_AUX_DATA_WIDTH: natural := 16;
	constant PPU_POS_H_WIDTH: natural := 9; -- amount of bits for horizontal screen offset
	constant PPU_POS_V_WIDTH: natural := 8; -- amount of bits for vertical screen offset

	component ppu_pceg port( -- pipeline clock edge generator
		CLK: in std_logic; -- system clock
		R: in std_logic; -- async reset
		S: out std_logic_vector(PPU_PIPELINE_STAGE_COUNT-1 downto 0)); -- pipeline stages
	end component;
	component ppu_addr_dec port( -- pipeline clock edge generator
		WEN: in std_logic; -- EXT write enable
    TMM_WEN,
    BAM_WEN,
    FAM_WEN,
    PAL_WEN,
    AUX_WEN: out std_logic; -- write enable MUX
    TMM_DI: out std_logic_vector(PPU_TMM_DATA_WIDTH-1 downto 0); -- TMM write DATA MUX
    BAM_DI: out std_logic_vector(PPU_BAM_DATA_WIDTH-1 downto 0); -- BAM write DATA MUX
    FAM_DI: out std_logic_vector(PPU_FAM_DATA_WIDTH-1 downto 0); -- FAM write DATA MUX
    PAL_DI: out std_logic_vector(PPU_PAL_DATA_WIDTH-1 downto 0); -- PAL write DATA MUX
    AUX_DI: out std_logic_vector(PPU_AUX_DATA_WIDTH-1 downto 0); -- AUX write DATA MUX

		EN: in std_logic; -- EXT *ADDR enable (switch *AO to ADDR instead of *AI)
		ADDR: in std_logic_vector(15 downto 0); -- address in
    TMM_AI: in std_logic_vector(PPU_TMM_ADDR_WIDTH-1 downto 0);
    BAM_AI: in std_logic_vector(PPU_BAM_ADDR_WIDTH-1 downto 0);
    FAM_AI: in std_logic_vector(PPU_FAM_ADDR_WIDTH-1 downto 0);
    PAL_AI: in std_logic_vector(PPU_PAL_ADDR_WIDTH-1 downto 0);
    AUX_AI: in std_logic_vector(PPU_AUX_ADDR_WIDTH-1 downto 0);
    TMM_AO: out std_logic_vector(PPU_TMM_ADDR_WIDTH-1 downto 0);
    BAM_AO: out std_logic_vector(PPU_BAM_ADDR_WIDTH-1 downto 0);
    FAM_AO: out std_logic_vector(PPU_FAM_ADDR_WIDTH-1 downto 0);
    PAL_AO: out std_logic_vector(PPU_PAL_ADDR_WIDTH-1 downto 0);
    AUX_AO: out std_logic_vector(PPU_AUX_ADDR_WIDTH-1 downto 0));
	end component;
  component ppu_bam port( -- BAM block memory
		clka: in std_logic;
		rsta: in std_logic;
		wea: in std_logic_vector(0 downto 0);
		addra: in std_logic_vector(PPU_BAM_ADDR_WIDTH-1 downto 0);
		dina: in std_logic_vector(PPU_BAM_DATA_WIDTH-1 downto 0);
		douta: out std_logic_vector(PPU_BAM_DATA_WIDTH-1 downto 0);
		rsta_busy: out std_logic);
	end component;
  component ppu_tmm port( -- TMM block memory
		clka: in std_logic;
		rsta: in std_logic;
		wea: in std_logic_vector(0 downto 0);
		addra: in std_logic_vector(PPU_TMM_ADDR_WIDTH-1 downto 0);
		dina: in std_logic_vector(PPU_TMM_DATA_WIDTH-1 downto 0);
		douta: out std_logic_vector(PPU_TMM_DATA_WIDTH-1 downto 0);
		rsta_busy: out std_logic);
	end component;
	component ppu_sprite_bg port( -- background sprite
		-- inputs
		CLK: in std_logic; -- system clock
		OE: in std_logic; -- output enable (of CIDX)
		X: in std_logic_vector(PPU_POS_H_WIDTH-1 downto 0); -- current screen pixel x
		Y: in std_logic_vector(PPU_POS_V_WIDTH-1 downto 0); -- current screen pixel y

		-- internal memory block (AUX)
		AUX_WEN: in std_logic; -- VRAM AUX write enable
		AUX_ADDR: in std_logic_vector(PPU_AUX_ADDR_WIDTH-1 downto 0); -- VRAM AUX address
		AUX_DATA: in std_logic_vector(PPU_AUX_DATA_WIDTH-1 downto 0); -- VRAM AUX data

		-- used memory blocks
		BAM_ADDR: out std_logic_vector(PPU_BAM_ADDR_WIDTH-1 downto 0);
		BAM_DATA: in std_logic_vector(PPU_BAM_DATA_WIDTH-1 downto 0);
		TMM_ADDR: out std_logic_vector(PPU_TMM_ADDR_WIDTH-1 downto 0);
		TMM_DATA: in std_logic_vector(PPU_TMM_DATA_WIDTH-1 downto 0);

		-- outputs
		CIDX: out std_logic_vector(PPU_PALETTE_CIDX_WIDTH-1 downto 0)); -- output color
	end component;
  component ppu_sprite_fg port( -- foreground sprite
		-- inputs
		CLK: in std_logic; -- system clock
		OE: in std_logic; -- output enable (of CIDX)
		X: in std_logic_vector(PPU_POS_H_WIDTH-1 downto 0); -- current screen pixel x
		Y: in std_logic_vector(PPU_POS_V_WIDTH-1 downto 0); -- current screen pixel y
		FETCH: in std_logic; -- fetch sprite data from TMM (TODO: generic map, set foreground sprite component index)

		-- internal memory block (FAM)
		FAM_WEN: in std_logic; -- VRAM FAM write enable
		FAM_ADDR: in std_logic_vector(PPU_FAM_ADDR_WIDTH-1 downto 0); -- VRAM fam address
		FAM_DATA: in std_logic_vector(PPU_FAM_DATA_WIDTH-1 downto 0); -- VRAM fam data

		-- used memory blocks
		TMM_ADDR: out std_logic_vector(PPU_TMM_ADDR_WIDTH-1 downto 0);
		TMM_DATA: in std_logic_vector(PPU_TMM_DATA_WIDTH-1 downto 0);

		-- outputs
		CIDX: out std_logic_vector(PPU_PALETTE_CIDX_WIDTH-1 downto 0); -- output color
		HIT: out std_logic); -- current pixel is not transparent
	end component;
	component ppu_comp port( -- compositor
		FG_HIT: in std_logic_vector(PPU_FG_SPRITE_COUNT-1 downto 0);
		BG_EN: out std_logic;
		FG_EN: out std_logic_vector(PPU_FG_SPRITE_COUNT-1 downto 0));
	end component;
	component ppu_plut port( -- palette lookup table
		CLK: in std_logic; -- system clock
		CIDX: in std_logic_vector(PPU_PALETTE_CIDX_WIDTH-1 downto 0); -- color in
    RESET: in std_logic;

		-- internal memory block (AUX)
		PAL_WEN: in std_logic; -- VRAM PAL write enable
		PAL_ADDR: in std_logic_vector(PPU_PAL_ADDR_WIDTH-1 downto 0); -- VRAM PAL address
		PAL_DATA: in std_logic_vector(PPU_PAL_DATA_WIDTH-1 downto 0); -- VRAM PAL data
    
    R,G,B: out std_logic_vector(3 downto 0)); -- VGA color out
	end component;
	component ppu_vga_tiny port( -- tiny vga signal generator
		CLK: in std_logic; -- system clock
    RESET: in std_logic;

		X: out std_logic_vector(PPU_POS_H_WIDTH-1 downto 0); -- current screen pixel x
		Y: out std_logic_vector(PPU_POS_V_WIDTH-1 downto 0); -- current screen pixel y
    PREADY: out std_logic; -- current pixel ready (pixel color is stable)
    
    VSYNC, VBLANK,
    HSYNC, HBLANK: out std_logic); -- VGA sync outputs
	end component;
	component ppu_vga_native port( -- native vga signal generator (upscaler)
		CLK: in std_logic; -- system clock
    RESET: in std_logic;

		X: in std_logic_vector(PPU_POS_H_WIDTH-1 downto 0); -- current screen pixel x
		Y: in std_logic_vector(PPU_POS_V_WIDTH-1 downto 0); -- current screen pixel y
    PREADY: in std_logic; -- current pixel ready (pixel color is stable)
    RI,GI,BI: in std_logic_vector(PPU_COLOR_OUTPUT_DEPTH-1 downto 0); -- VGA color in
    
    RO,GO,BO: out std_logic_vector(PPU_COLOR_OUTPUT_DEPTH-1 downto 0); -- VGA color out
    VSYNC, HSYNC: out std_logic); -- VGA sync outputs
	end component;

	-- signals
	signal SYSCLK, SYSRST: std_logic; -- system clock and reset
	signal PL_S: std_logic_vector(PPU_PIPELINE_STAGE_COUNT-1 downto 0); -- pipeline stages
  signal TMM_WEN, BAM_WEN, FAM_WEN, PAL_WEN, AUX_WEN: std_logic;
	signal TMM_AI, TMM_AO: std_logic_vector(PPU_TMM_ADDR_WIDTH-1 downto 0);
	signal BAM_AI, BAM_AO: std_logic_vector(PPU_BAM_ADDR_WIDTH-1 downto 0);
	signal FAM_AI, FAM_AO: std_logic_vector(PPU_FAM_ADDR_WIDTH-1 downto 0);
	signal PAL_AI, PAL_AO: std_logic_vector(PPU_PAL_ADDR_WIDTH-1 downto 0);
	signal AUX_AI, AUX_AO: std_logic_vector(PPU_AUX_ADDR_WIDTH-1 downto 0);
	signal TMM_DI, TMM_DO: std_logic_vector(PPU_TMM_DATA_WIDTH-1 downto 0);
	signal BAM_DI, BAM_DO: std_logic_vector(PPU_BAM_DATA_WIDTH-1 downto 0);
	signal FAM_DI, FAM_DO: std_logic_vector(PPU_FAM_DATA_WIDTH-1 downto 0);
	signal PAL_DI, PAL_DO: std_logic_vector(PPU_PAL_DATA_WIDTH-1 downto 0);
	signal AUX_DI, AUX_DO: std_logic_vector(PPU_AUX_DATA_WIDTH-1 downto 0);
  signal CIDX: std_logic_vector(PPU_PALETTE_CIDX_WIDTH-1 downto 0);
  signal BG_EN: std_logic;
  signal FG_EN, FG_HIT: std_logic_vector(PPU_FG_SPRITE_COUNT-1 downto 0);
	signal X: std_logic_vector(PPU_POS_H_WIDTH-1 downto 0); -- current screen pixel x
	signal Y: std_logic_vector(PPU_POS_V_WIDTH-1 downto 0); -- current screen pixel y
	signal TR,TG,TB: std_logic_vector(PPU_COLOR_OUTPUT_DEPTH-1 downto 0); -- tiny RGB out (to be buffered)
	signal PREADY: std_logic; -- current pixel color stable
begin
  SYSCLK <= CLK100;
  SYSRST <= RESET;

  pipeline_clock_edge_generator: component ppu_pceg port map(
    CLK => SYSCLK,
    R => SYSRST,
    S => PL_S);

  address_decoder: component ppu_addr_dec port map(
		EN => EN,
		WEN => WEN,
		ADDR => ADDR,
    TMM_DI => TMM_DI,
    BAM_DI => BAM_DI,
    FAM_DI => FAM_DI,
    PAL_DI => PAL_DI,
    AUX_DI => AUX_DI,
    TMM_AI => TMM_AI,
    BAM_AI => BAM_AI,
    FAM_AI => FAM_AI,
    PAL_AI => PAL_AI,
    AUX_AI => AUX_AI,
    TMM_AO => TMM_AO,
    BAM_AO => BAM_AO,
    FAM_AO => FAM_AO,
    PAL_AO => PAL_AO,
    AUX_AO => AUX_AO,
    TMM_WEN => TMM_WEN,
    BAM_WEN => BAM_WEN,
    FAM_WEN => FAM_WEN,
    PAL_WEN => PAL_WEN,
    AUX_WEN => AUX_WEN);

  background_attribute_memory: component ppu_bam port map(
		clka => SYSCLK,
		rsta => SYSRST,
		wea => (others => BAM_WEN),
		addra => BAM_AO,
		dina => BAM_DI,
		douta => BAM_DO,
		rsta_busy => open);
  tilemap_memory: component ppu_tmm port map(
		clka => SYSCLK,
		rsta => SYSRST,
		wea => (others => TMM_WEN),
		addra => TMM_AO,
		dina => TMM_DI,
		douta => TMM_DO,
		rsta_busy => open);

  background_sprite: component ppu_sprite_bg port map(
    CLK => SYSCLK,
    OE => BG_EN,
    X => X,
    Y => Y,
    AUX_WEN => AUX_WEN,
    AUX_ADDR => AUX_AO,
    AUX_DATA => AUX_DI,
    BAM_ADDR => BAM_AI,
    BAM_DATA => BAM_DO,
    TMM_ADDR => TMM_AI,
    TMM_DATA => TMM_DO,
    CIDX => CIDX);

	foreground_sprites: for FG_IDX in 0 to PPU_FG_SPRITE_COUNT-1 generate
		foreground_sprite: component ppu_sprite_fg port map(
			CLK => SYSCLK,
			OE => FG_EN(FG_IDX),
			X => X,
			Y => Y,
			FETCH => '0',
			FAM_WEN => FAM_WEN,
			FAM_ADDR => FAM_AO,
			FAM_DATA => FAM_DI,
			TMM_ADDR => TMM_AI,
			TMM_DATA => TMM_DO,
			CIDX => CIDX,
			HIT => FG_HIT(FG_IDX));
	end generate;

	compositor: component ppu_comp port map( -- compositor
		FG_HIT => FG_HIT,
		BG_EN => BG_EN,
		FG_EN => FG_EN);

	palette_lookup: component ppu_plut port map( -- palette lookup table
		CLK => SYSCLK,
		CIDX => CIDX,
		RESET => SYSRST,
		PAL_WEN => PAL_WEN,
		PAL_ADDR => PAL_AO,
		PAL_DATA => PAL_DI,
		R => TR,
		G => TG,
		B => TB);

	tiny_vga_signal_generator: component ppu_vga_tiny port map( -- tiny vga signal generator
		CLK => SYSCLK,
		RESET => SYSRST,
		X => X,
		Y => Y,
    PREADY => PREADY,
    VSYNC => TVSYNC,
		VBLANK => TVBLANK,
    HSYNC => THSYNC,
		HBLANK => THBLANK);

	native_vga_signal_generator: component ppu_vga_native port map( -- native vga signal generator (upscaler)
		CLK => SYSCLK,
		RESET => SYSRST,
		X => X,
		Y => Y,
    PREADY => PREADY,
		RI => TR,
		GI => TG,
		BI => TB,
		RO => R,
		GO => G,
		BO => B,
		VSYNC => NVSYNC,
		HSYNC => NHSYNC);
end Behavioral;
