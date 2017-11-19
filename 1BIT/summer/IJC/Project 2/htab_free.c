/*
*       Subor : htab_free.c
*       Riešenie IJC-DU2, priklad 2
*       Autor: Peter Grofčík, FIT 1BIT
*/

#include "htable.h"

//funkcia zavola htab_clear na odstranenie poloziek, a uvolni tabulku
	void htab_free(htab_t * t)
	{
		if(t!=NULL)
		{
			htab_clear(t);
			free(t);
		}
		return;
	}
