library ieee;
use ieee.std_logic_1164.all;
use work.ppu_consts.all;

entity ppu_pceg is port(
	CLK : in std_logic; -- system clock
	RESET : in std_logic; -- async reset
	SPRITE : out std_logic; -- sprite info fetch + sprite pixel fetch
	DONE : out std_logic; -- last pipeline stage done
	READY : out std_logic); -- rgb buffer propagation ready
end ppu_pceg;

architecture Behavioral of ppu_pceg is
	type states is (PL_SPRITE, PL_DONE, PL_READY);
	signal state : states := PL_SPRITE;
begin
	-- output drivers
	SPRITE <= CLK when RESET = '0' and state = PL_SPRITE else '0';
	DONE <= CLK when RESET = '0' and state = PL_DONE else '0';
	READY <= '1' when RESET = '0' and state = PL_READY else '0';

	process(CLK, RESET)
		variable CLK_IDX : natural range 0 to PPU_PL_TOTAL_STAGES+1 := 0;
	begin
		if RESET = '1' then
			state <= PL_SPRITE;
		elsif rising_edge(CLK) then
			-- clock counter ranges
			if CLK_IDX < 4 then
				state <= PL_SPRITE;
			elsif CLK_IDX < 5 then
				state <= PL_DONE;
			else
				state <= PL_READY;
			end if;

			-- increment clock counter
			CLK_IDX := CLK_IDX + 1;
			if CLK_IDX = PPU_PL_TOTAL_STAGES then
				CLK_IDX := 0;
			end if;
		end if;
	end process;
end Behavioral;
