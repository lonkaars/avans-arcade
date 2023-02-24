----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 23.02.2023 10:31:25
-- Design Name: 
-- Module Name: ppu_comp - Behavioral
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
-- Uncomment the following library declaration if using
-- arithmetic functions with Signed or Unsigned values
--use IEEE.NUMERIC_STD.ALL;

-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity ppu_comp is
 Port ( 
 		FG_HIT: in std_logic_vector(PPU_FG_SPRITE_COUNT-1 downto 0);
		BG_EN: out std_logic;
		FG_EN: out std_logic_vector(PPU_FG_SPRITE_COUNT-1 downto 0));
		
end ppu_comp;

architecture Behavioral of ppu_comp is
signal FG_HIT_Empty : std_logic_vector(PPU_FG_SPRITE_COUNT-1 downto 0) := (others => '0');
begin
    process (FG_HIT)
    variable HIT : BOOLEAN := FALSE;
    begin
        -- check if fg_hit is not empty
        if FG_HIT /= FG_HIT_Empty then
            BG_EN <= '0';
            FOR I IN 0 TO PPU_FG_SPRITE_COUNT-1 LOOP
                -- if fg_hit is the first one then enable it
                IF(FG_HIT(I) = '1' AND HIT = FALSE) THEN
                    FG_EN(I) <= '1';
                    HIT := TRUE;
                ELSE
                    -- make rest low
                    FG_EN(I) <= '0';
                END IF;
            END LOOP;
            HIT := FALSE;
        else
            BG_EN <= '1';
            FG_EN <= (others => '0');
        end if;
    end process;
end Behavioral;
