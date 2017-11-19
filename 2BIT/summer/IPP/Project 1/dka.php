<?php
/**
 * autor: Peter Grofčík
 * predmet: IPP
 * projekt: DKA, proj č.1
 * date: 3:2017
 */
/**
 * Gets the parameters.
 *	Funkcia prechadza parametre prikazovej riadky a zaroven kontroluje ich spravne zadanie
 * 
 */
function getParams($argc,$argv){
if($argc!=1)
{
	for($i=1;$i<$argc;$i++)
	{
		if(!strncmp($argv[$i],"--input=",8)||!strncmp($argv[$i],"-input=",7))
		{
			if($input!=null)
				errors(1);
			global $input;
			if(!strncmp($argv[$i],"--input=",8))
				$input = substr($argv[$i],8);
			else 
				$input = substr($argv[$i],7);
		}
		elseif(!strncmp($argv[$i],"--output=",9)||!strncmp($argv[$i],"-output=",8))
		{
			if($output!=null)
				errors(1);
			global $output;
			if(!strncmp($argv[$i],"--output=",9))
				$output = substr($argv[$i],9);
			else
				$output = substr($argv[$i],8);
		}
		elseif (!strncmp($argv[$i],"--analyze-string=",17)) 
		{
			if($analyze==TRUE)
				errors(1);
			global $analyze;global $analyze_str;
			$analyze=TRUE;
			$analyze_str=substr($argv[$i], 17);
			
		}
		else
		{
			switch($argv[$i]){
				case "--help":
					if($argc==2)
						help();
					else
						errors(1);
					break;
				case '-e':
				case '--no-epsilon-rules':
					if($epsilon==TRUE)
						errors(1);
					global $epsilon;$epsilon=TRUE;
					break;
				case "-d":
				case "--determinization":
					if($determ==TRUE)
						errors(1);
					global $determ;$determ=TRUE;
					break;
				case "-i":
				case "--case-insensitive":
					if($insens==TRUE)
						errors(1);
					global $insens;$insens=TRUE;
					break;
				case "-r":
				case "--rules-only":
					if($r_only==TRUE)
						errors(1);
					global $r_only;$r_only=TRUE;
					break;
				case "-w":
				case "--white-char":
					if($white_char==TRUE)
						errors(1);
					global $white_char;$white_char=TRUE;
					break;
				default:
					errors(1);
					break;
			}

		}
	}
	if(($epsilon && $determ)==TRUE )
		errors(1);
	if($analyze&&($determ||$epsilon)==TRUE)
		errors(1);
}


}
/**
 * errors
 * Jednoducha funkcia na vypis stderr hlasok + ukoncenie programu s error kodom
 * @param      < kod erroru
 */
function errors($err)
{
	switch($err){
		case 0:
			break;
		case 1:
			fwrite(STDERR, "wrong param format\n");
			break;
		case 2:
			fwrite(STDERR, "input file error\n");
			break;
		case 3:
			fwrite(STDERR, "output file error\n");
			break;
		case 4:
			fwrite(STDERR, "wrong input file format\n");
			break;
		case 40:
			fwrite(STDERR, "syntax error\n");
			break;
		case 41:
			fwrite(STDERR, "wrong fine state machine\n");
			break;
		default:
	}
	exit($err);
}
/**
 * funkcia vypise help
 */
function help()
{
	echo "*****************************************************************\n
--input=filename      >> vstupny subor\n
--output=filename     >> vystupny subor\n
-e,--no-epsilon-rules >> odstranenie epsilon pravidiel
-d,--determinization	 >> determinizacia bey nedostupnych stavov\n
-i,--case-insensitive >> bez ohladu na velkost znakov\n
-e a -d 	>>>> nie je mozne kombinovat\n
*****************************************************************\n";
	errors(0);
}
/**
 * funkcia sa pokusi otvorit vstup/vystup 
 * na zaklade toho ci sa jej to podary alebo nie
 * pripadne vola funkciu errors
 *
 */
function open($input,$output)
{
	global $input;global $output;
	
	if($input||$output!=null)
	{
		if($input!=null)
		{
			if((file_exists($input)&&$input=fopen($input,'r'))!=TRUE)
				errors(2);
		}
		if($output!=null)
		{
			if(($output=fopen($output,'w'))!=TRUE)
				errors(2);
		}		
		 
	}
	if($input==null)
	{
		$input = STDIN;
	}
	if($output==null)
		$output = STDOUT;
	
}
/**
 * funkcia ktora prechadza vstupny subor znak po znaku a podla syntaxe
 *
 * funkcia obsahuje hlavny switch pre jednotlivy znak mimo vnutrajsku
 * jednotlivych komponent konecneho automatu
 * 
 * vedlajsie switche su konkretne pre obsah vnutra jednotlivych komponent
 */
