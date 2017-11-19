-- cpu.vhd: Simple 8-bit CPU (BrainLove interpreter)
-- Copyright (C) 2016 Brno University of Technology,
--                    Faculty of Information Technology
-- Author : Peter Grofèík, xgrofc00
--

library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;

-- ----------------------------------------------------------------------------
--                        Entity declaration
-- ----------------------------------------------------------------------------
entity cpu is
 port (
   CLK   : in std_logic;  -- hodinovy signal
   RESET : in std_logic;  -- asynchronni reset procesoru
   EN    : in std_logic;  -- povoleni cinnosti procesoru
 
   -- synchronni pamet ROM
   CODE_ADDR : out std_logic_vector(11 downto 0); -- adresa do pameti
   CODE_DATA : in std_logic_vector(7 downto 0);   -- CODE_DATA <- rom[CODE_ADDR] pokud CODE_EN='1'
   CODE_EN   : out std_logic;                     -- povoleni cinnosti
   
   -- synchronni pamet RAM
   DATA_ADDR  : out std_logic_vector(9 downto 0); -- adresa do pameti
   DATA_WDATA : out std_logic_vector(7 downto 0); -- mem[DATA_ADDR] <- DATA_WDATA pokud DATA_EN='1'
   DATA_RDATA : in std_logic_vector(7 downto 0);  -- DATA_RDATA <- ram[DATA_ADDR] pokud DATA_EN='1'
   DATA_RDWR  : out std_logic;                    -- cteni (1) / zapis (0)
   DATA_EN    : out std_logic;                    -- povoleni cinnosti
   
   -- vstupni port
   IN_DATA   : in std_logic_vector(7 downto 0);   -- IN_DATA <- stav klavesnice pokud IN_VLD='1' a IN_REQ='1'
   IN_VLD    : in std_logic;                      -- data platna
   IN_REQ    : out std_logic;                     -- pozadavek na vstup data
   
   -- vystupni port
   OUT_DATA : out  std_logic_vector(7 downto 0);  -- zapisovana data
   OUT_BUSY : in std_logic;                       -- LCD je zaneprazdnen (1), nelze zapisovat
   OUT_WE   : out std_logic                       -- LCD <- OUT_DATA pokud OUT_WE='1' a OUT_BUSY='0'
 );
end cpu;


-- ----------------------------------------------------------------------------
--                      Architecture declaration
-- ----------------------------------------------------------------------------
architecture behavioral of cpu is

 type type_of_instr is (
  acc_inc, acc_dec, value_inc, value_dec, start_w, end_w, print, get, save_value, pom_save, halt, nop
  );
  signal instr : type_of_instr;
  signal codeID: type_of_instr;


  type fsm_state is (
    Sidle, Sfetch,
    Sdecode, 
    Sacc_inc, Sacc_dec, Svalue_inc, Svalue_dec, Sget, Sput, 
    start_while, start_while2, start_while3, end_while, end_while2, end_while3,
    Spom_save,
    S_halt, S_nop
);

signal prime_state : fsm_state;
signal next_state : fsm_state;

signal pc_addr:	std_logic_vector(11 downto 0);
signal pc_inc :	std_logic;
signal pc_dec : std_logic;

signal ptr_addr: std_logic_vector(9 downto 0);
signal ptr_inc : std_logic;
signal ptr_dec : std_logic;

signal cnt_addr : std_logic_vector(7 downto 0);
signal cnt_inc : std_logic;
signal cnt_dec : std_logic;

signal TMP_addr : std_logic_vector(7 downto 0);
signal TMP_val : std_logic_vector(7 downto 0);
signal TMP_id : std_logic_vector(1 downto 0);

signal MX : std_logic_vector(1 downto 0) := (others => '0');




