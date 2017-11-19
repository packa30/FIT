#!/usr/bin/env python3

import sys
import argparse
import re

##
## @brief      funkcia na argumenty
##
## @return     vracia objekt vytvorený funkciou ArgumentParser
##
def defArg(argv):
	parser = argparse.ArgumentParser(add_help=False)
	parser.add_argument('--help', action='store_true', default=False)
	parser.add_argument('--input', '-input','-inpu','-inp','-in','-i', nargs='?', action='append')
	parser.add_argument('--output', '-output','-outpu','-outp','-out','-ou','-o', nargs='?', action='append')
	parser.add_argument('--format', nargs='?',action='append')
	parser.add_argument('--br', action='store_true', default=False)
	parser.add_argument('--nooverlap', action='store_true', default=False)
	parser.add_argument('--escape', action='store_true', default=False)

	parserARG=['help','input','output','format','br']
	try:
		args = parser.parse_args()
	except:
		errors(1)
	for ARG in parserARG :
		ARGV=[]
		ARGV=getattr(args,ARG)
		if ARGV != None:
			try:
				ARGV[1]
			except:
				if re.match('(br)|(help)',ARG) == None:
					ARGVS=(str(ARGV))
					ARGVS=ARGVS.strip("([)|(])|(')")
					setattr(args,ARG,ARGVS)
			else:
				errors(1)
	if args.help:
		if len(argv)==2:
			print("""
--help                            napoveda
--format=filename                 nazov formatovacieho suboru
--input=filename                  vstupny subor(kodovanie UTF-8)
--output=filename                 vystupny subor(kodovanie UTF-8)
--br                              pridanie elementu <br /> na koniec kazdeho riadku
""")
		else:
			errors(1)
	return args

##
## @brief      Jednoducha funkcia error vystupu
##
def errors(err):
	if err == 1:
		print('wrong param format', file=sys.stderr)
	elif err == 2:
		print('input file error', file=sys.stderr)
	elif err == 3:
		print('output file error', file=sys.stderr)
	elif err == 4:
		print('wrong input format file', file=sys.stderr)
	else:
		print('unknown err',file=sys.stderr)
	exit(err)

##
## @brief      funkcia na kontrolu formátu formátovacieho súboru 
##
## @return     vráti pole polí v každom poli v poli je na prvom prvku regex
## 			   z formátovacieho súboru a na ostatných typy formátovania
##
def format(formatFile):
	format_in = []
	Commands = ['bold', 'italic', 'underline', 'teletype']
	freg=['(?:^[|])|(?:[^%][|]$)|(?:^\|$)|(?:^\|\|)|(?:[^%]\|\|)','(?:^\.)|(?:[^%]\.\.+)|(?:[^%]\.$)',
		  '(?:^!$)|(?:[^%]!$)','[(][)]','(?:^\.\|)|(?:[^%]\.\|)|(?:^\|\.)|(?:[^%]\|\.)']
	for line in formatFile:
		if line == '\n':
			continue
		try:
			part = ()
			Fpart = []
			part = line.partition('\t')
			if part[1] == "":
				errors(4)
			for fre in freg :
				if re.search(fre, part[0]) != None:
					exit(1)
			Fpart.append(part[0])
			party=part[2]
			for ch in ['\n','\t',' ']:
				party=(party.replace(ch,""))
			
			comms=party.split(",")
			for comm in comms:
				isthere=False
				if re.match('color:([0-9a-fA-F]{6})',comm) or re.match('size:([1-7]{1})',comm):
					isthere=True
				else:
					for Command in Commands:
						if Command == comm:
							isthere=True

				if isthere != True :
					exit(4)
				else:
					isthere=False

			Fpart.append(party)
		except:
			errors(4)
		format_in.append(Fpart)
	count=0
	for form in format_in:
		while re.search('(\*|\+){2}', format_in[count][0]) != None:
			format_in[count][0] = re.sub('(\++\*+)+', '*', format_in[count][0]);
			format_in[count][0] = re.sub('(\*+\++)+', '*', format_in[count][0]);
			format_in[count][0] = re.sub('(\*\*+)+', '*', format_in[count][0]);
			format_in[count][0] = re.sub('(\+\++)+', '+', format_in[count][0]);
		count+=1
	#print(format_in)
	return format_in