function inputFile($input,$insens,$r_only,$white_char)
{
	$semicolon;$semicolon2=1;$next;
	
	while(!feof($input))
	{
		if($r_only==TRUE)
		{
			$char="{";
			$semicolon2=3;
			$semicolon=TRUE;
		}
		else
			$char=fgetc($input);
		switch($char){
			case '#':
				while(!feof($input)){
					if(($char=fgetc($input))!="\n"){}
					else
					{
						if($white_char==TRUE && $ok==TRUE)
						{
							$semicolon2++;
							$ok=FALSE;
						}
						break;
					}
				}
				break;
			case '(':
				$semicolon=TRUE;
				break;
			case '{':
				if($semicolon!=TRUE)
					errors(40);
				$prvok=null;
				switch($semicolon2){
					case 1:
						global $finstates;
						$finstates=array();$wch_pomoc=FALSE;
						while(($char=fgetc($input))!='}')
						{
							if(ctype_space($char)!=FALSE)
							{
								if($white_char==TRUE&&$prvok!=null)
									$wch_pomoc=TRUE;
							}
							elseif($char=="#"){
								while(($char=fgetc($input))!="\n"){}
								if($white_char==TRUE&&$prvok!=null)
									$wch_pomoc=TRUE;
							}
							elseif($char==",")
							{
								if(!in_array($prvok, $finstates))
									array_push($finstates, $prvok);
								$prvok=null;
								$ended=TRUE;
							}
							else
							{
								if($wch_pomoc==TRUE&&$white_char==TURE)
								{
									if(!in_array($prvok, $finstates))
										array_push($finstates, $prvok);
									$prvok=$char;
									$ended=FALSE;
									$wch_pomoc=FALSE;
								}
								else
								{
									$prvok.=$char;
									$ended=FALSE;
								}
							}
						}
						if($ended==TRUE)
							errors(40);
						if(!in_array($prvok, $finstates))
							array_push($finstates, $prvok);
						$prvok=null;
						$ok=TRUE;
						break;
					case 2:
						global $abc;$wch_pomoc=FALSE;
						$abc=array();
						while(($char=fgetc($input))!='}')
						{
							if(ctype_space($char)!=FALSE)
							{
								if($white_char==TRUE&&$prvok!=null)
								{
									if(!in_array($prvok, $abc))
										array_push($abc, $prvok);
									$prvok=null;
								}
							}
							elseif($char=="#"){
								while(($char=fgetc($input))!="\n");
								if($white_char==TRUE&&$prvok!=null)
								{
									if(!in_array($prvok, $abc))
										array_push($abc, $prvok);
									$prvok=null;
								}
							}
							elseif($char=="'")
							{
								$ended=FALSE;
								if(($char=fgetc($input))=="'")
								{
									if(($char=fgetc($input))=="'")
									{
										if(($char=fgetc($input))=="'")
										{
											$prvok="'";
										}
										else
										{
											errors(40);
										}
									}
									else
										$prvok="";
								}
								else
								{
									$prvok.=$char;
									while(($char=fgetc($input))!="'")
									{
										$prvok.=$char;
									}
								}
							}
							elseif($char==",")
							{
								if(!in_array($prvok, $abc))
									array_push($abc, $prvok);
								$prvok=null;$ended=TRUE;
							}
							else
							{
									if($white_char==TRUE)
										$prvok.=$char;
									else
										errors(40);
							}
						}
						if($ended==TRUE)
						{
							errors(40);
						}
						if(!in_array($prvok, $abc))
							array_push($abc, $prvok);
						$abc=array_filter($abc);
						$prvok=null;
						$ok=TRUE;
						break;
					case 3:
						global $rulez;global $ep_ar;
						$rulez=array();$ep_ar=array();
						$apos=FALSE;$apose=FALSE;$mins=FALSE;$almost=FALSE;$pomocn=FALSE;
						if($r_only==TRUE)
						{
							$rules_only=feof($input);
						}
						else
							$rules_only="}";
						if($white_char==TRUE)
						{
							while(!feof($input))
							{
								$char=fgetc($input);
								if($r_only==FALSE)
									if($char==$rules_only)
										break;
								switch ($char){
									case "#":
										while(!feof($input))
											{
												$char=fgetc($input);
												if($char=="\n")
												{
													break;
												}
											}
											if(feof($input))
											{
												$he=ftell($input);
												fseek($input, $he-1);
											}
											
											
										if($prvok!=null)
											$medzera=TRUE;
										break;
									default:
										if(ctype_space($char)==TRUE)
										{
												if($prvok!=null)
													$medzera=TRUE;
										
										}
										else
										{
											if($char=="'" && $apos==FALSE)
											{
												$prvok.=" $char";
												if(($char=fgetc($input))=="'")
												{
													$prvok.=$char;
													if(($char=fgetc($input))=="'")
													{
														if(($char=fgetc($input))=="'")
															$prvok.=$char;
														else
															errors(40);
													}
													else
													{
														$eps=TRUE;
														$prvok.=" ";
													}
												}
												else
												{
													$prvok.=$char;
													while(($char=fgetc($input))!="'")
													{
														$prvok.=$char;
													}
													$prvok.="' ";
												}
												$apos=TRUE;
											}
											elseif($prvok!=null && $apos==TRUE && $sipka!=TRUE)
											{
												if($char!="-")
												{
													errors(40);
												}
												if(($char=fgetc($input))!=">")
													errors(40);
												$prvok.="-> ";
												$sipka=TRUE;
											}
											elseif($prvok!=null && $medzera==TRUE && $apos==FALSE && $char!="-")
											{
												
												$prvok.=" '$char";
												$char=fgetc($input);
												while(!ctype_space($char))
												{
													if($char=="#")
														break;
													$prvok.=$char;
													$char=fgetc($input);
												}
												$prvok.="' ";
												if($char=="#")
													while($char=fgetc($input)!="\n");
												$apos=TRUE;

											}
											elseif($char=="-" && $prvok !=null && $apos==FALSE)
											{
												$prvok.=" '' ";
												$apos=TRUE;
												$he=ftell($input);
												fseek($input, $he-1);
											}
											elseif($sipka==TRUE)
											{
												$prvok.=$char;
												$char=fgetc($input);

												while(!ctype_space($char))
												{
													if($char=="#")
														break;
													if($char==",")
													{
														$ciarka=TRUE;
														break;
													}
													if($char==$rules_only)
														break;
													$prvok.=$char;
													$char=fgetc($input);
												}
												
												if($char=="#")
													while($char=fgetc($input)!="\n");
												if($EPS==TRUE)
												{	
													if(!in_array($prvok, $ep_ar))
													array_push($ep_ar, $prvok);
													$EPS=FALSE;	
												}
												if(!in_array($prvok, $rulez))
													array_push($rulez, $prvok);
												$prvok=null;$apos=FALSE;$sipka=FALSE;$medzera=FALSE;
												if($char==$rules_only)
												{
													$he=ftell($input);
													fseek($input, $he-1);
												}


											}
											else
											{
												$ciarka=FALSE;
												$prvok.=$char;
											}

										}
								}	
							}
							
							if($ciarka==TRUE)
							{
								errors(40);
							}
							if($char==$rules_only)
							{
								$he=ftell($input);
								fseek($input, $he-1);
							}
							
						}
						if($white_char==TRUE && $r_only==TRUE)
							$char=fgetc($input);

						while(($char=fgetc($input))!=$rules_only)
						{
							if(ctype_space($char)){}
							elseif($char=="#"){
								while(($char=fgetc($input))!="\n"){}
							}
							elseif($char==",")
							{
									if($prvok==null)
										errors(40);
									if(!in_array($prvok, $rulez))
										array_push($rulez, $prvok);
									if($EPS==TRUE)
									{	
										if(!in_array($prvok, $ep_ar))
											array_push($ep_ar, $prvok);
										$EPS=FALSE;	
									}
									$prvok=null;$ended=TRUE;
							}
							else
							{	
								$ended=FALSE;
								if($char=="'"&&$apos==TRUE)
								{
									$prvok.=" $char";
									if(($char=fgetc($input))=="'")
									{
										$prvok.=$char;
										if(($char=fgetc($input))=="'")
										{
											if(($char=fgetc($input))=="'")
											{
												$prvok.="' ";
											}
											else
												errors(40);
										}
										else
										{
	
											if($char=="-")
											{
												$prvok.=" $char";
												$mins=FALSE;
												$minss=TRUE;
											}
											else
											{
												$prvok.=" ";
												while(($char=fgetc($input))!="-")
												{
													if(!ctype_space($char))
													{
														errors(40);
													}
												}
												$prvok.=$char;
												$mins=FALSE;
												$minss=TRUE;
											}
											$EPS=TRUE;

										}
									
									}
									else
									{
										$prvok.=$char;
										while(($char=fgetc($input))!="'")
										{
										$prvok.=$char;
										}
										$prvok.="$char ";
										
									}
									$apose=TRUE;
									$apos=1;
									if($minss!=TRUE)
										$mins=TRUE;
									
								}
								elseif(($mins==TRUE)&&($char!="-"))
								{
									errors(40);
								}
								elseif(($minss==TRUE)&&($char!=">"))
								{
									errors(40);
								}
								else
								{
										$minss=FALSE;
										$prvok.=$char;
										if($char==">")
										{
											$prvok.=" ";
											$almost=TRUE;
										}
										if($apose==FALSE)
											$apos=TRUE;
										if($mins==TRUE)
										{
											$mins=FALSE;
											$minss=TRUE;
										}
								}
							}
						
						}
						if($white_char!=TRUE)
						{
							if($ended==TRUE)
							{
								errors(40);
							}
							if(!in_array($prvok, $rulez))
								array_push($rulez, $prvok);
							if($EPS==TRUE)
							{	
								if(!in_array($prvok, $ep_ar))
									array_push($ep_ar, $prvok);
								$EPS=FALSE;	
							}
						}
						$rulez=array_filter($rulez);
						$prvok=null;$apos=$apose=$mins=$minss=FALSE;
						$ok=TRUE;
						break;
					case 5:
						global $finalstates;
						$finalstates=array();
						while(($char=fgetc($input))!='}')
						{
							if(ctype_space($char)!=FALSE)
							{
								if($white_char==TRUE && $prvok!=null)
								{
									if(!in_array($prvok, $finalstates))
										array_push($finalstates, $prvok);
									$prvok=null;$ended=TRUE;
								}
							}
							elseif($char=="#"){
								while(($char=fgetc($input))!="\n"){}
									if($white_char==TRUE && $prvok!=null)
									{
										if(!in_array($prvok, $finalstates))
											array_push($finalstates, $prvok);
										$prvok=null;$ended=TRUE;
									}
							}
							elseif($char==",")
							{
								if(!in_array($prvok, $finalstates))
									array_push($finalstates, $prvok);
								$prvok=null;$ended=TRUE;
							}
							else
							{
								$ended=FALSE;
								$prvok.=$char;
							}
						}

						if($ended==TRUE)
							errors(40);
						if(!in_array($prvok, $finalstates))
							array_push($finalstates, $prvok);
						$prvok=null;
						$ok=TRUE;
						break;
					default:
						errors(40);
						break;
				}
				break;
			case ',':
				$semicolon2++;
				$ok=FALSE;
				break;
			case ')':
				$semicolon=FALSE;
				break;
			default :
				if(ctype_space($char)==TRUE||$char==null){
					if($white_char==TRUE && $ok==TRUE)
					{
						$semicolon2++;
						$ok=FALSE;
					}
					break;
				}
				elseif($semicolon2==4)
				{
					global $pociatok;
					$pociatok.=$char;
					if($white_char==TRUE)
					{
						$char=fgetc($input);
						while(($char!=",") && (!ctype_space($char)) && ($char!="#"))
						{
							$pociatok.=$char;
							$char=fgetc($input);	
						}
						if($char=="#")
							while(($char=fgetc($input))!="\n");
						$semicolon2++;
					}
					else
					{
						while(($char=fgetc($input))!=',')
							{
								if(ctype_space($char)!=FALSE){}
								elseif($char=="#")
									while(($char=fgetc($input))!="\n");
								else
									$pociatok.=$char;
							}
						$semicolon2++;
					}
						break;
				}
				else
				{
					errors(40);
				}

			}
	}


	if($r_only==TRUE)
	{

		global $abc,$finstates,$finalstates,$pociatok;
		rul_only($rulez,$abc,$finstates,$finalstates,$ep_ar,$pociatok);
	}


	if($insens==TRUE)
	{
		for($count10=0;$abc[$count10]!=null;$count10++)
		{
			$abc[$count10]=strtolower($abc[$count10]);
		}
		for($count10=0;$finstates[$count10]!=null;$count10++)
		{
			$finstates[$count10]=strtolower($finstates[$count10]);
		}
		for($count10=0;$finalstates[$count10]!=null;$count10++)
		{
			$finalstates[$count10]=strtolower($finalstates[$count10]);
		}
		for($count10=0;$rulez[$count10]!=null;$count10++)
		{
			$rulez[$count10]=strtolower($rulez[$count10]);
		}
		$pociatok=strtolower($pociatok);
	}
	

	if($abc[0]==null)
	{
		errors(41);
	}
	if(!in_array($pociatok,$finstates))
	{
		errors(41);
	}
	$count=0;
	while($finalstates[$count]!=null)
	{
		if(!in_array($finalstates[$count], $finstates))
			errors(41);
		$count++;
	}
	$count=0;$help=null;
	while($rulez[$count]!=null)
	{
		$rule=$rulez[$count];
		$count2=0;
		while($rule[$count2]!=" ")
		{
			$help.=$rule[$count2];
			$count2++;
		}
		$count2++;
		if(!in_array($help,$finstates))
		{
			errors(41);
		}
		$help=null;
		
		$pomoc=null;
		while($pomoc!=" -")
		{
			if($rule[$count2]!="'")
				$help.=$rule[$count2];
			$count2++;
			$pomoc=$rule[$count2];
			$pomoc.=$rule[$count2+1];
		}
		$count2+=3;
		
		if($help==null){}
		else
		{
			if(!in_array($help, $abc))
			{
				errors(41);
			}
		}
		$help=null;$count2++;
		while($rule[$count2]!=null)
		{
			$help.=$rule[$count2];
			$count2++;
		}
		if(!in_array($help,$finstates))
		{
			errors(41);
		}
		$help=null;
		$count++;
	}
}
/**
 * funkcia na korektny vypis podla zadania 
 * vpodstate vypise na output konecny automat v spravnom formate a v stave 
 * v akom je pri volani funkcie
 * nijak neupravuje konecny automat
 *
 */
	function out($output,$finstates,$abc,$rulez,$pociatok,$finalstates)
	{
		sort($abc);sort($finstates);sort($finalstates);sort($rulez);
		$outputs;
		$outputs="(\n{";
		$count=0;
		while($finstates[$count]!=null)
		{	
			if($count!=0)
				$outputs.=", ";
			$outputs.="$finstates[$count]";
			$count++;
		}
		$outputs.="},\n{";
		$count=0;
		while($abc[$count]!=null)
		{
			if($count!=0)
				$outputs.=", ";
			if($abc[$count]=="'")
				$outputs.="'$abc[$count]''";
			else
				$outputs.="'$abc[$count]'";
			$count++;
		}
		$outputs.="},\n{\n";
		$count=0;
		while($rulez[$count]!=null)
		{
			$count2=0;$rule=$rulez[$count];
			while($rule[$count2]!="'")
			{
				if(!ctype_space($rule[$count2]))
					$outputs.=$rule[$count2];
				$count2++;
			}
			$outputs.=" ";$apes=0;
			while($rule[$count2]!=null)
			{
				if($rule[$count2]=="'")
					$apes++;
				if($apes==3)
				{
					$outputs.="'";
					$apes=0;
				}
				if($rule[$count2]=="-"&&$rule[$count2+1]==">")
				{
					$outputs.="->";
					$count2=$count2+2;
				}
				
				$outputs.=$rule[$count2];
				
				$count2++;
			}
			if($rulez[$count+1]!=null)
				$outputs.=",\n";
			else
				$outputs.="\n";
			$count++;
		}
		$outputs.="},\n$pociatok,\n{";$count=0;
		while($finalstates[$count]!=null)
		{
			if($count!=0)
				$outputs.=", ";
			$outputs.=$finalstates[$count];
			$count++;
		}
		$outputs.="}\n)\n";
		fwrite($output,$outputs);
	}
