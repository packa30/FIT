library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.std_logic_arith.all;
use IEEE.std_logic_unsigned.all;

entity ledc8x8 is
port ( -- Sem doplnte popis rozhrani obvodu.

		SMCLK			: in std_logic;
		RESET			: in std_logic;
		LED			: out std_logic_vector(0 to 7);
		ROW			: out std_logic_vector(0 to 7)
);
end ledc8x8;

architecture main of ledc8x8 is

		signal change		: std_logic;
		signal count		: std_logic;
		signal print_ce	: std_logic_vector(22 downto 0);
		signal active		: std_logic_vector(7 downto 0);
		signal light			: std_logic_vector(7 downto 0);
    -- Sem doplnte definice vnitrnich signalu.

begin

	 
	gen_cl: process(SMCLK,RESET)
	begin
				if RESET = '1' then
					print_ce <= "00000000000000000000000";
				elsif SMCLK'event and SMCLK = '1' then
						if print_ce(7 downto 0) = "11111111" then
							count <= '1';
						else
							count <= '0';
						end if;
					change <= print_ce(22);
					print_ce <= print_ce + 1;
				end if;
				
	end process gen_cl;
	
	
	riadky:process(RESET,count,SMCLK)
	begin
			if RESET = '1' then
					active <= "10000000";
			elsif	SMCLK'event and SMCLK = '1' and count = '1' then
					active <= active(0) & active(7 downto 1);
			end if;
	end process riadky;
	
	
	dekoder: process(active,change)
	begin
		if change = '0' then
		case active is
			when "10000000" => light <= "10000011";
			when "01000000" => light <= "10111101";
			when "00100000" => light <= "10111101";
			when "00010000" => light <= "10000011";
			when "00001000" => light <= "10111111";
			when "00000100" => light <= "10111111";
			when "00000010" => light <= "10111111";
			when "00000001" => light <= "10111111";
			when others 	 => light <= "11111111";
		end case;
	
		else
		case active is
			when "10000000" => light <= "11000011";
			when "01000000" => light <= "10111101";
			when "00100000" => light <= "10111111";
			when "00010000" => light <= "10111111";
			when "00001000" => light <= "10110001";
			when "00000100" => light <= "10111101";
			when "00000010" => light <= "10111101";
			when "00000001" => light <= "11000011";
			when others		 => light <= "11111111";
		end case;
		end if;
	end process dekoder;
	
	LED <= light;
	ROW <= active;
end main;
