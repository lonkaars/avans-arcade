library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity top is port (
	SYSCLK : in std_logic; -- clock basys3 100MHz
	RESET : in std_logic; -- global (async) system reset
	SPI_CLK, SPI_MOSI, SPI_SR : in std_logic;
	DBG_SWTC_IN : in std_logic_vector(15 downto 0); -- switches
	DBG_LEDS_OUT : out std_logic_vector(15 downto 0); -- leds
	VBLANK, HBLANK : out std_logic); -- vblank for synchronization
end top;

architecture Behavioral of top is
begin
	process(SYSCLK, RESET)
	begin
		if RESET = '1' then
			VBLANK <= '0';
			HBLANK <= '0';
			DBG_LEDS_OUT(15) <= '0';
			DBG_LEDS_OUT(14) <= '0';
			DBG_LEDS_OUT(13) <= '0';
		elsif rising_edge(SYSCLK) then
			VBLANK <= DBG_SWTC_IN(0);
			HBLANK <= DBG_SWTC_IN(1);
			DBG_LEDS_OUT(15) <= SPI_SR;
			DBG_LEDS_OUT(14) <= SPI_CLK;
			DBG_LEDS_OUT(13) <= SPI_MOSI;
		end if;
	end process;
end Behavioral;