/**
 * zakladna funkcia na odstranenie epsilon rules
 * prechadza vsetky epsilon rules ktore mi funkcia inputFile() nahrala
 * nasledne vola funkciu ep_s za predpokladu že sa v automate nejake epsilon rules nachadzaju
 */
function ep($rulez,$ep_ar)
{
	global $rulez;global $ep_ar;
	$rulez_tmp=$ep_ar;
	$count=0;
	while($ep_ar[$count]!=null)
	{
		global $rules;
		$rules=$ep_ar[$count];
		ep_s($rulez,$rules,$ep_ar);

		$count++;	
	}
	$count=0;
	while($rulez_tmp[$count]!=null)
	{
		$count2=0;
		while($rulez[$count2]!=null)
		{	
			if($rulez[$count2]==$rulez_tmp[$count])
			{	
				$count3=$count2;
				while($rulez[$count3]!=null)
				{
					$rulez[$count3]=$rulez[$count3+1];
					$count3++;
				}
				
				$rulez=array_filter($rulez);
			}	
			$count2++;
		}
		
		$count++;
	}

}
/**
 * funkcia s rekurzivnym volanim
 * 
 * rekurzivne sa vola kym existuje epsilon prechod za epsilon prechodom najdenym
 * prevyhotovenie epsilon uzaveru
 * 
 * nasledne pracuje s danym uzaverom a podla neho upravy rules a tiez finalne stavy 
 * aby z konecneho automatu vznikol nedeterministicky konecny automat bez epsilon prechodov 
 *
 */
