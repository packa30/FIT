// ppm.c
// Riešenie IJC-DU1 príklad b) 26.3.2016
// Autor: Peter Grofčík FIT 1 BIA

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include "ppm.h"
#include "error.h"


struct ppm * ppm_read(const char * filename)
{
	FILE *Mfile;
	Mfile=fopen(filename,"rb");

	if(Mfile != NULL)
	{
		int xsize,ysize,rgb;
		if ((fscanf(Mfile,"P6 %i %i %i \n",&xsize,&ysize,&rgb) != 3) || xsize <= 0 || ysize <= 0 || rgb != 255)
		{
			warning_msg("Zly vstupny format\n");
			fclose(Mfile);
			return NULL;
		}


		unsigned long rozmer=xsize*ysize*3; 
		struct ppm *My_ppm=NULL;
		if((My_ppm = (struct ppm *) malloc(sizeof(struct ppm)+rozmer))==NULL)
		{
			fclose(Mfile);
			warning_msg("Alokacia neuspesna\n");
			return NULL;

		}
		My_ppm->xsize=xsize;
		My_ppm->ysize=ysize;
		if(fread(My_ppm->data,1,rozmer,Mfile) != rozmer)
		{
			fatal_error("Citanie suboru neuspesne\n");
			free(My_ppm);
			fclose(Mfile);

		}
		fclose(Mfile);
		return My_ppm;
	}
	else
	{
		warning_msg("Subor neexistuje\n");
		return NULL;
	}





}

int ppm_write(struct ppm *p, const char * filename)
{
	FILE *Mfile;
	if((Mfile=fopen(filename,"wb")))
	{
		warning_msg("Chyba pri otvarani suboru\n");
		return -1;
	}
	fprintf(Mfile,"P6\n %i %i\n 255\n",p->xsize,p->ysize);
	fclose(Mfile);
	return 1;

}