##
## @brief      jednoduchá syntaktická analíza vstupných regexov
##
## @param      format_in > pole vstupných regexov
##
## @return     vráti upravený vstupný regex vo formáte použiteľnom 
## 			   ako python regex
##
def regex(format_in):
	count=0
	for form in format_in:
		state='start'
		convS=''
		neg=False
		for f in range(0,len(form)):
			if state == 'start':
				if form[f] == '%':
					state = 'mod'
				elif form[f] == '.':
					continue
				elif form[f] == '(':
					if f+1 < len(form):
						if form[f+1] in '.|':
							errors(4)
					else:
						errors(4)
					convS+=form[f]
				elif form[f] == '!':
					if form[f+1] in '.|)+*':
						errors(4)
					if neg == False:
						neg=True
					else:
						errors(4)
					if form[f+1] == '(':
						state='neg_in'
				else:
					if neg == True:
						convS+='[^' + form[f] + ']'
						neg = False
					else:
						if form[f] in '[{\^$?\\\"\'}]' :
							convS += '\\'
						convS += form[f]
					#print(convS)
				#print(state)
			elif state == 'mod':
				if form[f] in 'asdlLwWtn':
					if form[f] != 'a' :
						convS+='['
						if neg == True:
							convS+='^'
							neg=False
						specials=['\t\n\r\f\v ','0-9','a-z','A-Z','a-zA-Z','a-zA-Z0-9','\t','\n']
						convS+=specials['sdlLwWtn'.find(form[f])]
						convS+=']'
					else :
						if neg == True:
							convS+='(?!.)'
							neg=False
						else:
							convS+='.'
						convS+='.'
				else:
					if form[f] in '!.()*+|%':
						if neg == True:
							convS+='[^'
						else:
							convS+='\\'
						convS+=form[f]
						if neg == True:
							convS+=']'
							neg=False
					else:
						errors(4)
				state='start'
			elif state == 'neg_in':
				convS+='[^(]'
				if form[f+1] not in '.|!+*%':
					errors(4)
				state='start'
			else:
				errors(4)
		try:
			re.compile(convS)
		except:
			errors(4)

		format_in[count]=convS
		count+=1
	#print(format_in)
	return format_in

##
## @brief      funkcia prepisu vstupného súboru podla formátovacieho súboru
##
## @param      format_in  pole vstupných regexov
## @param      format_do  pole typu formátu pre dané regexy
## @param      input_in   vstupný súbor
## @param      args       argumenty z funkcie defArgs
##
## @return     vracia výstupný string (podla pravidiel upravený vstupný súbor)
##
def out(format_in,format_do,input_in,args):
	input_help=[]
	count=0
	index=None
	for inp in range(0,len(input_in)):
		input_help.append(input_in[count])
		count+=1
	count=0
	for form in format_in:
		index = re.finditer(form,input_in,re.DOTALL)
		for catch in index:
			if catch.start() != catch.end():
				index_first = catch.start()
				index_last = catch.end()-1
				for form_d in format_do[count] :
					inputer=None
					if form_d == 'bold':
						outputer = inputer='b'
					elif form_d == 'italic':
						outputer = inputer='i'
					elif form_d == 'underline':
						outputer = inputer='u'
					elif form_d == 'teletype':
						outputer = inputer='tt'
					elif re.match('color:([0-9a-fA-F]{6})',form_d):
						inputer = 'font color=#'+form_d[6:]
						outputer = 'font'
					elif re.match('size:([1-7]{1})',form_d):
						inputer = 'font size='+form_d[5:]
						outputer = 'font'
					else:
						errors(4)
					count3=0
					while input_help[index_first][count3] == '<':
						while input_help[index_first][count3] != '>':
							count3+=1
						count3+=1

					#print(input_help[index_first][-count3:])
					input_help[index_first]=input_help[index_first][:count3]+'<'+inputer+'>'+input_help[index_first][count3:]
					
					count3=0
					while input_help[index_last][count3] == '<':
						while input_help[index_last][count3] != '>':
							count3+=1
						count3+=1
					count3+=1
					input_help2=input_help[index_last][:count3]+'</'+outputer+'>'+input_help[index_last][count3:]
					input_help[index_last]=input_help2
		count+=1

	if args.nooverlap == True:
		input_help=regex_nooverlap(input_help)
	input_output=''
	for i in input_help : 
		input_output+=i
	if args.escape == True:
		input_output=regex_escape(input_output)
	#print(input_output)
	return input_output

