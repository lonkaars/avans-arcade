library ieee;
use ieee.std_logic_1164.all;
use work.ppu_consts.all;

entity ppu_pceg is port(
	CLK : in std_logic; -- system clock
	RESET : in std_logic; -- async reset
	SPRITE_BG : out std_logic; -- sprite info fetch + sprite pixel fetch
	SPRITE_FG : out std_logic; -- sprite pixel fetch
	DONE : out std_logic; -- last pipeline stage done
	READY : out std_logic); -- rgb buffer propagation ready
end ppu_pceg;

architecture Behavioral of ppu_pceg is
	signal PL_SPRITE_BG, PL_SPRITE_FG, PL_DONE, PL_READY : boolean := false;
begin
	-- output drivers
	SPRITE_BG <= CLK when RESET = '0' and PL_SPRITE_BG else '0';
	SPRITE_FG <= CLK when RESET = '0' and PL_SPRITE_FG else '0';
	DONE <= CLK when RESET = '0' and PL_DONE else '0';
	READY <= '1' when RESET = '0' and PL_READY else '0';

	process(CLK, RESET)
		variable CLK_IDX : natural range 0 to PPU_PL_TOTAL_STAGES+1 := 0;
	begin
		if RESET = '1' then
			CLK_IDX := 0;
			PL_SPRITE_BG <= false;
			PL_SPRITE_FG <= false;
			PL_DONE <= false;
			PL_READY <= false;
		elsif rising_edge(CLK) then
			-- clock counter ranges
			PL_SPRITE_BG <= true when CLK_IDX >= 0 and CLK_IDX <= 3 else false;
			PL_SPRITE_FG <= true when CLK_IDX >= 1 and CLK_IDX <= 2 else false;
			PL_DONE <= true when CLK_IDX = 4 else false;
			PL_READY <= true when CLK_IDX >= 5 else false;

			-- increment clock counter
			CLK_IDX := CLK_IDX + 1;
			if CLK_IDX = PPU_PL_TOTAL_STAGES then
				CLK_IDX := 0;
			end if;
		end if;
	end process;
end Behavioral;
