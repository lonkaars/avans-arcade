library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.std_logic_unsigned.all;
use work.ppu_consts.all;

entity spi is port (
	SYSCLK : in std_logic; -- clock basys3 100MHz
	SPI_CLK : in std_logic; -- incoming clock of SPI 
	SPI_MOSI : in std_logic; -- incoming data of SPI
	SPI_CS : in std_logic;   -- incoming select of SPI
	DATA : out std_logic_vector(PPU_RAM_BUS_ADDR_WIDTH+PPU_RAM_BUS_DATA_WIDTH-1 downto 0)); --  data read
end spi;

architecture Behavioral of spi is
	signal PulseFF0,PulseFF1,PulseFF2,PulseFF3 : std_logic := '0';	 -- signal for metastability synchronizer of clk SPI
	signal dataFF0,dataFF1,dataFF2,dataFF3 : std_logic := '0';		  -- signal for metastability synchronizer of data SPI
	signal ssFF0,ssFF1,ssFF2,ssFF3 : std_logic := '0';				  -- signal for metastability synchronizer of slave select SPI
	
	signal SPI_REG : std_logic_vector(PPU_RAM_BUS_ADDR_WIDTH+PPU_RAM_BUS_DATA_WIDTH-1 downto 0) := (others => '0');	  -- signal to store incomming data of dataSPI (2x 8bit)
	signal counter : integer := 23;									  -- counter for data position
	signal enable : std_logic := '0';								   -- enable signal if slave is selected
begin
	
	process (SYSCLK)
	begin
		if rising_edge(SYSCLK) then	   
			-- flip flop for clk SPI to synchronise a
			PulseFF0 <= SPI_CLK;
			PulseFF1 <= PulseFF0;
			PulseFF2 <= PulseFF1;
			PulseFF3 <= PulseFF2;  
			-- flip flop for data SPI to synchronise	  
			dataFF0 <= SPI_MOSI;
			dataFF1 <= dataFF0;
			dataFF2 <= dataFF1;
			dataFF3 <= dataFF2;			 
			 -- flip flop for slave select SPI to synchronise				  
			ssFF0 <= SPI_CS;
			ssFF1 <= ssFF0;
			ssFF2 <= ssFF1;
			ssFF3 <= ssFF2; 
			-- check if slave select signal has falling edge (slave is selected by master)	
			if(ssFF3 = '1' and ssFF2 = '0') then
			-- reset counter if true
				counter <= 23;
			-- disable data read if rising edge (slave is not selected)	
			elsif (ssFF3 = '0' and ssFF2 = '1') then
				enable <= '0';
			end if;			
			-- check if synchronised slave select signal is falling edge or data read is enabled
			if(ssFF3 = '1' and ssFF2 = '0') or enable = '1' then
				enable <= '1';										  -- enable data read
				if (PulseFF3 = '0' and PulseFF2 = '1') then			 -- check for rising edge of clk SPI
					if counter > -1 then								
						counter <= counter - 1;
						-- data transfer into vector  
						SPI_REG(counter) <= dataFF3;
					end if;
				end if;
				-- check if counter is done
				if counter = -1 then
					counter <= 23;	   -- reset counter 
					DATA <= SPI_REG;			 
				end if;	  
			elsif (enable = '0') then
				-- DATA <= SPI_REG;
			end if;
		end if;
	end process;
end Behavioral;
