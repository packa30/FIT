/*
*       Subor : io.c
*       Riešenie IJC-DU2, priklad 2
*       Autor: Peter Grofčík, FIT 1BIT
*/

#include "io.h"

int get_word(char *s,int max, FILE *f)
{
	if(f==NULL)
		return EOF;

	int n_char,o_char=0,c_char=0;//premenne pre pracu so znakmi/nacitanie,odtrhnutie,celkom/

	while((n_char=fgetc(f))!=EOF)
	{
		if(!isspace(n_char))//ak sa nenacita biely znak
			c_char+=1;
		if(!isspace(n_char) && max-1>o_char)//+nie je prekroceny limit znakov
		{
			s[o_char]=n_char;
			o_char+=1;
		}
		if(isspace(n_char) && o_char)//je nacitany biely znak + dalsi znak
			break;

	}

	s[o_char]=0;//+nulovy znak
	if(c_char)//ak bol nacitany aspon jeden znak
		return c_char;
	else
		return EOF;

}
