library ieee;
library unisim;

use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use unisim.vcomponents.all;
use work.ppu_consts.all;

entity spi_tb is
end spi_tb;

architecture behavioral of spi_tb is
	signal SYSCLK : std_logic := '0';
	signal SPI_CLK : std_logic := '0';
	signal SPI_MOSI : std_logic := '0';
	signal RESET : std_logic := '0';
	signal DATA : std_logic_vector(PPU_RAM_BUS_ADDR_WIDTH+PPU_RAM_BUS_DATA_WIDTH-1 downto 0) := (others => '0');
begin
	uut : entity work.spi port map(
		SYSCLK => SYSCLK,
		RESET => RESET,
		DATA => DATA,
		SPI_CLK => SPI_CLK,
		SPI_MOSI => SPI_MOSI);

	sysclkgen: process
	begin
		for i in 0 to 10000 loop
			wait for 5 ns;
			SYSCLK <= '1';
			wait for 5 ns;
			SYSCLK <= '0';
		end loop;
		wait; -- stop for simulator
	end process;

	spi_data: process
	begin
		SPI_MOSI <= '0';
		wait for 50 ns;
		SPI_CLK <= '1';
		wait for 50 ns;
		SPI_CLK <= '0';

		SPI_MOSI <= '1';
		wait for 50 ns;
		SPI_CLK <= '1';
		wait for 50 ns;
		SPI_CLK <= '0';

		SPI_MOSI <= '0';
		wait for 50 ns;
		SPI_CLK <= '1';
		wait for 50 ns;
		SPI_CLK <= '0';

		SPI_MOSI <= '1';
		wait for 50 ns;
		SPI_CLK <= '1';
		wait for 50 ns;
		SPI_CLK <= '0';

		SPI_MOSI <= '0';
		wait for 50 ns;
		SPI_CLK <= '1';
		wait for 50 ns;
		SPI_CLK <= '0';

		SPI_MOSI <= '1';
		wait for 50 ns;
		SPI_CLK <= '1';
		wait for 50 ns;
		SPI_CLK <= '0';

		SPI_MOSI <= '1';
		wait for 50 ns;
		SPI_CLK <= '1';
		wait for 50 ns;
		SPI_CLK <= '0';

		SPI_MOSI <= '0';
		wait for 50 ns;
		SPI_CLK <= '1';
		wait for 50 ns;
		SPI_CLK <= '0';

		SPI_MOSI <= '0';
		wait for 50 ns;
		SPI_CLK <= '1';
		wait for 50 ns;
		SPI_CLK <= '0';

		SPI_MOSI <= '1';
		wait for 50 ns;
		SPI_CLK <= '1';
		wait for 50 ns;
		SPI_CLK <= '0';

		SPI_MOSI <= '0';
		wait for 50 ns;
		SPI_CLK <= '1';
		wait for 50 ns;
		SPI_CLK <= '0';

		SPI_MOSI <= '1';
		wait for 50 ns;
		SPI_CLK <= '1';
		wait for 50 ns;
		SPI_CLK <= '0';

		SPI_MOSI <= '0';
		wait for 50 ns;
		SPI_CLK <= '1';
		wait for 50 ns;
		SPI_CLK <= '0';

		SPI_MOSI <= '0';
		wait for 50 ns;
		SPI_CLK <= '1';
		wait for 50 ns;
		SPI_CLK <= '0';

		SPI_MOSI <= '0';
		wait for 50 ns;
		SPI_CLK <= '1';
		wait for 50 ns;
		SPI_CLK <= '0';

		SPI_MOSI <= '0';
		wait for 50 ns;
		SPI_CLK <= '1';
		wait for 50 ns;
		SPI_CLK <= '0';

		SPI_MOSI <= '1';
		wait for 50 ns;
		SPI_CLK <= '1';
		wait for 50 ns;
		SPI_CLK <= '0';

		SPI_MOSI <= '0';
		wait for 50 ns;
		SPI_CLK <= '1';
		wait for 50 ns;
		SPI_CLK <= '0';

		SPI_MOSI <= '0';
		wait for 50 ns;
		SPI_CLK <= '1';
		wait for 50 ns;
		SPI_CLK <= '0';

		SPI_MOSI <= '1';
		wait for 50 ns;
		SPI_CLK <= '1';
		wait for 50 ns;
		SPI_CLK <= '0';

		SPI_MOSI <= '1';
		wait for 50 ns;
		SPI_CLK <= '1';
		wait for 50 ns;
		SPI_CLK <= '0';

		SPI_MOSI <= '1';
		wait for 50 ns;
		SPI_CLK <= '1';
		wait for 50 ns;
		SPI_CLK <= '0';

		SPI_MOSI <= '0';
		wait for 50 ns;
		SPI_CLK <= '1';
		wait for 50 ns;
		SPI_CLK <= '0';

		SPI_MOSI <= '0';
		wait for 50 ns;
		SPI_CLK <= '1';
		wait for 50 ns;
		SPI_CLK <= '0';

		SPI_MOSI <= '1';
		wait for 50 ns;
		SPI_CLK <= '1';
		wait for 50 ns;
		SPI_CLK <= '0';

		SPI_MOSI <= '1';
		wait for 50 ns;
		SPI_CLK <= '1';
		wait for 50 ns;
		SPI_CLK <= '0';

		SPI_MOSI <= '1';
		wait for 50 ns;
		SPI_CLK <= '1';
		wait for 50 ns;
		SPI_CLK <= '0';

		SPI_MOSI <= '1';
		wait for 50 ns;
		SPI_CLK <= '1';
		wait for 50 ns;
		SPI_CLK <= '0';

		SPI_MOSI <= '1';
		wait for 50 ns;
		SPI_CLK <= '1';
		wait for 50 ns;
		SPI_CLK <= '0';

		SPI_MOSI <= '1';
		wait for 50 ns;
		SPI_CLK <= '1';
		wait for 50 ns;
		SPI_CLK <= '0';

		SPI_MOSI <= '1';
		wait for 50 ns;
		SPI_CLK <= '1';
		wait for 50 ns;
		SPI_CLK <= '0';

		SPI_MOSI <= '1';
		wait for 50 ns;
		SPI_CLK <= '1';
		wait for 50 ns;
		SPI_CLK <= '0';

		wait for 50 ns;
		SPI_CLK <= '1';
		wait for 50 ns;
		SPI_CLK <= '0';
		wait for 50 ns;
		SPI_CLK <= '1';
		wait for 50 ns;
		SPI_CLK <= '0';
		wait for 50 ns;
		SPI_CLK <= '1';
		wait for 50 ns;
		SPI_CLK <= '0';
		wait for 50 ns;
		SPI_CLK <= '1';
		wait for 50 ns;
		SPI_CLK <= '0';
		wait for 50 ns;
		SPI_CLK <= '1';
		wait for 50 ns;
		SPI_CLK <= '0';
		wait for 50 ns;
		RESET <= '1';
		wait for 50 ns;
		RESET <= '0';


		wait; -- stop for simulator
	end process;
end;
