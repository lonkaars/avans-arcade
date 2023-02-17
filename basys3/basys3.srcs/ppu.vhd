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
  constant PPU_FG_SPRITE_COUNT: natural := 128;
  constant PPU_PALETTE_IDX_SIZE: natural := 3;
  constant PPU_PALETTE_SIZE: natural := 3;
  constant PPU_PALETTE_CIDX_SIZE: natural := PPU_PALETTE_IDX_SIZE + PPU_PALETTE_SIZE;
  constant PPU_PIPELINE_STAGE_COUNT: natural := 5;

	component ppu_pceg port( -- pipeline clock edge generator
		CLK: in std_logic; -- system clock
		R: in std_logic; -- async reset
		S: out std_logic_vector(PPU_PIPELINE_STAGE_COUNT-1 downto 0)); -- pipeline stages
	end component;
	component ppu_addr_dec port( -- pipeline clock edge generator
		EN: in std_logic; -- ADDR enable (tri-state driver)
		WEN: in std_logic; -- write enable
		ADDR: in std_logic_vector(15 downto 0); -- address in

		ADDR_DRV: out std_logic_vector(15 downto 0); -- address out driver
    TMM_WEN,
    BAM_WEN,
    FAM_WEN,
    PAL_WEN,
    AUX_WEN: out std_logic);

	end component;
  component ppu_bam port( -- BAM block memory
		clka: in std_logic;
		rsta: in std_logic;
		wea: in std_logic_vector(0 downto 0);
		addra: in std_logic_vector(10 downto 0);
		dina: in std_logic_vector(14 downto 0);
		douta: out std_logic_vector(14 downto 0);
		rsta_busy: out std_logic);
	end component;
  component ppu_tmm port( -- TMM block memory
		clka: in std_logic;
		rsta: in std_logic;
		wea: in std_logic_vector(0 downto 0);
		addra: in std_logic_vector(15 downto 0);
		dina: in std_logic_vector(15 downto 0);
		douta: out std_logic_vector(15 downto 0);
		rsta_busy: out std_logic);
	end component;
	component ppu_sprite_bg port( -- background sprite
		-- inputs
		CLK: in std_logic; -- system clock
		OE: in std_logic; -- output enable (of CIDX)
		X: in std_logic_vector(8 downto 0); -- current screen pixel x
		Y: in std_logic_vector(7 downto 0); -- current screen pixel y

		-- internal memory block (AUX)
		AUX_WEN: in std_logic; -- VRAM AUX write enable
		AUX_ADDR: in std_logic_vector(1 downto 0); -- VRAM AUX address
		AUX_DATA: in std_logic_vector(15 downto 0); -- VRAM AUX data

		-- used memory blocks
		BAM_ADDR: out std_logic_vector(10 downto 0);
		BAM_DATA: in std_logic_vector(15 downto 0);
		TMM_ADDR: out std_logic_vector(15 downto 0);
		TMM_DATA: in std_logic_vector(15 downto 0);

		-- outputs
		CIDX: out std_logic_vector(PPU_PALETTE_CIDX_SIZE-1 downto 0)); -- output color
	end component;
  component ppu_sprite_fg port( -- foreground sprite
		-- inputs
		CLK: in std_logic; -- system clock
		OE: in std_logic; -- output enable (of CIDX)
		X: in std_logic_vector(8 downto 0); -- current screen pixel x
		Y: in std_logic_vector(7 downto 0); -- current screen pixel y
		FETCH: in std_logic; -- fetch sprite data from TMM (TODO: generic map, set foreground sprite component index)

		-- internal memory block (FAM)
		FAM_WEN: in std_logic; -- VRAM FAM write enable
		FAM_ADDR: in std_logic_vector(1 downto 0); -- VRAM fam address
		FAM_DATA: in std_logic_vector(15 downto 0); -- VRAM fam data

		-- used memory blocks
		TMM_ADDR: out std_logic_vector(15 downto 0);
		TMM_DATA: in std_logic_vector(15 downto 0);

		-- outputs
		CIDX: out std_logic_vector(PPU_PALETTE_CIDX_SIZE-1 downto 0); -- output color
		HIT: out std_logic); -- current pixel is not transparent
	end component;
	component ppu_comp port( -- compositor
		FG_HIT: in std_logic_vector(PPU_FG_SPRITE_COUNT-1 downto 0);
		BG_EN: out std_logic;
		FG_EN: out std_logic_vector(PPU_FG_SPRITE_COUNT-1 downto 0));
	end component;
	component ppu_plut port( -- palette lookup table
		CLK: in std_logic; -- system clock
		CIDX: in std_logic_vector(PPU_PALETTE_CIDX_SIZE-1 downto 0); -- color in
    RESET: in std_logic;

		-- internal memory block (AUX)
		PAL_WEN: in std_logic; -- VRAM PAL write enable
		PAL_ADDR: in std_logic_vector(5 downto 0); -- VRAM PAL address
		PAL_DATA: in std_logic_vector(11 downto 0); -- VRAM PAL data
    
    R,G,B: out std_logic_vector(3 downto 0)); -- VGA color out
	end component;
	component ppu_vga_tiny port( -- tiny vga signal generator
		CLK: in std_logic; -- system clock
    RESET: in std_logic;

		X: out std_logic_vector(8 downto 0); -- current screen pixel x
		Y: out std_logic_vector(7 downto 0); -- current screen pixel y
    PREADY: out std_logic; -- current pixel ready (pixel color is stable)
    
    VSYNC, VBLANK,
    HSYNC, HBLANK: out std_logic); -- VGA sync outputs
	end component;
	component ppu_vga_native port( -- native vga signal generator (upscaler)
		CLK: in std_logic; -- system clock
    RESET: in std_logic;

		X: in std_logic_vector(8 downto 0); -- current screen pixel x
		Y: in std_logic_vector(7 downto 0); -- current screen pixel y
    PREADY: in std_logic; -- current pixel ready (pixel color is stable)
    RI,GI,BI: in std_logic_vector(3 downto 0); -- VGA color in
    
    RO,GO,BO: out std_logic_vector(3 downto 0); -- VGA color out
    VSYNC, HSYNC: out std_logic); -- VGA sync outputs
	end component;

	-- signals
	signal SYSCLK, SYSRST: std_logic; -- system clock and reset
	signal PL_S: std_logic_vector(PPU_PIPELINE_STAGE_COUNT-1 downto 0); -- pipeline stages
  signal TMM_WEN, BAM_WEN, FAM_WEN, PAL_WEN, AUX_WEN: std_logic;
  signal ADDR_BUS: std_logic_vector(15 downto 0);
  signal CIDX: std_logic_vector(PPU_PALETTE_CIDX_SIZE-1 downto 0);
  signal BG_EN: std_logic;
  signal FG_EN, FG_HIT: std_logic_vector(PPU_FG_SPRITE_COUNT-1 downto 0);
	signal X: std_logic_vector(8 downto 0); -- current screen pixel x
	signal Y: std_logic_vector(7 downto 0); -- current screen pixel y
	signal TR,TG,TB: std_logic_vector(3 downto 0); -- tiny RGB out (to be buffered)
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
		ADDR_DRV => ADDR_BUS,
    TMM_WEN => TMM_WEN,
    BAM_WEN => BAM_WEN,
    FAM_WEN => FAM_WEN,
    PAL_WEN => PAL_WEN,
    AUX_WEN => AUX_WEN);

  background_attribute_memory: component ppu_bam port map(
		clka => SYSCLK,
		rsta => SYSRST,
		wea => (others => BAM_WEN),
		addra => ADDR_BUS(10 downto 0),
		dina => DATA(14 downto 0),
		douta => open,
		rsta_busy => open);
  tilemap_memory: component ppu_tmm port map(
		clka => SYSCLK,
		rsta => SYSRST,
		wea => (others => TMM_WEN),
		addra => ADDR_BUS(15 downto 0),
		dina => DATA(15 downto 0),
		douta => open,
		rsta_busy => open);

  background_sprite: component ppu_sprite_bg port map(
    CLK => SYSCLK,
    OE => BG_EN,
    X => X,
    Y => Y,
    AUX_WEN => AUX_WEN,
    AUX_ADDR => ADDR_BUS(1 downto 0),
    AUX_DATA => DATA(15 downto 0),
    BAM_ADDR => open,
    BAM_DATA => (others => '0'),
    TMM_ADDR => open,
    TMM_DATA => (others => '0'),
    CIDX => CIDX);

	foreground_sprites: for FG_IDX in 0 to PPU_FG_SPRITE_COUNT-1 generate
		foreground_sprite: component ppu_sprite_fg port map(
			CLK => SYSCLK,
			OE => FG_EN(FG_IDX),
			X => X,
			Y => Y,
			FETCH => '0',
			FAM_WEN => FAM_WEN,
			FAM_ADDR => (others => '0'),
			FAM_DATA => (others => '0'),
			TMM_ADDR => open,
			TMM_DATA => (others => '0'),
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
		PAL_WEN => '0',
		PAL_ADDR => (others => '0'),
		PAL_DATA => (others => '0'),
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
