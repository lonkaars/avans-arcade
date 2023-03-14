library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_unsigned.all;
use work.ppu_consts.all;

entity spi is port (
	SYSCLK : in std_logic; -- clock basys3 100MHz
	SPI_CLK : in std_logic; -- incoming clock of SPI
	SPI_MOSI : in std_logic; -- incoming data of SPI
	RESET : in std_logic; -- async reset
	DATA : out std_logic_vector(PPU_RAM_BUS_ADDR_WIDTH+PPU_RAM_BUS_DATA_WIDTH-1 downto 0) := (others => '0')); --	data read
end spi;

architecture Behavioral of spi is
	signal clkFF0,clkFF1,clkFF2,clkFF3 : std_logic := '0'; -- signal for metastability synchronizer of clk SPI
	signal dataFF0,dataFF1,dataFF2,dataFF3 : std_logic := '0'; -- signal for metastability synchronizer of data SPI

	signal SPI_REG : std_logic_vector(PPU_RAM_BUS_ADDR_WIDTH+PPU_RAM_BUS_DATA_WIDTH-1 downto 0) := (others => '0');
	signal counter : integer := 31; -- counter for data position

	constant COUNTER_RESET_VALUE : integer := PPU_RAM_BUS_ADDR_WIDTH + PPU_RAM_BUS_DATA_WIDTH - 1;
begin
	process (SYSCLK)
	begin
		if RESET = '1' then
			 counter <= COUNTER_RESET_VALUE;
			 DATA <= (others => '0');
		elsif rising_edge(SYSCLK) then
			-- flip flop for clk SPI to synchronise a
			clkFF0 <= SPI_CLK;
			clkFF1 <= clkFF0;
			clkFF2 <= clkFF1;
			clkFF3 <= clkFF2;
			-- flip flop for data SPI to synchronise
			dataFF0 <= SPI_MOSI;
			dataFF1 <= dataFF0;
			dataFF2 <= dataFF1;
			dataFF3 <= dataFF2;

			if (clkFF3 = '0' and clkFF2 = '1') then -- check for rising edge of clk SPI
				if counter > -1 then
					counter <= counter - 1;
					-- data transfer into vector
					SPI_REG(counter) <= dataFF3;
				end if;
			end if;
			-- check if counter is done
			if counter = -1 then
				counter <= COUNTER_RESET_VALUE; -- reset counter
				DATA <= SPI_REG;
			end if;
		end if;
	end process;
end Behavioral;
