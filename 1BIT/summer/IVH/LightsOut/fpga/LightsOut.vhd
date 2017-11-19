library IEEE;
use IEEE.std_logic_1164.ALL;

use IEEE.NUMERIC_STD.ALL;

use work.vga_controller_cfg.all;
use work.clkgen_cfg.all;
use maska_pack.ALL;

architecture main of tlv_pc_ifc is
	-- Data for VGA
	signal DAT_SELECT			: STD_LOGIC_VECTOR (24 downto 0) := (others => '0');
	signal DAT_ACTIVE			: STD_LOGIC_VECTOR (24 downto 0) := (others => '0');
	-- Init signals
	signal INIT_ACTIVE			: STD_LOGIC_VECTOR (24 downto 0) := (others => '0');
	signal INIT_SELECT			: STD_LOGIC_VECTOR (24 downto 0) := (others => '0');
	signal CELL_RESET			: STD_LOGIC := '0';

	-- key binds
	signal DAT_KEYS				: STD_LOGIC_VECTOR (15 downto 0);
	signal DAT_KEYS_VLD			: STD_LOGIC := '0';
	signal DAT_KEYS_2			: STD_LOGIC_VECTOR(15 downto 0);
	
	constant IDX_TOP    			: NATURAL := 0;
	constant IDX_LEFT   			: NATURAL := 1;
	constant IDX_RIGHT  			: NATURAL := 2;
	constant IDX_BOTTOM 			: NATURAL := 3;
	constant IDX_ENTER  			: NATURAL := 4;

	-- cursor signals
	signal CUR_X				: INTEGER := 0;
	signal CUR_Y				: INTEGER := 0;
	signal CUR_GX				: INTEGER := 0;
	signal CUR_GY				: INTEGER := 0;

	-- Score counter
	signal BCD_CLK				: STD_LOGIC := '0';
	signal BCD_RESET			: STD_LOGIC := '0';
	signal BCD_NUM1				: STD_LOGIC_VECTOR (3 downto 0) := "0000";
	signal BCD_NUM2				: STD_LOGIC_VECTOR (3 downto 0) := "0000";
	signal BCD_NUM3				: STD_LOGIC_VECTOR (3 downto 0) := "0000";

	-- Signals for VGA
	signal vga_mode  			: std_logic_vector(60 downto 0); -- default 640x480x60
	signal irgb   		 		: std_logic_vector(8 downto 0);
	signal rows   				: std_logic_vector(11 downto 0);
	signal colums   			: std_logic_vector(11 downto 0);

	signal INVERT_REQ			: std_logic_vector (99 downto 0) := (others => '0');
	signal SELECT_REQ			: std_logic_vector (99 downto 0) := (others => '0');

	signal ENABLE : STD_LOGIC := '1';
	signal CHECK_2 : INTEGER := 0;
	signal CHECK_4 : INTEGER := 0;
	signal CHECK_6 : INTEGER := 0;
	signal CHECK_8 : INTEGER := 0;
	signal CHECK_5 : INTEGER := 0;
	signal CHECK : INTEGER := 0;

	function get_bit(x,y,bitfield: std_logic_vector; SIZE: natural) return boolean is begin
		return (bitfield(to_integer(unsigned(x))+SIZE*to_integer(unsigned(y)))='1');
	end get_bit;

	function get_req_index(x,y: integer ; dir_id: natural; SIZE: natural) return natural is begin
	return (((x + SIZE) mod SIZE)+((y + SIZE) mod SIZE)*SIZE)*4 + (dir_id mod 4);
	end get_req_index;

