----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 22.02.2023 13:20:47
-- Design Name: 
-- Module Name: ppu_vga_native - Behavioral
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

entity ppu_vga_native is
 Port (	CLK: in std_logic; -- system clock
		RESET: in std_logic;

		X: in std_logic_vector(PPU_POS_H_WIDTH-1 downto 0); -- current screen pixel x
		Y: in std_logic_vector(PPU_POS_V_WIDTH-1 downto 0); -- current screen pixel y
		PREADY: in std_logic; -- current pixel ready (pixel color is stable)
		RI,GI,BI: in std_logic_vector(PPU_COLOR_OUTPUT_DEPTH-1 downto 0); -- VGA color in
		
		RO,GO,BO: out std_logic_vector(PPU_COLOR_OUTPUT_DEPTH-1 downto 0); -- VGA color out
		VSYNC, HSYNC: out std_logic); -- VGA sync outputs
end ppu_vga_native;

architecture Behavioral of ppu_vga_native is
  type line_buffer is array(319 downto 0) of std_logic_vector(11 downto 0);
  signal ram_x0 : line_buffer; -- buffer 0
  signal ram_x1: line_buffer; -- buffer 1
  signal hcount: STD_LOGIC_VECTOR(9 downto 0):= (others => '0');
  signal vcount: STD_LOGIC_VECTOR(9 downto 0):= (others => '0');
  signal rgb_out :  STD_LOGIC_VECTOR(11 downto 0):= (others => '0'); -- output colors
  signal px : integer; -- conversion for hcount
  signal py :integer; -- conversion for vcount
  signal bufferFilledOnbuffer0 : integer;
  signal bufferFilledOnbuffer1 :integer;
begin
    process (clk, x, y)
        variable v_x : integer ; -- integer to hold vector X
    begin
        if rising_edge(clk) then

              v_x := TO_INTEGER(unsigned(x) - 72);
              if(v_x >= 0 and v_x < 320 and PREADY = '1') then
                 if(y(0) = '0') then
                    ram_x0(v_x) <= RI & GI & BI;
                    if v_x = 319 then
                        bufferFilledOnbuffer0 <= TO_INTEGER(unsigned(y) - 14);
                    end if;
                else
                    ram_x1(v_x) <= RI & GI & BI;
                    if v_x = 319 then
                        bufferFilledOnbuffer1 <= TO_INTEGER(unsigned(y) - 14);
                    end if;
                end if;          
             end if;     
            -- T display(display data)     
            if (hcount >= 144) and (hcount < 784) and (vcount >= 31) and (vcount < 511) then
                px <= TO_INTEGER(unsigned(hcount) - 144); 
                py <= TO_INTEGER(unsigned(vcount) - 31);   
                if(bufferFilledonBuffer0 = (py/2)) then
                    rgb_out <= ram_x0(px/2);
                elsif(bufferFilledonbuffer1 = (py/2)) then
                    rgb_out <= ram_x1(px/2); 

                else
                    rgb_out <= (others => '0');

                end if;
            end if;
            -- pulse width
            hsync <= '1';
            if hcount < 97 then
                hsync <= '0';
            end if;

            vsync <= '1';
            if vcount < 3 then
                vsync <= '0';
            end if;

            -- sync pulse time
            hcount <= hcount + 1;

            if hcount = 800 then
                vcount <= vcount + 1;
                hcount <= (others => '0');
            end if;

            if vcount = 521 then
                vcount <= (others => '0');
            end if;

        end if;
        -- output colors
    RO <= rgb_out(11 downto 8);
    GO <= rgb_out(7 downto 4);
    BO <= rgb_out(3 downto 0);
    end process;

end Behavioral;
