library ieee;
use ieee.std_logic_1164.all;
use work.ppu_consts.all;

entity ppu_comp is port (
	OE : in std_logic; -- global output enable (screen active)
	FG_HIT : in std_logic_vector(PPU_FG_SPRITE_COUNT-1 downto 0); -- foreground hit array
	BG_EN : out std_logic; -- background enable output
	FG_EN : out std_logic_vector(PPU_FG_SPRITE_COUNT-1 downto 0)); -- foreground enable output
end ppu_comp;

architecture Behavioral of ppu_comp is
	signal FG_HIT_EMPTY : std_logic_vector(PPU_FG_SPRITE_COUNT-1 downto 0) := (others => '0');
	signal TMP_BG_EN : std_logic; -- background enable output
	signal TMP_FG_EN : std_logic_vector(PPU_FG_SPRITE_COUNT-1 downto 0); -- foreground enable output
begin
	BG_EN <= TMP_BG_EN and OE;
	FG_EN <= TMP_FG_EN and OE;

	process (FG_HIT)
		variable HIT : boolean := false;
	begin
		-- check if FG_HIT is not empty
		if FG_HIT /= FG_HIT_EMPTY then
			TMP_BG_EN <= '0';
			for i in 0 to PPU_FG_SPRITE_COUNT-1 loop
				-- if FG_HIT is the first one then enable it
				if(FG_HIT(i) = '1' and HIT = false) then
					TMP_FG_EN(i) <= '1';
					HIT := true;
				else
					-- make rest low
					TMP_FG_EN(i) <= '0';
				end if;
			end loop;
			HIT := false;
		else
			TMP_BG_EN <= '1';
			TMP_FG_EN <= (others => '0');
		end if;
	end process;
end Behavioral;
