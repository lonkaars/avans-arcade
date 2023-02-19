library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity apu_note_to_frequency is port (
	-- clk : in std_logic;
	-- rst : in std_logic;
	data : in std_logic_vector(7 downto 0);
	freq : out std_logic_vector(11 downto 0)); --frequency
end entity;

architecture Behavioral of apu_note_to_frequency is
	signal buff_small: std_logic_vector(7 downto 0) := (others => '0');
	signal buff: std_logic_vector(15 downto 0) := (others => '0');
	signal shift: integer;
begin
	shift <= to_integer(unsigned(data(2 downto 0)));
	buff_small <= 
		x"f0" when data(7 downto 3) = (x"c" & '0') else -- C	496
		x"d0" when data(7 downto 3) = (x"c" & '1') else -- C# 464
		x"b0" when data(7 downto 3) = (x"d" & '0') else -- D	432
		x"a0" when data(7 downto 3) = (x"d" & '1') else -- D# 416
		x"80" when data(7 downto 3) = (x"e" & '0') else -- E	384
		x"70" when data(7 downto 3) = (x"f" & '0') else -- F	368
		x"58" when data(7 downto 3) = (x"f" & '1') else -- F# 344
		x"40" when data(7 downto 3) = (x"8" & '0') else -- G	320
		x"30" when data(7 downto 3) = (x"8" & '1') else -- G# 304
		x"20" when data(7 downto 3) = (x"a" & '0') else -- A	288
		x"10" when data(7 downto 3) = (x"a" & '1') else -- A# 272
		x"00" when data(7 downto 3) = (x"b" & '0') else -- B	256
		x"01";

	buff <= x"1" & buff_small;
	freq <= (others => '0') & buff(15 downto shift); -- bitshift values out (or div by powers of 2)
end architecture;
