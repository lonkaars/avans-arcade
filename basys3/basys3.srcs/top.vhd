library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.ppu_consts.all;

entity top is port (
	SYSCLK : in std_logic; -- clock basys3 100MHz
	RESET : in std_logic; -- global (async) system reset
	SPI_CLK : in std_logic; -- incoming clock of SPI 
	SPI_MOSI : in std_logic; -- incoming data of SPI
	SPI_RESET : in std_logic; -- PPU VRAM write enable
	DBG_DISP_ADDR : in std_logic; -- display address/data switch (debug)
	DBG_LEDS_OUT : out std_logic_vector(15 downto 0); -- debug address/data output leds
	R,G,B : out std_logic_vector(PPU_COLOR_OUTPUT_DEPTH-1 downto 0);
	VSYNC, HSYNC : out std_logic; -- VGA sync out
	VBLANK : out std_logic); -- vblank for synchronization
end top;

architecture Behavioral of top is
	component ppu port(
		CLK100 : in std_logic; -- system clock
		RESET : in std_logic; -- global (async) system reset
		WEN : in std_logic; -- PPU VRAM write enable
		ADDR : in std_logic_vector(PPU_RAM_BUS_ADDR_WIDTH-1 downto 0); -- PPU VRAM ADDR
		DATA : in std_logic_vector(PPU_RAM_BUS_DATA_WIDTH-1 downto 0);
		R,G,B : out std_logic_vector(PPU_COLOR_OUTPUT_DEPTH-1 downto 0);
		VSYNC, HSYNC : out std_logic; -- VGA sync out
		VBLANK : out std_logic); -- vblank for synchronization
	end component;
	component spi port (
		SYSCLK : in std_logic; -- clock basys3 100MHz
		RESET : in std_logic; -- async reset
		DCK : in std_logic; -- data clock (spi format)
		DI : in std_logic; -- data in (spi format)
		DO : out std_logic_vector(PPU_RAM_BUS_ADDR_WIDTH+PPU_RAM_BUS_DATA_WIDTH-1 downto 0) := (others => '0'); --	data out (parallel)
		WEN : out std_logic); -- write enable (triggers during each word to propagate previous word)
	end component;

	signal SPI_RST, PPU_WEN : std_logic;
	signal SPI_DATA : std_logic_vector(PPU_RAM_BUS_ADDR_WIDTH+PPU_RAM_BUS_DATA_WIDTH-1 downto 0);
	alias SPI_DATA_ADDR is SPI_DATA(31 downto 16);
	alias SPI_DATA_DATA is SPI_DATA(15 downto 0);
begin
	SPI_RST <= RESET or SPI_RESET;
	serial_peripheral_interface: component spi port map(
		SYSCLK => SYSCLK,
		RESET => SPI_RST,
		DCK => SPI_CLK,
		DI => SPI_MOSI,
		DO => SPI_DATA,
		WEN => PPU_WEN);

	DBG_LEDS_OUT <= SPI_DATA_ADDR when DBG_DISP_ADDR = '1' else SPI_DATA_DATA;

	picture_processing_unit: component ppu port map(
		CLK100 => SYSCLK,
		RESET => RESET,
		WEN => PPU_WEN,
		ADDR => SPI_DATA_ADDR,
		DATA => SPI_DATA_DATA,
		R => R,
		G => G,
		B => B,
		VSYNC => VSYNC,
		HSYNC => HSYNC,
		VBLANK => VBLANK);
end Behavioral;
