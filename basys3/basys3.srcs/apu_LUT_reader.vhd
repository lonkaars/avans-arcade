library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity apu_LUT_reader is
    port (
        clk   : in std_logic;
        rst : in std_logic;
        freq : in std_logic_vector(11 downto 0);
        wave : in std_logic_vector(1 downto 0);
        value : out std_logic_vector(7 downto 0)
    );
end entity;

architecture Behavioral of apu_LUT_reader is

    signal idx : std_logic_vector(7 downto 0) := (others => '0');
    signal buf : unsigned := (others => '0');
    
begin

    process (clk)
    begin
        if rst = '1' then
            idx <= x"00";
            buf <= x"00";
        elsif rising_edge(clk) then
            --main code here

            if wave = "00" then     --Sawtooth
                value <= idx;
            elsif wave = "01" then  --Square
                if (signed(idx) < 128) then
                    value <= x"00";
                else
                    value <= x"FF";
                end if;
            elsif wave = "10" then  --Triangle
                value <= std_logic_vector( abs(signed(idx)-127)*2 );
            else--wave = "11" then  --Noise
                --implement noise function here:
                value <= x"80"; --remove this
            end if;



            if buf < unsigned(freq) then -- change frequency
                buf <= buf + 1;
            else
                buf <= x"00";
                if unsigned(idx) < 255 then --moves to next index value
                    idx <= std_logic_vector( unsigned(idx) + '1');
                else
                    idx <= x"00";
                end if;
            end if;

        end if;
    end process;

end architecture;