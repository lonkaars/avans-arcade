----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 22.02.2023 13:13:03
-- Design Name: 
-- Module Name: ppu_vga_tiny - Behavioral
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
use work.ppu_consts.all;

use IEEE.NUMERIC_STD.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;
-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity ppu_vga_tiny is
  Port ( 
  		CLK: in std_logic; -- system clock
		RESET: in std_logic;

		X: out std_logic_vector(PPU_POS_H_WIDTH-1 downto 0); -- current screen pixel x
		Y: out std_logic_vector(PPU_POS_V_WIDTH-1 downto 0); -- current screen pixel y
		
		VSYNC, VBLANK, 
		HSYNC, HBLANK: out std_logic); -- VGA sync outputs 

end ppu_vga_tiny;

architecture Behavioral of ppu_vga_tiny is
  signal hcount: STD_LOGIC_VECTOR(PPU_POS_H_WIDTH-1 downto 0):= (others => '0');
  signal vcount: STD_LOGIC_VECTOR(PPU_POS_V_WIDTH-1 downto 0):= (others => '0');
  
begin
process (CLK) 
begin
    if rising_edge(CLK) then
           -- x,y data uit
            X <= hcount;
            Y <= vcount;
         --pulse width
          if hcount < 32 then
            hsync <= '0';
          else
            hsync <= '1';
          end if;
    
          if vcount < 8 then
            vsync <= '0';
          else
            vsync <= '1';
          end if;
         -- sync pulse time
          hcount <= hcount + 1;
         
          if hcount = 400 then
            vcount <= vcount + 1;
            hcount <= (others => '0');
          end if;
         
          if vcount = 255 then	   
            vcount <= (others => '0');
         end if;
    end if;
end process;

end Behavioral;
