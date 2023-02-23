----------------------------------------------------------------------------------
-- Company: 
-- Engineer: 
-- 
-- Create Date: 23.02.2023 11:03:27
-- Design Name: 
-- Module Name: ppu_plut - Behavioral
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
use IEEE.NUMERIC_STD.ALL;
use ieee.std_logic_unsigned.all;
-- Uncomment the following library declaration if instantiating
-- any Xilinx leaf cells in this code.
--library UNISIM;
--use UNISIM.VComponents.all;

entity ppu_plut is
  Port ( 
        CLK: in std_logic; -- system clock
		CIDX: in std_logic_vector(PPU_PALETTE_CIDX_WIDTH-1 downto 0); -- color in
		RESET: in std_logic;

		-- internal memory block (AUX)
		PAL_WEN: in std_logic; -- VRAM PAL write enable
		PAL_ADDR: in std_logic_vector(PPU_PAL_ADDR_WIDTH-1 downto 0); -- VRAM PAL address
		PAL_DATA: in std_logic_vector(PPU_PAL_DATA_WIDTH-1 downto 0); -- VRAM PAL data
		
		R,G,B: out std_logic_vector(PPU_COLOR_OUTPUT_DEPTH-1 downto 0)); -- VGA color out
end ppu_plut;

architecture Behavioral of ppu_plut is
	component er_ram
		generic(
			ADDR_W : natural := PPU_PAL_ADDR_WIDTH; -- ADDR line width
			DATA_W : natural := PPU_PAL_DATA_WIDTH; -- DATA line width
			ADDR_LOW : natural := 16#0000#; -- starting address
			ADDR_RANGE : natural := 16#0040#); -- amount of valid addresses after ADDR_LOW
		port(
			CLK : in std_logic; -- clock
			RST : in std_logic; -- async memory clear
			WEN : in std_logic; -- write enable
			ADDR : in std_logic_vector(ADDR_W-1 downto 0);
			DATA : in std_logic_vector(DATA_W-1 downto 0);
			REG : out std_logic_vector((ADDR_RANGE*DATA_W)-1 downto 0)); -- exposed register output
	end component;
	
	SIGNAL PLUT : std_logic_vector((64 * PPU_PAL_DATA_WIDTH)-1 downto 0) := (others => '0');
	SIGNAL CHECK_ZERO_CIDX   :  std_logic_vector(PPU_PALETTE_CIDX_WIDTH-1 downto 0) := (others => '0');-- color in
begin
	RAM : component er_ram port map(
		CLK => CLK,
		RST => RESET,
		WEN => PAL_WEN,
		ADDR => PAL_ADDR,
		DATA => PAL_DATA,
		REG => PLUT);


    process(CLK,RESET)
    	VARIABLE  COLOR   :  std_logic_vector(PPU_PAL_DATA_WIDTH-1 downto 0) := (others => '0');-- COLORS RGB IN
    	VARIABLE CIDX_INT : INTEGER := 0;
    begin
        IF(RESET = '1') THEN
            PLUT <= (others => '0');
        ELSE
            IF rising_edge (CLK) THEN
                IF (CIDX /= CHECK_ZERO_CIDX) THEN
                    CIDX_INT := TO_INTEGER(UNSIGNED(CIDX));
                    COLOR := PLUT((12 * CIDX_INT) + 11 DOWNTO (12*CIDX_INT));
                    R <= COLOR(11 DOWNTO  8);
                    G <= COLOR(7  DOWNTO  4);
                    B <= COLOR(3  DOWNTO  0);
                ELSE
                    R <= X"0";
                    G <= X"0";
                    B <= X"0";
                END IF;
            END IF;
        END IF;
    end process;

end Behavioral;