function ep_s($rulez,$rules,$ep_ar)
{
	$count=0;global $rulez;$r_now;$r_now=array();global $ep_ar;$start=null;global $rules;
	while($rules[$count]!=" ")
	{
		$start.=$rules[$count];
		$count++;
	}
	while($rules[$count]!=null)
		{
			
			if($rules[$count]==" "&&$rules[$count-1]==">")
			{
				$count++;
				while($rules[$count]!=null)
				{
					$rul_next.=$rules[$count];
					$count++;
				}
			}	

			$count++;
		}
	$count=0;
	while($rulez[$count]!=null)
	{
		$ru=$rulez[$count];

		$try=substr($ru,0,strlen($rul_next));
		if($try==$rul_next)
		{
			if(in_array($ru, $ep_ar))
			{
				$count2=0;
				while($ep_ar[$count2]!=null)
				{
					if($ep_ar[$count2]==$ru)
					{
						while($ep_ar[$count2]!=null)
						{
							$ep_ar[$count2]=$ep_ar[$count2+1];
							$count2++;
						}
						$rules=$ru;
						$ep_ar=array_filter($ep_ar);
						ep_s($rulez,$rules,$ep_ar);
					}
					$count2++;
				}
			}
			else
				array_push($r_now, $ru);
		}

		$count++;
	}
	$count=0;
	
	$row_abc=array();$row_where=array();
	while($r_now[$count]!=null)
	{	$rrnow=$r_now[$count];
		
		$count2=0;
		while($rrnow[$count2]!=null)
		{
			if($rrnow[$count2]=="'")
			{	
				$count2++;$abcnow=null;
				while($rrnow[$count2]!="'")
				{
					$abcnow.=$rrnow[$count2];
					$count2++;
				}
				array_push($row_abc,$abcnow);
				$count2+=5;
				break;
			}
			$count2++;
		}
		$end=null;

		while($rrnow[$count2]!=null)
		{
			$end.=$rrnow[$count2];
			$count2++;
		}
		array_push($row_where, $end);
		
		$count++;
	}
	$count=0;$count2=0;
	while($row_abc[$count]!=null)
	{
		$new_rule="$start '$row_abc[$count]' -> $row_where[$count]";
		if(!in_array($new_rule, $rulez))
			array_push($rulez, $new_rule);
		
		if($start==$rul_next)
		{
			$new_rule="$start '$row_abc[$count]' -> $rul_next";
			if(!in_array($new_rule, $rulez))
				array_push($rulez, $new_rule);
		}
		$count++;
	}


}
/**
 * hlavna funkcia determinizacie
 * pomocou rekurzivi prehliada rules ktore uz neobsahuju epsilon prechody 
 * 
 * v pripade zistenia rovnakej vstupnej abecedy z jedneho stavu do viacerych
 * tie nasledne spoji do jedneho a v pripade ze uz niesu vyriesene vola rekurzivne funkciu 
 * s novym vstupnym stavom
 * 
 * funkcia tiez vytvara nove ukoncujuce stavy v pripade ze stav vznikne spojenim aspon s jednym 
 * pôvodnym ukoncujucim stavom
 *
 */
