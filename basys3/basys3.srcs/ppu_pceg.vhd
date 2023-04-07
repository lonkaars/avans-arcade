library ieee;
use ieee.std_logic_1164.all;
use work.ppu_consts.all;
use work.ppu_pceg_consts.all;

entity ppu_pceg is port(
	CLK : in std_logic; -- system clock
	RESET : in std_logic; -- async reset
	SPRITE_BG : out ppu_sprite_bg_pl_state := PL_BG_IDLE; -- sprite info fetch + sprite pixel fetch
	SPRITE_FG : out ppu_sprite_fg_pl_state := PL_FG_IDLE; -- sprite pixel fetch
	SPRITE_FG_HIT : out ppu_sprite_fg_hit_pl_state := PL_HIT_INACCURATE; -- foreground hit accuracy
	DISPCTL_BWEN : out std_logic := '0'); -- display controller scanline buffer write enable
end ppu_pceg;

architecture Behavioral of ppu_pceg is
	signal CLK_IDX_T : natural range 0 to PPU_PCEG_TOTAL_STAGES+1 := 0;
begin
	process(CLK, RESET)
		variable CLK_IDX : natural range 0 to PPU_PCEG_TOTAL_STAGES+1 := 0;
	begin
		if RESET = '1' then
			CLK_IDX := 0;

			SPRITE_BG <= PL_BG_IDLE;
			SPRITE_FG <= PL_FG_IDLE;
			DISPCTL_BWEN <= '0';
		elsif falling_edge(CLK) then
			case CLK_IDX is
				when 0 =>
					DISPCTL_BWEN <= '0';
					SPRITE_BG <= PL_BG_IDLE;
					SPRITE_FG <= PL_FG_IDLE;
					SPRITE_FG_HIT <= PL_HIT_INACCURATE;
				when 1 =>
					SPRITE_BG <= PL_BG_BAM_ADDR;
					SPRITE_FG <= PL_FG_TMM_ADDR;
				when 2 =>
					SPRITE_BG <= PL_BG_BAM_DATA;
					SPRITE_FG <= PL_FG_TMM_DATA;
				when 3 =>
					SPRITE_BG <= PL_BG_TMM_ADDR;
					SPRITE_FG <= PL_FG_IDLE;
					SPRITE_FG_HIT <= PL_HIT_ACCURATE;
				when 4 =>
					SPRITE_BG <= PL_BG_TMM_DATA;
				when 5 =>
					SPRITE_BG <= PL_BG_IDLE;
				when 6 =>
					DISPCTL_BWEN <= '1';
				when 7 =>
					DISPCTL_BWEN <= '0';
				when others => null;
			end case;

			-- increment clock counter
			CLK_IDX := CLK_IDX + 1;
			if CLK_IDX = PPU_PCEG_TOTAL_STAGES then
				CLK_IDX := 0;
			end if;

			CLK_IDX_T <= CLK_IDX;
		end if;
	end process;
end Behavioral;

