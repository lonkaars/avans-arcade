----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 23.02.2023 12:23:00
-- Design Name: 
-- Module Name: PPU_COMP_TB - Behavioral
-- Project Name: 
-- Target Devices: 
-- Tool Versions: 
-- Description: 
-- 
-- Dependencies: 
-- 
-- Revision:
-- Revision 0.01 - File Created
-- Additional Comments:
-- 
----------------------------------------------------------------------------------


library IEEE;
use IEEE.STD_LOGIC_1164.ALL;

-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
library UNISIM;
use work.ppu_consts.all;
use UNISIM.VComponents.all;

entity PPU_COMP_TB is

end PPU_COMP_TB;

architecture Behavioral of PPU_COMP_TB is
COMPONENT ppu_comp
        port (
 		FG_HIT: in std_logic_vector(PPU_FG_SPRITE_COUNT-1 downto 0);
		BG_EN: out std_logic;
		FG_EN: out std_logic_vector(PPU_FG_SPRITE_COUNT-1 downto 0));
end component;

signal  FG_HIT: std_logic_vector(PPU_FG_SPRITE_COUNT-1 downto 0) := (others => '0');
signal	BG_EN: std_logic  := '0';
signal	FG_EN: std_logic_vector(PPU_FG_SPRITE_COUNT-1 downto 0) := (others => '0');
begin
UUT : ppu_comp port map (
FG_HIT => FG_HIT,
BG_EN  => BG_EN,
FG_EN  => FG_EN
);
 TB : PROCESS
 BEGIN
    FG_HIT <= (OTHERS => '0');
    wait for 1 ps;
    FG_HIT(6) <= '1';
    FG_HIT(5) <= '1';
    FG_HIT(100) <= '1';
    wait for 1 ps;
    
    FG_HIT(0) <= '1';
    wait for 1 ps;
    FG_HIT <= (OTHERS => '0');
    wait for 1 ps;
    
    
    wait;
 END PROCESS;
end Behavioral;