function determin($prechody,$st,$rulez,$kstav,$abc,$finalstates,$finals)
{

	
	global $prechody;global $st;global $kstav;global $finals;
	$prechod=array();
	sort($kstav);
		$preco=$kstav[0];	
		for($count2=1;$kstav[$count2]!=null;$count2++)
		{
			$preco.="_$kstav[$count2]";
		}
		if(!in_array($preco, $st))
			array_push($st, $preco);
	for($count=0;$kstav[$count]!=null;$count++)
	{
		for($count2=0;$abc[$count2];$count2++)
		{
			$try="$kstav[$count] '$abc[$count2]'";
			for($count3=0;$rulez[$count3];$count3++)
			{
				if($try==substr($rulez[$count3], 0,strlen($try)))
				{				
					array_push($prechod,$rulez[$count3]);
					
				}

			}

		}

	}
	
	
	for($count=0;$abc[$count]!=null;$count++)
	{
		$final__state=FALSE;
		$prechodys=array();$proc=null;
		for($count2=0;$prechod[$count2];$count2++)
		{	
			$pre=$prechod[$count2];
			$i=0;
			while ($pre[$i]!= "'"){$i++;}
			$try="'$abc[$count]'";
			if($try==substr($prechod[$count2], $i, strlen($try)))
				array_push($prechodys, $prechod[$count2]);
		}
		
		$prav=array();

		$count2=0;
		for($count2=0;$prechodys[$count2]!=null;$count2++)
		{
			$prech=$prechodys[$count2];
			for($count3=0;$prech[$count3]!="'";$count3++){}$count3++;
			
			if($abc[$count]==substr($prechodys[$count2],$count3,strlen($abc[$count])))
			{
		
				if(!in_array(substr($prechodys[$count2], $count3+strlen($abc[$count])+5), $prav))
					array_push($prav, substr($prechodys[$count2], $count3+strlen($abc[$count])+5));
			
				
			}

		}
		
		sort($prav);

		
		for($count2=0;$prav[$count2]!=null;$count2++)
		{	
			if($count2==0)
			{
				$proc=$prav[0];
				
			}
			else
			{
				$proc.="_$prav[$count2]";

			}
			for($count5=0;$finalstates[$count5]!=null;$count5++)
			{
				if($prav[$count2]==$finalstates[$count5])
					$final__state=TRUE;
			}
			
		}
		if($proc==null){}
		else
		{
			$lebo="$preco '$abc[$count]' -> $proc";
		
			if(!in_array($lebo, $prechody))
			{
				array_push($prechody, $lebo);
			}
			
			if($final__state==TRUE)
				if(!in_array($proc, $finals))
					array_push($finals, $proc);
			
			if(!in_array($proc, $st))
			{
				$kstav=$prav;
				determin($prechody,$st,$rulez,$kstav,$abc,$finalstates,$finals);
			}

		}
		


	}

	
	

}
/**
 * pomocna funkcia na determinizaciu 
 * prednastavy parametre pre hlavnu funkciu determinizacie a zavola ju
 *
 */
