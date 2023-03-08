library ieee;
use ieee.math_real.all;

-- https://docs.google.com/spreadsheets/d/1MU6K4c4PtMR_JXIpc3I0ZJdLZNnoFO7G2P3olCz6LSc
package ppu_consts is
	-- utility functions
	function ceil_log2(n : natural) return natural;

	constant PPU_RAM_BUS_ADDR_WIDTH : natural := 16; -- RAM bus address width
	constant PPU_RAM_BUS_DATA_WIDTH : natural := 16; -- RAM bus data width
	constant PPU_FG_SPRITE_COUNT : natural := 128; -- foreground sprites
	constant PPU_COLOR_OUTPUT_DEPTH : natural := 4; -- VGA output channel depth
	constant PPU_RGB_COLOR_OUTPUT_DEPTH : natural := 3 * PPU_COLOR_OUTPUT_DEPTH;
	constant PPU_PALETTE_COLOR_WIDTH : natural := 3; -- palette index width (within sprite)
	constant PPU_PALETTE_INDEX_WIDTH : natural := 3; -- palette index width (palette table)
	constant PPU_PALETTE_CIDX_WIDTH : natural := (PPU_PALETTE_COLOR_WIDTH + PPU_PALETTE_INDEX_WIDTH); -- global palette index width
	constant PPU_TMM_ADDR_WIDTH : natural := 16; -- tilemap memory ram bus address width
	constant PPU_TMM_DATA_WIDTH : natural := 15; -- tilemap memory ram bus data width
	constant PPU_BAM_ADDR_WIDTH : natural := 11; -- background attribute memory ram bus address width
	constant PPU_BAM_DATA_WIDTH : natural := 15; -- background attribute memory ram bus data width
	constant PPU_FAM_ADDR_WIDTH : natural := 8; -- foreground attribute memory ram bus address width
	constant PPU_FAM_DATA_WIDTH : natural := 16; -- foreground attribute memory ram bus data width
	constant PPU_PAL_ADDR_WIDTH : natural := 6; -- palette memory ram bus address width
	constant PPU_PAL_DATA_WIDTH : natural := 12; -- palette memory ram bus data width
	constant PPU_AUX_ADDR_WIDTH : natural := 1; -- auxiliary memory ram bus address width
	constant PPU_AUX_DATA_WIDTH : natural := 16; -- auxiliary memory ram bus data width
	constant PPU_POS_H_WIDTH : natural := 9; -- bits for horizontal screen offset
	constant PPU_POS_V_WIDTH : natural := 8; -- bits for vertical screen offset
	constant PPU_SPRITE_WIDTH : natural := 16; -- sprite width (pixels)
	constant PPU_SPRITE_HEIGHT : natural := 16; -- sprite height (pixels)
	constant PPU_SPRITE_PIDX_WIDTH : natural := 8; -- bits needed to identify horizontal and vertical pixel within sprite
	constant PPU_SPRITE_POS_H_WIDTH: natural := 4; -- bits needed to identify horizontal pixel within sprite
	constant PPU_SPRITE_POS_V_WIDTH: natural := 4; -- bits needed to identify vertical pixel within sprite
	constant PPU_SCREEN_WIDTH : natural := 320; -- absolute screen width (tiny pixels)
	constant PPU_SCREEN_HEIGHT : natural := 240; -- absolute screen height (tiny pixels)
	constant PPU_NATIVE_SCREEN_WIDTH : natural := 2 * PPU_SCREEN_WIDTH; -- screen width (native pixels)
	constant PPU_NATIVE_SCREEN_HEIGHT : natural := 2 * PPU_SCREEN_HEIGHT; -- screen height (native pixels)
	constant PPU_DISPCTL_SLBUF_ADDR_WIDTH : natural := ceil_log2(2 * PPU_SCREEN_WIDTH);
	constant PPU_BG_CANVAS_TILES_H : natural := 40; -- tiles (horizontally) on background canvas
	constant PPU_BG_CANVAS_TILES_V : natural := 30; -- tiles (vertically) on background canvas
	constant PPU_BG_CANVAS_TILE_H_WIDTH : natural := 6; -- bits needed to describe horizontal bg tile index (grid coordinates)
	constant PPU_BG_CANVAS_TILE_V_WIDTH : natural := 5; -- bits needed to describe vertical bg tile index (grid coordinates)
	constant PPU_TILE_INDEX_WIDTH : natural := 10; -- bits needed to index a tile from TMM memory
	constant PPU_PIXELS_PER_TILE_WORD : natural := 5; -- pixels defined in one word in TMM memory
	constant PPU_SPRITE_WORD_COUNT : natural := 52; -- words needed for a single sprite
	constant PPU_PIXEL_BIT_WIDTH : natural := 3; -- bits needed to identify pixel in TMM word
	constant PPU_TILE_BIT_WIDTH : natural := (PPU_SPRITE_WIDTH * PPU_SPRITE_HEIGHT * PPU_PALETTE_COLOR_WIDTH); -- bits in single tile
	constant PPU_TMM_CACHE_FETCH_C_COUNT : natural := PPU_SPRITE_WORD_COUNT + 1;
	constant PPU_TMM_CACHE_FETCH_A_COUNT : natural := PPU_TMM_CACHE_FETCH_C_COUNT * PPU_FG_SPRITE_COUNT; -- amount of clocks to fetch new TMM cache
	constant PPU_TMM_CACHE_FETCH_A_WIDTH : natural := ceil_log2(PPU_TMM_CACHE_FETCH_A_COUNT);
	constant PPU_ACCURATE_FG_SPRITE_COUNT : natural := 16;
	constant PPU_PL_TOTAL_STAGES : natural := 14;
	-- VGA signal timings (https://tomverbeure.github.io/video_timings_calculator)
	constant PPU_VGA_H_ACTIVE : natural := PPU_NATIVE_SCREEN_WIDTH;
	constant PPU_VGA_H_PORCH_FRONT : natural := 16;
	constant PPU_VGA_H_SYNC : natural := 64;
	constant PPU_VGA_H_PORCH_BACK : natural := 80;
	constant PPU_VGA_H_BLANK : natural := PPU_VGA_H_PORCH_FRONT + PPU_VGA_H_SYNC + PPU_VGA_H_PORCH_BACK;
	constant PPU_VGA_H_TOTAL : natural := PPU_VGA_H_BLANK + PPU_VGA_H_ACTIVE;
	constant PPU_VGA_V_ACTIVE : natural := PPU_NATIVE_SCREEN_HEIGHT;
	constant PPU_VGA_V_PORCH_FRONT : natural := 4;
	constant PPU_VGA_V_SYNC : natural := 4;
	constant PPU_VGA_V_PORCH_BACK : natural := 12;
	constant PPU_VGA_V_BLANK : natural := PPU_VGA_V_PORCH_FRONT + PPU_VGA_V_SYNC + PPU_VGA_V_PORCH_BACK;
	constant PPU_VGA_V_TOTAL : natural := PPU_VGA_V_BLANK + PPU_VGA_V_ACTIVE;
	constant PPU_VGA_SIGNAL_PIXEL_IDX_MAX : natural := PPU_VGA_V_TOTAL * PPU_VGA_H_TOTAL; -- horizontal and vertical pixel clock index
	constant PPU_VGA_SIGNAL_PIXEL_WIDTH : natural := ceil_log2(PPU_VGA_SIGNAL_PIXEL_IDX_MAX); -- bit width to count total horizontal and vertical pixel clock index
	constant PPU_SCREEN_T_POS_X_WIDTH : natural := ceil_log2(PPU_SCREEN_WIDTH);
	constant PPU_SCREEN_T_POS_Y_WIDTH : natural := ceil_log2(PPU_SCREEN_HEIGHT);
	constant PPU_SCREEN_N_POS_X_WIDTH : natural := ceil_log2(PPU_NATIVE_SCREEN_WIDTH);
	constant PPU_SCREEN_N_POS_Y_WIDTH : natural := ceil_log2(PPU_NATIVE_SCREEN_HEIGHT);
end package ppu_consts;
package body ppu_consts is
	-- https://stackoverflow.com/questions/21783280/number-of-bits-to-represent-an-integer-in-vhdl
	-- Returns number of bits required to represent val in binary vector
	function ceil_log2(n : natural) return natural is
	begin
		return natural(integer(ceil(log2(real(n - 1)))));
	end function;
end package body ppu_consts;
