library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity apu_note_to_frequency is
    port (
        -- clk : in std_logic;
        -- rst : in std_logic;
        data : in std_logic_vector(7 downto 0);
        freq : out std_logic_vector(11 downto 0) --frequency
    );
end entity;

architecture Behavioral of apu_note_to_frequency is
signal buffSmall : std_logic_vector(7 downto 0) := (others => '0');
signal buff : std_logic_vector(15 downto 0) := (others => '0');
signal shift : integer;
begin

    shift <= to_integer(unsigned( data(2 downto 0) ));

    buffSmall <= 
        x"F0" when data(7 downto 3) = (x"C" & '0') else -- C  496
        x"D0" when data(7 downto 3) = (x"C" & '1') else -- C# 464
        x"B0" when data(7 downto 3) = (x"D" & '0') else -- D  432
        x"A0" when data(7 downto 3) = (x"D" & '1') else -- D# 416
        x"80" when data(7 downto 3) = (x"E" & '0') else -- E  384
        x"70" when data(7 downto 3) = (x"F" & '0') else -- F  368
        x"58" when data(7 downto 3) = (x"F" & '1') else -- F# 344
        x"40" when data(7 downto 3) = (x"8" & '0') else -- G  320
        x"30" when data(7 downto 3) = (x"8" & '1') else -- G# 304
        x"20" when data(7 downto 3) = (x"A" & '0') else -- A  288
        x"10" when data(7 downto 3) = (x"A" & '1') else -- A# 272
        x"00" when data(7 downto 3) = (x"B" & '0') else -- B  256
        x"01";

    buff <= x"1" & buffSmall;
    freq <= (others => '0') & buff(15 downto shift); -- bitshift values out (or div by powers of 2)

end architecture;