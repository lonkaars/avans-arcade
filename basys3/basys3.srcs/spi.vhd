library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_unsigned.all;
use work.ppu_consts.all;

entity spi is port (
	SYSCLK : in std_logic; -- clock basys3 100MHz
	RESET : in std_logic; -- async reset
	DCK : in std_logic; -- data clock (spi format)
	DI : in std_logic; -- data in (spi format)
	DO : out std_logic_vector(PPU_RAM_BUS_ADDR_WIDTH+PPU_RAM_BUS_DATA_WIDTH-1 downto 0) := (others => '1'); --	data out (parallel)
	WEN : out std_logic := '0'); -- write enable (triggers during each word to propagate previous word)
end spi;

architecture Behavioral of spi is
	signal clkFF0,clkFF1,clkFF2,clkFF3 : std_logic := '0'; -- signal for metastability synchronizer of clk SPI
	signal dataFF0,dataFF1,dataFF2,dataFF3 : std_logic := '0'; -- signal for metastability synchronizer of data SPI

	constant COUNTER_RESET_VALUE : integer := PPU_RAM_BUS_ADDR_WIDTH + PPU_RAM_BUS_DATA_WIDTH - 1;
	signal DBG_I : integer range 0 to COUNTER_RESET_VALUE := COUNTER_RESET_VALUE; -- counter for data position
begin
	process (SYSCLK)
		variable i : integer range 0 to COUNTER_RESET_VALUE := COUNTER_RESET_VALUE; -- counter for data position
		variable data_r : std_logic_vector(PPU_RAM_BUS_ADDR_WIDTH+PPU_RAM_BUS_DATA_WIDTH-1 downto 0) := (others => '1'); -- data register
	begin
		if RESET = '1' then
			data_r := (others => '1');
			i := COUNTER_RESET_VALUE;
			DBG_I <= i;
			DO <= (others => '1');
			WEN <= '0';
		elsif rising_edge(SYSCLK) then
			-- flip flop for clk SPI to synchronise a
			clkFF0 <= DCK;
			clkFF1 <= clkFF0;
			clkFF2 <= clkFF1;
			clkFF3 <= clkFF2;
			-- flip flop for data SPI to synchronise
			dataFF0 <= DI;
			dataFF1 <= dataFF0;
			dataFF2 <= dataFF1;
			dataFF3 <= dataFF2;

			if (clkFF3 = '0' and clkFF2 = '1') then -- check for rising edge of clk SPI
				data_r(i) := dataFF3; -- load new data into temporary register

				if i = 0 then
					i := COUNTER_RESET_VALUE; -- reset bit index
					DO <= data_r; -- flush temporary register to data outpu
				else
					i := i - 1; -- decrement bit index
				end if;

				-- propagate previous command to ppu during second byte of current command
				if i = 23 then WEN <= '1'; end if;
				if i = 15 then WEN <= '0'; end if;
			end if;
		DBG_I <= i;
		end if;
	end process;
end Behavioral;