##
## @brief      rozsirenie HTM argument --escape
##
## @param      input_output  vystupny string (upraveny vstupny subor)
##
## @return     vrati upraveny vstupny string
##
def regex_escape(input_output):
	input_output = re.sub('&', '&amp', input_output);
	input_output = re.sub('<', '&lt', input_output);
	input_output = re.sub('>', '&gt', input_output);
	return input_output

##
## @brief      rozsirenie HTM argument --nooverlap
##
## @param      input_help  upraveny vstupny string z funkcie out() vo formate pola
## 			   kazdy prvok predstavuje jeden char vystupneho stringu
##
## @return     upravene pole predstavujuce string vystupu so zamedzenym prekryvanim
##
def regex_nooverlap(input_help):
	start=[]
	start_2=[]
	arrayc=0
	input_halt=input_help
	for he in input_help:
		helps = re.finditer('<[a-z0-9 =#A-Z]+>',input_help[arrayc])
		for hel in helps:
			helpss=''
			count=hel.start()+1
			while count != hel.end()-1:
				helpss+=he[count]
				count+=1
			if helpss not in start_2:
				if helpss != 'font':
					start.append(helpss)
					start_2.append(helpss)

		helps2 = re.finditer('</[a-z]+>',input_help[arrayc])
		
		tryhard=[]
		for hel2 in helps2:
			trolo=hel2.start()+2
			trotl=''
			while trolo < hel2.end()-1:
				trotl+=input_help[arrayc][trolo]
				trolo+=1
			tryhard.append(trotl)
		#print(tryhard)	

		helps2 = re.finditer('</[a-z]+>',input_help[arrayc])
		for hel2 in helps2:
			helpss=''
			count=hel2.start()+2
			while count != hel2.end()-1:
				helpss+=he[count]
				count+=1
			if helpss == 'font':
				fontcounter=len(start)-1
				while re.match('font [a-z]+=',start[fontcounter]) == None:
					fontcounter-=1
				helpss = start[fontcounter]
			count11=1
			if helpss != start[-1]:
				counter=hel2.start()-1
				while helpss != start[-count11]:
					fontcon=start[-count11]
					if re.match('font [a-z]+=',fontcon):
						fontcon='font'
					count3=0
					tod=''
					counter2=counter
					while count3 <= counter:
						tod+=input_help[arrayc][count3]
						count3+=1
					counter=counter+3+len(fontcon)
					
					tod+="</"+fontcon+">"+input_help[arrayc][counter2+1:]
					input_halt[arrayc]=tod

					if fontcon not in tryhard:
						#print(fontcon)
						todo="<"+start[-count11]+">"+input_help[arrayc+1]
						input_halt[arrayc+1]=todo
					count11+=1

				start=start[:len(start)-count11]+start[len(start)-count11+1:]
			else:
				if re.match('font [a-z]+=',start[-1]) == None:
					input_halt[arrayc]=input_halt[arrayc][:len(input_halt[arrayc])-len(start[-1])-3]
				#print(input_halt[arrayc+1])
				start.pop()
				
		arrayc+=1
	input_help=input_halt
	#print(input_help)
	return input_help	
def main(argv):
	args=defArg(argv)
	formatFile=None
	fortmat_in=None
	output_in=None
	inputFile=sys.stdin
	outputFile=sys.stdout
	
	#print(args.input)
	if args.input != None:
		try:
			inputFile=open(args.input,'r')
		except:
			errors(2)
	input_in=inputFile.read()
	if args.output != None:
		try:
			outputFile=open(args.output,'w')
		except:
			errors(3)
	if args.format != None:
		try:
			formatFile=open(args.format,'r')	
		except:
			output_in=input_in
	else:
		output_in=input_in
	#print(formatFile)
	if output_in == None:
		format_in=format(formatFile)
		if not format_in :
			output_in=input_in
		else:
			format_do = []
			count = 0
			for format_i in format_in:
				format_do.append(format_i[1].split(","))
				format_in[count]=format_i[0]
				count+=1
	
			format_in=regex(format_in)
			output_in=out(format_in,format_do,input_in,args)

	if args.br != False:
		output_in = output_in.replace('\n', '<br />\n')
	outputFile.write(output_in)
	outputFile.close()
	inputFile.close()
	exit(0)
if __name__ == "__main__":
	main(sys.argv)