begin

  PC_proc: process (RESET, CLK)
  begin
  	if (RESET = '1') then 
  		pc_addr <= (others => '0');
  	elsif (CLK'event) and (CLK='1') then
	   	if (pc_inc = '1') then
	 	   	pc_addr <= pc_addr + '1';
	   	elsif (pc_dec = '1') then
	 	   	pc_addr <= pc_addr - '1';
  		end if;
	  end if;
  end process;
  CODE_ADDR <= pc_addr;
  
CNT_proc: process(RESET, CLK)
	begin
		if RESET = '1' then
			cnt_addr <= "00000000";
		elsif CLK'event and CLK = '1' then
			if cnt_inc = '1' then
				cnt_addr <= cnt_addr + '1';
			elsif cnt_dec = '1' then
				cnt_addr <= cnt_addr - '1';
			end if;
		end if;
	end process;	


	PTR_proc: process(CLK, RESET)
	begin
		if (RESET = '1') then
			ptr_addr <= "1000000000";
		elsif (CLK'event) and (CLK = '1') then
			if (ptr_inc = '1') then
				ptr_addr <= ptr_addr + '1';
			elsif (ptr_dec = '1') then
				ptr_addr <= ptr_addr - '1';
			end if;
		end if;
	end process;
	DATA_ADDR <= ptr_addr;

	TMP_proc: process(CLK, RESET)
	begin
		if (RESET = '1') then
			TMP_val <= "00000000";
		elsif (CLK'event) and (CLK = '1') then
			if (TMP_id = "00") then	
				TMP_addr <= DATA_RDATA; 
			elsif (TMP_id = "01") then
				TMP_val <= TMP_addr;
			else
				TMP_val <= "00000000";
			end if;
		end if;
	end process;
  
  
  dec_pro: process (CODE_DATA)
  begin
  	case (CODE_DATA) is
  		when X"3E" => instr <= acc_inc; 
  		when X"3C" => instr <= acc_dec; 
  		when X"2B" => instr <= value_inc;
  		when X"2D" => instr <= value_dec;
  		when X"5B" => instr <= start_w;
  		when X"5D" => instr <= end_w;
  		when X"2E" => instr <= print; 
  		when X"2C" => instr <= get; 
      when X"24" => instr <= save_value;
      when X"21" => instr <= pom_save;
  		when X"00" => instr <= halt;
  		when others=> instr <= nop; 
  	end case;
  end process;
  
  mulx : process(DATA_RDATA, MX, TMP_val)
	begin
		case MX is
			when "00" => DATA_WDATA <= IN_DATA;
			when "10" => DATA_WDATA <= DATA_RDATA + "00000001";
			when "11" => DATA_WDATA <= DATA_RDATA - "00000001";
      when others => DATA_WDATA <= TMP_val;
		end case;
	end process;

  prime_state_fsm : process(RESET, CLK)
  begin
	 if (RESET = '1') then
	 	 prime_state <= sidle;
	 elsif (CLK'event) and (CLK = '1') then
		  if (EN = '1') then
		  	prime_state <= next_state;
		  end if;
   end if;
  end process;

  next_state_fsm : process( DATA_RDATA, OUT_BUSY, prime_state, next_state, instr, cnt_addr, pc_addr, ptr_addr)
  begin
    DATA_EN <= '0';
    CODE_EN <= '0';
  	DATA_RDWR <= '0';
  	pc_inc <= '0';
  	pc_dec <= '0';
  	ptr_inc <= '0';
  	ptr_dec <= '0';
  	cnt_inc <= '0';
  	cnt_dec <= '0';
  	IN_REQ <= '0';
  	OUT_WE <= '0';
    MX <= "00";
    TMP_id <= "11";
    
    case prime_state is
      when Sidle =>
				next_state <= Sfetch;
      when Sfetch =>
				CODE_EN <= '1';
				next_state <= Sdecode;
      when Sdecode =>
          case instr is
          
            when acc_inc => 
					   	next_state <= Sacc_inc;
					  when acc_dec => 
						  next_state <= Sacc_dec;
            
            when print =>
              DATA_EN <= '1';
				      DATA_RDWR <= '1';
					   	next_state <= Sget;	
					  when get =>
              IN_REQ <= '1';
					   	next_state <= Sput;
					  
            when start_w =>
              pc_inc <= '1';
				      DATA_EN <= '1';
				      DATA_RDWR <= '1';
					   	next_state <= start_while;
					  when end_w =>
              DATA_EN <= '1';
				      DATA_RDWR <= '1';
					   	next_state <= end_while;
              
            when value_inc => 
              DATA_EN <= '1';
				      DATA_RDWR <= '1';
						  next_state <=Svalue_inc;
				  	when value_dec =>
              DATA_EN <= '1';
				      DATA_RDWR <= '1';
					   	next_state <= Svalue_dec;
					  
            when save_value =>
              TMP_id <= "00";
              DATA_EN <= '1';
				      DATA_RDWR <= '1';
				      pc_inc <= '1';
					  	next_state <= Sfetch;
					  when pom_save =>
              TMP_id <= "01";
						  next_state <= Spom_save;
              
					  when halt =>
					   	next_state <= S_halt;
					  when others =>
					   	next_state <= S_nop;
				end case;
      
      when Svalue_inc =>
        MX <= "10";
				DATA_EN <= '1';
				DATA_RDWR <= '0';
				pc_inc <= '1';
				next_state <= Sfetch;
       
      when Svalue_dec =>
        MX <= "11";
				DATA_EN <= '1';
				DATA_RDWR <= '0';
				pc_inc <= '1';
				next_state <= Sfetch;
      
      when Sget =>
        if OUT_BUSY = '0' then
					OUT_WE <= '1';
					OUT_DATA <= DATA_RDATA;
					pc_inc <= '1';
					next_state <= Sfetch;
				else 
					next_state <= Sget;
				end if;
        
      when Sput =>
        if IN_VLD = '1' then
					MX <= "00";
					DATA_EN <= '1';
					DATA_RDWR <= '0';
					pc_inc <= '1';
					next_state <= Sfetch;
				else
					next_state <= Sput;
				end if;  
        
     
      when Sacc_inc =>
        pc_inc <= '1';
				ptr_inc <= '1';			
				next_state <= Sfetch;
     
      when Sacc_dec =>
        pc_inc <= '1';
				ptr_dec <= '1';			
				next_state <= Sfetch;
      
      
      
      --CYCLE
      when start_while =>
				if DATA_RDATA = "00000000" then
					cnt_inc <= '1';
					next_state <= start_while2;
				else 
					next_state <= Sfetch;
				end if;
      
      when start_while2 =>
				CODE_EN <= '1';
				codeID <= instr;
				next_state <= start_while3;
      
      when start_while3 =>
				if instr = start_w then
					cnt_inc <= '1';
				elsif instr = end_w then 
					cnt_dec <= '1';
				end if;
				pc_inc <= '1';
        if cnt_addr = "00000000" then
					next_state <= Sfetch;
				else 
					next_state <= start_while2;
				end if;
       
       --CYCLE END
       when end_while => 
				if DATA_RDATA = "00000000" then
					pc_inc <= '1';
					next_state <= Sfetch;
				else
					cnt_inc <= '1';
					pc_dec <= '1';
					next_state <= end_while2;
				end if;
        
        when end_while2 =>
				CODE_EN <= '1';
				codeID <= instr;
				next_state <= end_while3;
        
        when end_while3 => 
				if instr = end_w then 
					cnt_inc <= '1';
				elsif instr = start_w then
					cnt_dec <= '1';
				end if;
				if cnt_addr = "00000000" then
					pc_inc <= '1';
					next_state <= Sfetch;
				else
				pc_dec <= '1';
					next_state <= end_while2;
				end if;
       
      when Spom_save =>
        DATA_EN <= '1';
				DATA_RDWR <= '0';
	      MX <= "01";
	      pc_inc <= '1';	
			  next_state <= Sfetch;
        
      
      when S_nop =>
        next_state<=Sfetch;
        pc_inc<='1';
	when S_halt =>
        next_state<=S_halt;                    
    end case;  
  end process;
end behavioral;
 
