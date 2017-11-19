library IEEE;
use IEEE.STD_LOGIC_1164.all;
	
package maska_pack is

 type mask_t is
  record
    top   	:	std_logic;
    left	 	:	std_logic;
	 right	:	std_logic;
	 bottom	:	std_logic;
 end record;


function getmask  (x,y : natural; COLLUMNS, ROWS : natural) return mask_t;

end maska_pack;

package body maska_pack is

	function getmask  (x,y : natural; COLLUMNS, ROWS : natural) return mask_t is
	
	variable mask : mask_t;
	
	begin
		if (x = 0) then
			mask.left := '0';
		else
			mask.left := '1';
		end if;
		
		if (x = (COLLUMNS-1)) then
			mask.right := '0';
		else
			mask.right := '1';
		end if;
		
		if (y = 0) then
			mask.top := '0';
		else
			mask.top := '1';
		end if;
		
		if (y =(ROWS -1)) then
			mask.bottom := '0';
		else
			mask.bottom := '1';
		end if;
		
		return mask;
		
  end getmask;
 
end maska_pack;