function det($rulez,$finstates,$finalstates,$abc,$pociatok)
{
	
	global $prechody;$prechody=array();
	global $st;$st=array();
	global $finstates;sort($finstates);
	global $kstav;$kstav=array();
	global $finals;$finals=array();
	global $finalstates;global $rulez;
	array_push($kstav, $pociatok);
	array_push($st, $pociatok);
	determin($prechody,$st,$rulez,$kstav,$abc,$finalstates,$finals);
	$finalstates=$finals;
	$finstates=$st;
	$rulez=$prechody;


}
/**
 * funkcia analyze string (rozsirenie)
 * 
 * funkcia najskôr zisti ci sa zadany string nachadza v abecede 
 * ak nie vola funkciu errors() s err kodom 1 a do outputu nic nezapisuje
 * 
 * nasledne funkcia prechodom cez rules zistuje ci je dany string retazcom jazyka konecneho automatu
 * ak je vypise na out 1
 * ak nie vypise na out 0
 * obydve varianty koncia bez erroru
 * 
 */
function analy($abc,$rulez,$analyze_str,$output,$pociatok,$finalstates)
{
	
	global $output;$rulezz=$rulez;
	for($count=0;$analyze_str[$count]!=null;$count++)
	{
		
		if(!in_array($analyze_str[$count], $abc))
		{
			errors(1);
		}
	}
	$count2=0;$next=$pociatok;
	$v_string=0;
	$vo_string=strlen($analyze_str);$ok=FALSE;
	while($next!=null)
	{

		$count2=0;
		while($rulezz[$count2]!=null)
		{
			
			
			if($next==substr($rulezz[$count2], 0, strlen($next)))
			{
				$abe=null;
				$glhf=substr($rulezz[$count2], strlen($next)+2);
				$count3=0;
				while($glhf[$count3]!="'")
				{
					$abe.=$glhf[$count3];
					$count3++;
				}

				if($abe==$analyze_str[$v_string])
				{
					$ok=TRUE;
					$v_string++;
					break;
				}
				else
					$ok=FALSE;
			}
			$count2++;
		}
		if($ok!=TRUE)
		{
			fwrite($output,0);
			exit(0);
		}
		
		$count3=$count3+5+strlen($next)+2;

		$next=substr($rulezz[$count2], $count3);
		
		if($v_string==$vo_string && in_array($next,$finalstates))
		{
			fwrite($output, 1);
			exit(0);
		}
		$rulezz[$count2]=null;
		$rulezz=array_filter($rulezz);
		$rulezz=array_values($rulezz);
	}

}
/**
 * funkcia pre RUL (rozsirenie)
 * 
 * pomocna funkcia ktora zo vstupu na ktorom prisli len prechody
 * vytvori konecny automat ale sama o sebe ho nijak nemeni
 * 
 * v pripade ze bol zadany prepinac -r 
 * tak už funkcia inputFile vyriesi spravne nacitanie len prechodov
 * s ktorymi neskôr pracuje funkcia rul_only
 * 
 */
