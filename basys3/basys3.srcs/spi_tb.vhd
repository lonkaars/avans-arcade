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
	signal SPI_DATA : std_logic := '0';
	signal RESET : std_logic := '0';
begin
	uut : entity work.spi port map(
		SYSCLK => SYSCLK,
		RESET => RESET,
		DO => open,
		DI => SPI_DATA,
		DCK => SPI_CLK,
		WEN => open);

	process
	begin
		for i in 0 to 10000 loop
			wait for 5 ns;
			SYSCLK <= '1';
			wait for 5 ns;
			SYSCLK <= '0';
		end loop;
		wait; -- stop for simulator
	end process;

	process
	begin
		for i in 0 to 2 loop
			-- data = 0b01010110010100001001110011111111 (0x56509cff)
			SPI_DATA <= '0';
			wait for 50 ns;
			SPI_CLK <= '1';
			wait for 50 ns;
			SPI_CLK <= '0';

			SPI_DATA <= '1';
			wait for 50 ns;
			SPI_CLK <= '1';
			wait for 50 ns;
			SPI_CLK <= '0';

			SPI_DATA <= '0';
			wait for 50 ns;
			SPI_CLK <= '1';
			wait for 50 ns;
			SPI_CLK <= '0';

			SPI_DATA <= '1';
			wait for 50 ns;
			SPI_CLK <= '1';
			wait for 50 ns;
			SPI_CLK <= '0';

			SPI_DATA <= '0';
			wait for 50 ns;
			SPI_CLK <= '1';
			wait for 50 ns;
			SPI_CLK <= '0';

			SPI_DATA <= '1';
			wait for 50 ns;
			SPI_CLK <= '1';
			wait for 50 ns;
			SPI_CLK <= '0';

			SPI_DATA <= '1';
			wait for 50 ns;
			SPI_CLK <= '1';
			wait for 50 ns;
			SPI_CLK <= '0';

			SPI_DATA <= '0';
			wait for 50 ns;
			SPI_CLK <= '1';
			wait for 50 ns;
			SPI_CLK <= '0';

			SPI_DATA <= '0';
			wait for 50 ns;
			SPI_CLK <= '1';
			wait for 50 ns;
			SPI_CLK <= '0';

			SPI_DATA <= '1';
			wait for 50 ns;
			SPI_CLK <= '1';
			wait for 50 ns;
			SPI_CLK <= '0';

			SPI_DATA <= '0';
			wait for 50 ns;
			SPI_CLK <= '1';
			wait for 50 ns;
			SPI_CLK <= '0';

			SPI_DATA <= '1';
			wait for 50 ns;
			SPI_CLK <= '1';
			wait for 50 ns;
			SPI_CLK <= '0';

			SPI_DATA <= '0';
			wait for 50 ns;
			SPI_CLK <= '1';
			wait for 50 ns;
			SPI_CLK <= '0';

			SPI_DATA <= '0';
			wait for 50 ns;
			SPI_CLK <= '1';
			wait for 50 ns;
			SPI_CLK <= '0';

			SPI_DATA <= '0';
			wait for 50 ns;
			SPI_CLK <= '1';
			wait for 50 ns;
			SPI_CLK <= '0';

			SPI_DATA <= '0';
			wait for 50 ns;
			SPI_CLK <= '1';
			wait for 50 ns;
			SPI_CLK <= '0';

			SPI_DATA <= '1';
			wait for 50 ns;
			SPI_CLK <= '1';
			wait for 50 ns;
			SPI_CLK <= '0';

			SPI_DATA <= '0';
			wait for 50 ns;
			SPI_CLK <= '1';
			wait for 50 ns;
			SPI_CLK <= '0';

			SPI_DATA <= '0';
			wait for 50 ns;
			SPI_CLK <= '1';
			wait for 50 ns;
			SPI_CLK <= '0';

			SPI_DATA <= '1';
			wait for 50 ns;
			SPI_CLK <= '1';
			wait for 50 ns;
			SPI_CLK <= '0';

			SPI_DATA <= '1';
			wait for 50 ns;
			SPI_CLK <= '1';
			wait for 50 ns;
			SPI_CLK <= '0';

			SPI_DATA <= '1';
			wait for 50 ns;
			SPI_CLK <= '1';
			wait for 50 ns;
			SPI_CLK <= '0';

			SPI_DATA <= '0';
			wait for 50 ns;
			SPI_CLK <= '1';
			wait for 50 ns;
			SPI_CLK <= '0';

			SPI_DATA <= '0';
			wait for 50 ns;
			SPI_CLK <= '1';
			wait for 50 ns;
			SPI_CLK <= '0';

			SPI_DATA <= '1';
			wait for 50 ns;
			SPI_CLK <= '1';
			wait for 50 ns;
			SPI_CLK <= '0';

			SPI_DATA <= '1';
			wait for 50 ns;
			SPI_CLK <= '1';
			wait for 50 ns;
			SPI_CLK <= '0';

			SPI_DATA <= '1';
			wait for 50 ns;
			SPI_CLK <= '1';
			wait for 50 ns;
			SPI_CLK <= '0';

			SPI_DATA <= '1';
			wait for 50 ns;
			SPI_CLK <= '1';
			wait for 50 ns;
			SPI_CLK <= '0';

			SPI_DATA <= '1';
			wait for 50 ns;
			SPI_CLK <= '1';
			wait for 50 ns;
			SPI_CLK <= '0';

			SPI_DATA <= '1';
			wait for 50 ns;
			SPI_CLK <= '1';
			wait for 50 ns;
			SPI_CLK <= '0';

			SPI_DATA <= '1';
			wait for 50 ns;
			SPI_CLK <= '1';
			wait for 50 ns;
			SPI_CLK <= '0';

			SPI_DATA <= '1';
			wait for 50 ns;
			SPI_CLK <= '1';
			wait for 50 ns;
			SPI_CLK <= '0';
		end loop;

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
