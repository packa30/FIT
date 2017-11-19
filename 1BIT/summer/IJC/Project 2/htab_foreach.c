/*
*       Subor : htab_foreach.c
*       Riešenie IJC-DU2, priklad 2
*       Autor: Peter Grofčík, FIT 1BIT
*/

#include "htable.h"
//funkcia operuje s kazdym prvkom zoznamu
void htab_foreach(htab_t * t, void (*f)(const char * key,unsigned long int value))
{
	unsigned long int i=0;

	while(i<(t->htab_size))
	{
		for(htab_listitem_list * fst_list=t->ptr[i];fst_list!=NULL;fst_list=fst_list->next)
		{
			f(fst_list->key,fst_list->data);
		}

		i++;
	}

}