function rul_only($rulez,$abc,$finalstates,$finstates,$ep_ar,$pociatok)
{
	global $rulez,$abc,$finalstates,$finstates,$ep_ar,$pociatok;
	$abc=array();$finstates=array();$finalstates=array();
	for($count=0;$rulez[$count]!=null;$count++)
	{

		$rul=$rulez[$count];$stav=null;
		for($count2=0;$rul[$count2]!="'";$count2++)
		{
			if(!ctype_space($rul[$count2]))
				$stav.=$rul[$count2];

		}
	
		if($count==0)
			$pociatok=$stav;
		if(!in_array($stav, $finstates))
			array_push($finstates, $stav);
		$count2++;
		$stav=null;
		
		for($count2=$count2;$rul[$count2]!="'";$count2++)
		{
			$stav.=$rul[$count2];
		}
		if($rul[$count2+1]=="'")
		{
			$count2+=1;
			$stav="'";
		}
		
		if($stav!=null)
			if(!in_array($stav, $abc))
				array_push($abc, $stav);
		$count2+=5;
		$stav=null;

		for($count2=$count2;$rul[$count2]!=null;$count2++)
		{
			$stav.=$rul[$count2];
		}

		if(substr($stav, strlen($stav)-1)==".")
		{
			$stav=substr($stav, 0, -1);

			if(!in_array($stav, $finalstates))
				array_push($finalstates, $stav);

			if(in_array($rulez[$count], $ep_ar))
			{
				for($count3=0;$ep_ar[$count3]!=null;$count3++)
				{
					if($ep_ar[$count3]==$rulez[$count])
						$ep_ar[$count3]=substr($ep_ar[$count3], 0, -1);

				}
			}
			$rulez[$count]=substr($rulez[$count], 0,-1);	
		}
		if(!in_array($stav, $finstates))
				array_push($finstates, $stav);


	}

}
/**
 * vpodstate tu zacina kvazi main programu v ktorom sa zaradom volaju jednotlive
 * funkcie podla vstupnych argumentov
 */
getParams($argc,$argv);

open($input,$output);

inputFile($input,$insens,$r_only,$white_char);

if($epsilon==TRUE)
	ep($rulez,$ep_ar);
if($determ==TRUE)
{

	if($ep_ar[0]!=null)
		ep($rulez,$ep_ar);
	det($rulez,$finstates,$finalstates,$abc,$pociatok);
}
if($analyze!=TRUE)
	out($output,$finstates,$abc,$rulez,$pociatok,$finalstates);
else
{
	det($rulez,$finstates,$finalstates,$abc,$pociatok);
	analy($abc,$rulez,$analyze_str,$output,$pociatok,$finalstates);
}

fclose($input);
fclose($output);
?>