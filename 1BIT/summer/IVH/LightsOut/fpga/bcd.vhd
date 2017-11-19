library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.STD_LOGIC_UNSIGNED.ALL;

entity BCDcounter is
    Port ( CLK : in  STD_LOGIC;
           RESET : in  STD_LOGIC;
           NUMBER3 : buffer  STD_LOGIC_VECTOR(3 downto 0) := "0000";
           NUMBER2 : buffer  STD_LOGIC_VECTOR(3 downto 0) := "0000";
           NUMBER1 : buffer  STD_LOGIC_VECTOR(3 downto 0) := "0000");
end BCDcounter;

architecture Behavioral of BCDcounter is

begin
	process ( CLK, RESET )
	begin
		if RESET='1' then 
			NUMBER1 <= "0000";
			NUMBER2 <= "0000";
			NUMBER3 <= "0000";
		elsif rising_edge(CLK) then
			if NUMBER1 = "1001" then
				NUMBER1 <= "0000";
				if NUMBER2= "1001" then
					NUMBER2 <= "0000";
					if NUMBER3= "1001" then
						NUMBER3 <= "0000";
					else NUMBER3 <= NUMBER3 + 1;
					end if;
				else	NUMBER2 <= NUMBER2 + 1;
				end if;
			else NUMBER1 <= NUMBER1 + 1;
			end if;
		end if;
	end process;
end Behavioral;

