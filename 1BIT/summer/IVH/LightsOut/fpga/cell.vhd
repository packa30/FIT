----------------------------------------------------------------------------------
-- Engineer: 
----------------------------------------------------------------------------------
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use work.maska_pack.ALL; -- vysledek z prvniho ukolu


entity cell is
   GENERIC (
      MASK              : mask_t := (others => '1') -- mask_t definovano v baliku math_pack
   );
   Port ( 
      INVERT_REQ_IN     : in   STD_LOGIC_VECTOR (3 downto 0);
      INVERT_REQ_OUT    : out  STD_LOGIC_VECTOR (3 downto 0);
      
      KEYS              : in   STD_LOGIC_VECTOR (4 downto 0);
      
      SELECT_REQ_IN     : in   STD_LOGIC_VECTOR (3 downto 0);
      SELECT_REQ_OUT    : out  STD_LOGIC_VECTOR (3 downto 0);
      
      INIT_ACTIVE       : in   STD_LOGIC;
      ACTIVE            : out  STD_LOGIC;
      
      INIT_SELECTED     : in   STD_LOGIC;
      SELECTED          : out  STD_LOGIC;

      CLK               : in   STD_LOGIC;
      RESET             : in   STD_LOGIC
   );
end cell;

architecture Behavioral of cell is
  constant IDX_TOP    : NATURAL := 0; -- index sousedni bunky nachazejici se nahore v signalech *_REQ_IN a *_REQ_OUT, index klavesy posun nahoru v KEYS
  constant IDX_LEFT   : NATURAL := 1; -- ... totez        ...                vlevo
  constant IDX_RIGHT  : NATURAL := 2; -- ... totez        ...                vpravo
  constant IDX_BOTTOM : NATURAL := 3; -- ... totez        ...                dole
  
  constant IDX_ENTER  : NATURAL := 4; -- index klavesy v KEYS, zpusobujici inverzi bunky (enter, klavesa 5)
  
  signal SEL_OUT : STD_LOGIC_VECTOR (3 downto 0):=(others => '0');
  signal INV_OUT : STD_LOGIC_VECTOR (3 downto 0):=(others => '0');
  signal KEYS_Q :STD_LOGIC_VECTOR (4 downto 0):=(others => '0');
  signal ACT : STD_LOGIC ;
  signal SEL : STD_LOGIC ;
  
begin
	SELECTED <= SEL;
	ACTIVE <= ACT;
	INVERT_REQ_OUT <= INV_OUT ;
	SELECT_REQ_OUT <= SEL_OUT ;
	process(CLK,RESET)
		begin
			
			if RESET='1' then
				ACT <= INIT_ACTIVE;
				SEL <= INIT_SELECTED;		
			elsif (CLK'event and CLK='1') then
				
				if SEL = '1' then
					if KEYS_Q /= KEYS then
					
						if KEYS = "00000" then
							SEL_OUT <= "0000";
							INV_OUT <= "0000";
						
						elsif KEYS(IDX_TOP)='1' and MASK.TOP='1' then
							SEL_OUT(IDX_TOP) <= '1';
							SEL <= '0';				
						elsif KEYS(IDX_LEFT)='1' and MASK.LEFT='1' then			
							SEL_OUT(IDX_LEFT) <= '1';
							SEL <= '0';
						elsif KEYS(IDX_RIGHT)='1' and MASK.RIGHT='1' then			
							SEL_OUT(IDX_RIGHT) <= '1';
							SEL <= '0';				
						elsif KEYS(IDX_BOTTOM)='1' and MASK.BOTTOM='1' then			
							SEL_OUT(IDX_BOTTOM) <= '1';
							SEL <= '0';				
						elsif  KEYS(IDX_ENTER)='1' then			
							ACT <= not ACT;
							if MASK.TOP='1' then
								INV_OUT(IDX_TOP) <= '1';
							end if;
							if MASK.LEFT='1' then
								INV_OUT(IDX_LEFT) <= '1';
							end if;
							if MASK.RIGHT='1' then
								INV_OUT(IDX_RIGHT) <= '1';
							end if;
							if MASK.BOTTOM='1' then
								INV_OUT(IDX_BOTTOM) <= '1';
							end if;			
						end if;
					end if;
					KEYS_Q <= KEYS;
				elsif SEL = '0' then
							SEL_OUT <= "0000";
							INV_OUT <= "0000";
				end if;
				if SELECT_REQ_IN/="0000" then
					SEL <= '1';
				elsif INVERT_REQ_IN/="0000" then
					ACT <= not ACT;
			end if;
		
	end if;

end process;

end Behavioral;

