package ppu_consts is
	constant PPU_RAM_BUS_ADDR_WIDTH: natural := 16; -- RAM bus address width
	constant PPU_RAM_BUS_DATA_WIDTH: natural := 16; -- RAM bus data width
	constant PPU_FG_SPRITE_COUNT: natural := 128; -- amount of foreground sprites
	constant PPU_COLOR_OUTPUT_DEPTH: natural := 4; -- VGA output channel depth
	constant PPU_PALETTE_IDX_WIDTH: natural := 3; -- palette index width (within sprite)
	constant PPU_PALETTE_WIDTH: natural := 3; -- palette index width (palette table)
	constant PPU_PALETTE_CIDX_WIDTH: natural := PPU_PALETTE_IDX_WIDTH + PPU_PALETTE_WIDTH; -- global palette index width
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
end package ppu_consts;

