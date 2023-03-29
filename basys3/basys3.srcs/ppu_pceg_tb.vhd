library ieee;
library unisim;

use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use unisim.vcomponents.all;
use work.ppu_pceg_consts.all;

entity ppu_pceg_tb is
end ppu_pceg_tb;

architecture behavioral of ppu_pceg_tb is
	component ppu_pceg port(
		CLK : in std_logic; -- system clock
		RESET : in std_logic; -- async reset
		SPRITE_BG : out ppu_sprite_bg_pl_state := PL_BG_IDLE; -- sprite info fetch + sprite pixel fetch
		SPRITE_FG : out ppu_sprite_fg_pl_state := PL_FG_IDLE; -- sprite pixel fetch
		DONE : out std_logic; -- last pipeline stage done
		READY : out std_logic); -- rgb buffer propagation ready
	end component;
	signal CLK : std_logic := '0';
	signal RESET : std_logic := '0';

begin
	uut : ppu_pceg port map(
		CLK => CLK,
		RESET => RESET,
		SPRITE_BG => open,
		SPRITE_FG => open,
		DONE => open,
		READY => open);

	tb : process
	begin
		for i in 0 to 32 loop
			wait for 5 ns;
			CLK <= '1';
			wait for 5 ns;
			CLK <= '0';
		end loop;
		wait; -- stop for simulator
	end process;

	gert : process
	begin
		RESET <= '1';
		wait for 1 ns;
		RESET <= '0';
		wait for 100 ns;
		RESET <= '1';
		wait for 5 ns;
		RESET <= '0';
		wait;
	end process;
end;
