library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_unsigned.all;
use work.ppu_consts.all;

entity spi is port (
	SYSCLK : in std_logic; -- system clock (100MHz)
	RESET : in std_logic; -- async reset
	DCK : in std_logic; -- data clock (spi format)
	DI : in std_logic; -- data in (spi format)
	SR : in std_logic; -- sync reset (spi reset)
	DO : out std_logic_vector(PPU_RAM_BUS_ADDR_WIDTH+PPU_RAM_BUS_DATA_WIDTH-1 downto 0) := (others => '1'); --data out (parallel)
	WEN : out std_logic := '0'); -- write enable (triggers during each word to propagate previous word)
end spi;

architecture Behavioral of spi is
	signal clkFF0,clkFF1,clkFF2,clkFF3 : std_logic := '0'; -- metastability
	signal dataFF0,dataFF1,dataFF2 : std_logic := '0'; -- metastability
	signal srFF0,srFF1,srFF2 : std_logic := '0'; -- metastability

	constant COUNTER_RESET_VALUE : integer := PPU_RAM_BUS_ADDR_WIDTH + PPU_RAM_BUS_DATA_WIDTH - 1;
begin
	process (SYSCLK)
		variable i : integer range 0 to COUNTER_RESET_VALUE := COUNTER_RESET_VALUE; -- received bits counter
		variable data_r : std_logic_vector(PPU_RAM_BUS_ADDR_WIDTH+PPU_RAM_BUS_DATA_WIDTH-1 downto 0) := (others => '1'); -- data register
	begin
		if RESET = '1' then
			data_r := (others => '1');
			i := COUNTER_RESET_VALUE;
			DO <= (others => '1');
			WEN <= '0';
			clkFF0 <= '0';
			clkFF1 <= '0';
			clkFF2 <= '0';
			clkFF3 <= '0';
			dataFF0 <= '0';
			dataFF1 <= '0';
			dataFF2 <= '0';
			srFF0 <= '0';
			srFF1 <= '0';
			srFF2 <= '0';
		elsif rising_edge(SYSCLK) then
			clkFF0 <= DCK;
			clkFF1 <= clkFF0;
			clkFF2 <= clkFF1;
			clkFF3 <= clkFF2;
			dataFF0 <= DI;
			dataFF1 <= dataFF0;
			dataFF2 <= dataFF1;
			srFF0 <= SR;
			srFF1 <= srFF0;
			srFF2 <= srFF1;

			if (clkFF3 = '0' and clkFF2 = '1') then
				-- data_r(i) := dataFF2;
				data_r := data_r(data_r'high-1 downto data_r'low) & dataFF2;

				if i = 0 then
					i := COUNTER_RESET_VALUE;
					DO <= data_r;
				else
					i := i - 1;
				end if;

				-- propagate previous command to ppu during second byte of current command
				if i = 23 then WEN <= '1'; end if;
				if i = 15 then WEN <= '0'; end if;
			end if;
			if srFF2 = '1' then
				data_r := (others => '1');
				i := COUNTER_RESET_VALUE;
				DO <= (others => '1');
				WEN <= '0';
				clkFF0 <= '0';
				clkFF1 <= '0';
				clkFF2 <= '0';
				clkFF3 <= '0';
				dataFF0 <= '0';
				dataFF1 <= '0';
				dataFF2 <= '0';
				srFF0 <= '0';
				srFF1 <= '0';
				srFF2 <= '0';
			end if;
		end if;
	end process;
end Behavioral;
