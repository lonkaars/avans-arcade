-- vim: ft=vhdl
library ieee;
library unisim;
use ieee.std_logic_1164.all;
use work.ppu_consts.all;
use unisim.vcomponents.all;

entity ppu_tb is
end ppu_tb;

architecture behavioral of ppu_tb is
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
	signal CLK100, RESET, WEN : std_logic := '0';
	signal ADDR : std_logic_vector(PPU_RAM_BUS_ADDR_WIDTH-1 downto 0);
	signal DATA : std_logic_vector(PPU_RAM_BUS_DATA_WIDTH-1 downto 0);
begin
	uut : ppu port map (
		CLK100 => CLK100,
		RESET => RESET,
		WEN => WEN,
		ADDR => ADDR,
		DATA => DATA,
		R => open,
		G => open,
		B => open,
		VSYNC => open,
		HSYNC => open,
		VBLANK => open);

	process
	begin
		for i in 0 to 3200000 loop
			wait for 5 ns;
			CLK100 <= '1';
			wait for 5 ns;
			CLK100 <= '0';
		end loop;
		wait; -- stop for simulator
	end process;
	
	process
	begin
		RESET <= '1';
		wait for 50 ns;
		RESET <= '0';
		-- undivert(`test-foreground-sprite-ppu.tb.vhd') -- m4 macro expansion (see makefile)
		wait; -- stop after one loop (process loops in simulator)
	end process;
end Behavioral;
