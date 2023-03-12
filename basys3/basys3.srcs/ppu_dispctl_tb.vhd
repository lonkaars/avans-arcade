library ieee;
library unisim;

use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use unisim.vcomponents.all;
use work.ppu_consts.all;

entity ppu_dispctl_tb is
end ppu_dispctl_tb;

architecture behavioral of ppu_dispctl_tb is
	signal SYSCLK : std_logic := '0';
	signal RESET : std_logic := '0';
	signal X : std_logic_vector(PPU_POS_H_WIDTH-1 downto 0) := (others => '0');
	signal Y : std_logic_vector(PPU_POS_V_WIDTH-1 downto 0) := (others => '0');
	signal RO,GO,BO : std_logic_vector(PPU_COLOR_OUTPUT_DEPTH-1 downto 0) := (others => '0');
	signal NVSYNC, NHSYNC : std_logic := '0';
	signal THBLANK, TVBLANK : std_logic := '0';
begin
	uut : entity work.ppu_dispctl port map(
		SYSCLK => SYSCLK,
		RESET => RESET,
		PREADY => '1',
		X => X,
		Y => Y,
		RI => (others => '1'),
		GI => (others => '0'),
		BI => (others => '1'),
		RO => RO,
		GO => GO,
		BO => BO,
		NVSYNC => NVSYNC,
		NHSYNC => NHSYNC,
		TVBLANK => TVBLANK,
		THBLANK => THBLANK);

	process
	begin
		for i in 0 to 3200000 loop
			wait for 5 ns;
			SYSCLK <= '1';
			wait for 5 ns;
			SYSCLK <= '0';
		end loop;
		wait; -- stop for simulator
	end process;
end;