begin


   -- Set graphical mode (640x480, 60 Hz refresh)
   setmode(r640x480x60, vga_mode);

	-- VGA controller
	vga: entity work.vga_controller(arch_vga_controller) 
	generic map (REQ_DELAY => 1)
	port map (
		CLK    => CLK, 
		RST    => RESET,
		ENABLE => '1',
		MODE   => vga_mode,

		DATA_RED    => irgb(8 downto 6),
		DATA_GREEN  => irgb(5 downto 3),
		DATA_BLUE   => irgb(2 downto 0),
		ADDR_COLUMN => colums,
		ADDR_ROW    => rows,

		VGA_RED   => RED_V,
		VGA_BLUE  => BLUE_V,
		VGA_GREEN => GREEN_V,
		VGA_HSYNC => HSYNC_V,
		VGA_VSYNC => VSYNC_V
	);

   -- Score counter
   SCORE_COUNTER : entity work.BCDcounter
		port map (
			CLK	 	=> BCD_CLK,
			RESET	=> BCD_RESET,

			NUMBER1 => BCD_NUM1,
			NUMBER2 => BCD_NUM2,
			NUMBER3 => BCD_NUM3
		);

    -- Attach keyboard
    KEYBOARD : entity work.keyboard_controller(arch_keyboard)
    generic map( READ_INTERVAL => 1200000 )
	port map (
		CLK => SMCLK,
		RST => RESET,

		DATA_OUT => DAT_KEYS_2(15 downto 0),
		DATA_VLD => DAT_KEYS_VLD,

		KB_KIN   => KIN,
		KB_KOUT  => KOUT
	);

	KEYBOARD_PROC : process(CLK)
	begin
		if CLK = '1' and CLK'event then
			BCD_RESET	<= '0';
			BCD_CLK		<= '0';
			CELL_RESET	<= '0';
			DAT_KEYS <= "0000"&"0000"&"0000"&"0000";

			if DAT_KEYS_2(15 downto 0) /= "00000000000000000" and ENABLE = '1' then
				BCD_CLK <= '1';
				CHECK <= CHECK +1;
			end if;

			-- Signal spliter
			if CHECK = 5500000 then
				CHECK <= 0;
				DAT_KEYS <= DAT_KEYS_2;
			end if;

			INIT_ACTIVE		<= "10000"&"01000"&"00100"&"01000"&"10000";
			INIT_SELECT		<= "00000"&"00000"&"00100"&"00000"&"00000";

			if DAT_KEYS_2(12) = '1' then
				INIT_ACTIVE		<= "01010"&"00000"&"01010"&"00000"&"01010";
				INIT_SELECT		<= "00000"&"00000"&"00100"&"00000"&"00000";
			elsif DAT_KEYS_2(11) = '1' then
				INIT_ACTIVE		<= "00100"&"00000"&"00100"&"00000"&"00100";
				INIT_SELECT		<= "00000"&"00000"&"00100"&"00000"&"00000";
			elsif DAT_KEYS_2(14) = '1' then
				BCD_RESET		<= '1';
				CELL_RESET		<= '1';
			end if;
		end if;
	end process;

   SCREEN: process(rows, colums)
		begin
 		if CLK'event and CLK = '1' then
   			irgb <= "000000000";	-- set default color for entire screen

		-- Draw grid
		if colums(5 downto 0) = 0 or rows(5 downto 0) = 0 then
				irgb <= "000000000";
		elsif (conv_integer(rows(11 downto 6)-1) < 5 and (conv_integer(colums(11 downto 6)-2) < 5)) then
			
				-- Draw default color of/ cell
				irgb <= "111111000";
		if get_bit(rows(11 downto 6)-1, colums(11 downto 6)-2, DAT_ACTIVE, 5) then
					-- Draw active cells
					irgb <= "010001011";
		end if;
		if get_bit(rows(11 downto 6)-1, colums(11 downto 6)-2, DAT_SELECT, 5) then
					-- Draw selected cells
				irgb <= "111000000";
			end if;
		end if;
		end if;
   end process;
  
 	GEN_CELL_X:
		for Y in 0 to 4 generate
		GEN_CELL_Y :
			for X in 0 to 4 generate
			CELLX : entity work.cell
			generic map (MASK => getmask(X, Y, 5, 5))
			PORT MAP (
				INVERT_REQ_OUT					=> INVERT_REQ((X + Y * 5)*4 + 3 downto (X + Y * 5)*4),
				INVERT_REQ_IN(IDX_TOP)			=> INVERT_REQ(get_req_index(X, Y -1, IDX_BOTTOM,5)),
				INVERT_REQ_IN(IDX_LEFT)			=> INVERT_REQ(get_req_index(X -1, Y, IDX_RIGHT,5)),
				INVERT_REQ_IN(IDX_RIGHT)		=> INVERT_REQ(get_req_index(X +1, Y, IDX_LEFT,5)),
				INVERT_REQ_IN(IDX_BOTTOM)		=> INVERT_REQ(get_req_index(X, Y +1, IDX_TOP,5)),

				SELECT_REQ_OUT					=> SELECT_REQ((X + Y * 5)*4 + 3 downto (X + Y * 5)*4),
				SELECT_REQ_IN(IDX_TOP)			=> SELECT_REQ(get_req_index(X, Y -1, IDX_BOTTOM,5)),
				SELECT_REQ_IN(IDX_LEFT)			=> SELECT_REQ(get_req_index(X -1, Y, IDX_RIGHT,5)),
				SELECT_REQ_IN(IDX_RIGHT)		=> SELECT_REQ(get_req_index(X +1, Y, IDX_LEFT,5)),
				SELECT_REQ_IN(IDX_BOTTOM)		=> SELECT_REQ(get_req_index(X, Y +1, IDX_TOP,5)),

				KEYS(IDX_RIGHT)			=> DAT_KEYS(6),
				KEYS(IDX_LEFT)			=> DAT_KEYS(4),
				KEYS(IDX_TOP)			=> DAT_KEYS(1),		
				KEYS(IDX_BOTTOM)		=> DAT_KEYS(9),
				KEYS(IDX_ENTER)			=> DAT_KEYS(5),

				INIT_ACTIVE				=> INIT_ACTIVE(Y * 5 + X),
				INIT_SELECTED			=> INIT_SELECT(Y * 5 + X),
				-- write out data for VGA
				ACTIVE					=> DAT_ACTIVE(Y * 5 + X),
				SELECTED				=> DAT_SELECT(Y * 5 + X),

				CLK						=> CLK,
				RESET					=> RESET
			);
		end generate;
	end generate;

end main;




