library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.ppu_consts.all;

entity top is port (
	SYSCLK : in std_logic; -- clock basys3 100MHz
	RESET : in std_logic; -- global (async) system reset
	SPI_CLK : in std_logic; -- incoming clock of SPI 
	SPI_MOSI : in std_logic; -- incoming data of SPI
	SPI_CS : in std_logic;   -- incoming select of SPI
	WEN : in std_logic; -- PPU VRAM write enable
	R,G,B : out std_logic_vector(PPU_COLOR_OUTPUT_DEPTH-1 downto 0);
	NVSYNC, NHSYNC : out std_logic; -- native VGA out
	TVBLANK, THBLANK : out std_logic); -- tiny VGA out
end top;

architecture Behavioral of top is
	component ppu port(
		CLK100 : in std_logic; -- system clock
		RESET : in std_logic; -- global (async) system reset
		EN : in std_logic; -- PPU VRAM enable (enable ADDR and DATA tri-state drivers)
		WEN : in std_logic; -- PPU VRAM write enable
		ADDR : in std_logic_vector(PPU_RAM_BUS_ADDR_WIDTH-1 downto 0); -- PPU VRAM ADDR
		DATA : in std_logic_vector(PPU_RAM_BUS_DATA_WIDTH-1 downto 0);
		R,G,B : out std_logic_vector(PPU_COLOR_OUTPUT_DEPTH-1 downto 0);
		NVSYNC, NHSYNC : out std_logic; -- native VGA out
		TVBLANK, THBLANK : out std_logic); -- tiny VGA out
	end component;
	component spi port (
		SYSCLK : in std_logic; -- clock basys3 100MHz
		SPI_CLK : in std_logic; -- incoming clock of SPI 
		SPI_MOSI : in std_logic; -- incoming data of SPI
		SPI_CS : in std_logic;   -- incoming select of SPI
		DATA : out std_logic_vector(PPU_RAM_BUS_ADDR_WIDTH+PPU_RAM_BUS_DATA_WIDTH-1 downto 0)); --  data read
	end component;

	signal SPI_DATA : std_logic_vector(PPU_RAM_BUS_ADDR_WIDTH+PPU_RAM_BUS_DATA_WIDTH-1 downto 0);
	alias SPI_DATA_ADDR is SPI_DATA(31 downto 16);
	alias SPI_DATA_DATA is SPI_DATA(15 downto 0);
begin
	serial_peripheral_interface: component spi port map(
		SYSCLK => SYSCLK,
		SPI_CLK => SPI_CLK,
		SPI_MOSI => SPI_MOSI,
		SPI_CS => '1',
		DATA => SPI_DATA);

	picture_processing_unit: component ppu port map(
		CLK100 => SYSCLK,
		RESET => RESET,
		EN => '1',
		WEN => WEN,
		ADDR => SPI_DATA_ADDR,
		DATA => SPI_DATA_DATA,
		R => R,
		G => G,
		B => B,
		NVSYNC => NVSYNC,
		NHSYNC => NHSYNC,
		TVBLANK => TVBLANK,
		THBLANK => THBLANK);
end Behavioral;
