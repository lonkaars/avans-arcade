library ieee;
library unisim;
library work;

use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use unisim.vcomponents.all;
use work.ppu_consts.all;

entity ppu_addr_dec_tb is
end ppu_addr_dec_tb;

architecture behavioral of ppu_addr_dec_tb is
	component ppu_addr_dec port(
		WEN : in std_logic; -- EXT write enable
		TMM_WEN,
		BAM_WEN,
		FAM_WEN,
		PAL_WEN,
		AUX_WEN : out std_logic; -- write enable MUX
		ADDR : in std_logic_vector(PPU_RAM_BUS_ADDR_WIDTH-1 downto 0); -- address in
		TMM_ADDR : out std_logic_vector(PPU_TMM_ADDR_WIDTH-1 downto 0);
		BAM_ADDR : out std_logic_vector(PPU_BAM_ADDR_WIDTH-1 downto 0);
		FAM_ADDR : out std_logic_vector(PPU_FAM_ADDR_WIDTH-1 downto 0);
		PAL_ADDR : out std_logic_vector(PPU_PAL_ADDR_WIDTH-1 downto 0);
		AUX_ADDR : out std_logic_vector(PPU_AUX_ADDR_WIDTH-1 downto 0));
	end component;
	signal WEN : std_logic;
	signal TMM_WEN, BAM_WEN, FAM_WEN, PAL_WEN, AUX_WEN : std_logic;
	signal ADDR : std_logic_vector(PPU_RAM_BUS_ADDR_WIDTH-1 downto 0);
	signal TMM_ADDR : std_logic_vector(PPU_TMM_ADDR_WIDTH-1 downto 0);
	signal BAM_ADDR : std_logic_vector(PPU_BAM_ADDR_WIDTH-1 downto 0);
	signal FAM_ADDR : std_logic_vector(PPU_FAM_ADDR_WIDTH-1 downto 0);
	signal PAL_ADDR : std_logic_vector(PPU_PAL_ADDR_WIDTH-1 downto 0);
	signal AUX_ADDR : std_logic_vector(PPU_AUX_ADDR_WIDTH-1 downto 0);
begin
	uut : ppu_addr_dec port map(
    WEN => WEN,
    TMM_WEN => TMM_WEN,
    BAM_WEN => BAM_WEN,
    FAM_WEN => FAM_WEN,
    PAL_WEN => PAL_WEN,
    AUX_WEN => AUX_WEN,
    ADDR => ADDR,
    TMM_ADDR => TMM_ADDR,
    BAM_ADDR => BAM_ADDR,
    FAM_ADDR => FAM_ADDR,
    PAL_ADDR => PAL_ADDR,
    AUX_ADDR => AUX_ADDR);

  WEN <= '1';

	tb : process
	begin
		for i in 0 to 65535 loop
      ADDR <= std_logic_vector(to_unsigned(i,16));
      wait for 10 ps;
		end loop;
		wait; -- stop for simulator
	end process;
end;
