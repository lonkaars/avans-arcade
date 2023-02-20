library ieee;
library work;
library unisim;

use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use unisim.vcomponents.all;
use work.ppu_consts.all;

entity ppu_sprite_bg_tb is
end ppu_sprite_bg_tb;

architecture Behavioral of ppu_sprite_bg_tb is
	component ppu_sprite_bg port(
		-- inputs
		CLK : in std_logic; -- pipeline clock
		RESET : in std_logic; -- reset clock counter
		OE : in std_logic; -- output enable (of CIDX)
		X : in std_logic_vector(PPU_POS_H_WIDTH-1 downto 0); -- current screen pixel x
		Y : in std_logic_vector(PPU_POS_V_WIDTH-1 downto 0); -- current screen pixel y
		
		-- aux inputs
		BG_SHIFT_X : in std_logic_vector(PPU_POS_H_WIDTH-1 downto 0);
		BG_SHIFT_Y : in std_logic_vector(PPU_POS_V_WIDTH-1 downto 0);

		-- used memory blocks
		BAM_ADDR : out std_logic_vector(PPU_BAM_ADDR_WIDTH-1 downto 0);
		BAM_DATA : in std_logic_vector(PPU_BAM_DATA_WIDTH-1 downto 0);
		TMM_ADDR : out std_logic_vector(PPU_TMM_ADDR_WIDTH-1 downto 0);
		TMM_DATA : in std_logic_vector(PPU_TMM_DATA_WIDTH-1 downto 0);

		-- outputs
		CIDX : out std_logic_vector(PPU_PALETTE_CIDX_WIDTH-1 downto 0)); -- output color
	end component;
	signal CLK : std_logic := '0'; -- pipeline clock
	signal RESET : std_logic := '0'; -- reset clock counter
	signal OE : std_logic := '0'; -- output enable (of CIDX)
	signal X : std_logic_vector(PPU_POS_H_WIDTH-1 downto 0) := (others => '0'); -- current screen pixel x
	signal Y : std_logic_vector(PPU_POS_V_WIDTH-1 downto 0) := (others => '0'); -- current screen pixel y
	signal BG_SHIFT_X : std_logic_vector(PPU_POS_H_WIDTH-1 downto 0) := (others => '0');
	signal BG_SHIFT_Y : std_logic_vector(PPU_POS_V_WIDTH-1 downto 0) := (others => '0');
	signal BAM_ADDR : std_logic_vector(PPU_BAM_ADDR_WIDTH-1 downto 0);
	signal BAM_DATA : std_logic_vector(PPU_BAM_DATA_WIDTH-1 downto 0) := (others => '0');
	signal TMM_ADDR : std_logic_vector(PPU_TMM_ADDR_WIDTH-1 downto 0);
	signal TMM_DATA : std_logic_vector(PPU_TMM_DATA_WIDTH-1 downto 0) := (others => '0');
	signal CIDX : std_logic_vector(PPU_PALETTE_CIDX_WIDTH-1 downto 0); -- output color
begin
	uut : ppu_sprite_bg port map(
		CLK => CLK,
		RESET => RESET,
		OE => OE,
		X => X,
		Y => Y,
		BG_SHIFT_X => BG_SHIFT_X,
		BG_SHIFT_Y => BG_SHIFT_Y,
		BAM_ADDR => BAM_ADDR,
		BAM_DATA => BAM_DATA,
		TMM_ADDR => TMM_ADDR,
		TMM_DATA => TMM_DATA,
		CIDX => CIDX);

	BAM_DATA <= std_logic_vector(to_unsigned(16#4814#, PPU_BAM_DATA_WIDTH));  -- hex((1 << 14) | (0 << 13) | (2 << 10) | (20 << 0))
	TMM_DATA <= std_logic_vector(to_unsigned(16#ffff#, PPU_TMM_DATA_WIDTH));
	X <= std_logic_vector(to_unsigned(25, PPU_POS_H_WIDTH));
	Y <= std_logic_vector(to_unsigned(60, PPU_POS_V_WIDTH));
	BG_SHIFT_X <= std_logic_vector(to_unsigned(3, PPU_POS_H_WIDTH));
	BG_SHIFT_Y <= std_logic_vector(to_unsigned(3, PPU_POS_V_WIDTH));

	tb : process
	begin
		for i in 0 to 32 loop
			if i > 10 then
				OE <= '1';
			end if;
			if i > 20 then
				RESET <= '1';
			end if;
	
			wait for 5 ns;
			CLK <= '1';
			wait for 5 ns;
			CLK <= '0';
		end loop;
		wait; -- stop for simulator
	end process;
end Behavioral;
