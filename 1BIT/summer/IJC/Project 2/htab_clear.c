/*
*       Subor : htab_clear.c
*       Riešenie IJC-DU2, priklad 2
*       Autor: Peter Grofčík, FIT 1BIT
*/

#include "htable.h"
	//funkcia uvolni vsetky prvky zoznamu
	void htab_clear(htab_t * t)
	{
		if (t!=NULL)
		{
			htab_listitem_list * fst_list = NULL;
			unsigned long int i=0;

			while(i<(t->htab_size))
			{
				while((fst_list=t->ptr[i])!=NULL)
				{
					htab_remove(t,fst_list->key);
				}
				i++;
			}
		}


	}
