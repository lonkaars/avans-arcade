library ieee;
library unisim;

use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use unisim.vcomponents.all;
use work.ppu_consts.all;

entity ppu_dispctl_tb is
end ppu_dispctl_tb;

architecture behavioral of ppu_dispctl_tb is
	component ppu_dispctl port(
		CLK : in std_logic; -- system clock
		RESET : in std_logic;

		X : out std_logic_vector(PPU_POS_H_WIDTH-1 downto 0); -- tiny screen pixel x
		Y : out std_logic_vector(PPU_POS_V_WIDTH-1 downto 0); -- tiny screen pixel y
		RI,GI,BI : in std_logic_vector(PPU_COLOR_OUTPUT_DEPTH-1 downto 0); -- color in
		PREADY : in std_logic; -- current pixel ready (pixel color is stable)

		RO,GO,BO : out std_logic_vector(PPU_COLOR_OUTPUT_DEPTH-1 downto 0); -- VGA color out
		NVSYNC, NHSYNC : out std_logic; -- VGA sync out
		THBLANK, TVBLANK : out std_logic); -- tiny sync signals
	end component;
	signal CLK : std_logic := '0';
	signal RESET : std_logic := '0';
	signal X : std_logic_vector(PPU_POS_H_WIDTH-1 downto 0) := (others => '0');
	signal Y : std_logic_vector(PPU_POS_V_WIDTH-1 downto 0) := (others => '0');
	signal RI,GI,BI : std_logic_vector(PPU_COLOR_OUTPUT_DEPTH-1 downto 0) := (others => '0');
	signal PREADY : std_logic := '0';
	signal RO,GO,BO : std_logic_vector(PPU_COLOR_OUTPUT_DEPTH-1 downto 0) := (others => '0');
	signal NVSYNC, NHSYNC : std_logic := '0';
	signal THBLANK, TVBLANK : std_logic := '0';
begin
	uut : component ppu_dispctl port map(
		CLK => CLK,
		RESET => RESET,
		PREADY => PREADY,
		X => X,
		Y => Y,
		RI => RI,
		GI => GI,
		BI => BI,
		RO => RO,
		GO => GO,
		BO => BO,
		NVSYNC => NVSYNC,
		NHSYNC => NHSYNC,
		TVBLANK => TVBLANK,
		THBLANK => THBLANK);

	process
	begin
		for i in 0 to 3200000 loop
			wait for 5 ps;
			CLK <= '1';
			wait for 5 ps;
			CLK <= '0';
		end loop;
		wait; -- stop for simulator
	end process;
end;
