library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

library UNISIM;
use UNISIM.VComponents.all;

entity apu_tb_note_to_frequency is
end entity;

architecture Behavioral of apu_tb_note_to_frequency is

    component apu_note_to_frequency is
        port (
            data : in std_logic_vector(7 downto 0);
            freq : out std_logic_vector(11 downto 0) --frequency
        );
    end component;

    signal data : std_logic_vector(7 downto 0) := (others => '0');
    signal freq : std_logic_vector(11 downto 0) := (others => '0');

    signal OK : boolean := false;

begin
    UUT: apu_note_to_frequency
    port map (
        data => data,
        freq => freq
    );
    
    TB: process
    begin
        for I in 0 to 255 loop
            data <= std_logic_vector(to_unsigned(I, 8));
            wait for 4 ps;
        end loop;
    end process;

end architecture;