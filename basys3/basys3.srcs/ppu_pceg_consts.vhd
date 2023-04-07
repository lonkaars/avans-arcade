library ieee;
use ieee.std_logic_1164.all;
use work.ppu_consts.all;

package ppu_pceg_consts is
	constant PPU_PCEG_TOTAL_STAGES : natural := 16;
	type ppu_sprite_bg_pl_state is (
		PL_BG_IDLE,
		PL_BG_BAM_ADDR,
		PL_BG_BAM_DATA,
		PL_BG_TMM_ADDR,
		PL_BG_TMM_DATA);
	type ppu_sprite_fg_pl_state is (
		PL_FG_IDLE,
		PL_FG_TMM_ADDR,
		PL_FG_TMM_DATA);
	type ppu_sprite_fg_hit_pl_state is (
		PL_HIT_INACCURATE,
		PL_HIT_ACCURATE);
end package ppu_pceg_consts;

