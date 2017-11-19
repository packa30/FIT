// steg-decode.c
// Riešenie IJC-DU1 príklad b) 26.3.2016
// Autor: Peter Grofčík FIT 1 BIA


#include <limits.h>
#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

#include "bit_array.h"
#include "error.h"
#include "eratosthenes.h"
#include "ppm.h"

#define MAX 5000*5000*3

int main(int argc, char *argv[])
{
	if(argc!=2)
		fatal_error("Chyba v pocte argumentov\n");
	ba_create(p,MAX);
	Eratosthenes(p);
	struct ppm * My_ppm;
		if((My_ppm = ppm_read(argv[1]))==NULL)
		{
			return 1;
		}

	int index=0;
	int failure=1;
	int bitlas;
	unsigned long cykle=2;
	while(cykle<MAX-1)
	{
		if(ba_get_bit(p,cykle)==0)
		{
			DU1_SET_BIT((&bitlas),index,DU1_GET_BIT(&(My_ppm->data[cykle]),0));
			index++;
			if(index==CHAR_BIT)
			{
				if(bitlas=='\0')
				{
					failure=0;
					break;
				}				
				if(!isprint(bitlas))
				{
					free(My_ppm);
					My_ppm=NULL;
					fatal_error("Netisknutelne znaky\n");
				}
			index=0;
			printf("%c",bitlas);
			}
		}



		cykle++;
	}
	if(failure==1)
		fatal_error("Nespravne nacitanie\n");
free(My_ppm);
My_ppm=NULL;
return 0;
}
