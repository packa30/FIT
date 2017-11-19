/*
*       Subor : htab_lookup_add.c
*       Riešenie IJC-DU2, priklad 2
*       Autor: Peter Grofčík, FIT 1BIT
*/

#include "htable.h"
#include "hash_function.h"
//funkcia najde prvok alebo vytvori novy podla hash funkcie
htab_listitem_list * htab_lookup_add(htab_t *t,const char *key)
{
	unsigned int hash_idx = t->hash_fun_ptr(key,t->htab_size);
	htab_listitem_list * fst_list = t->ptr[hash_idx];
	htab_listitem_list * tmp_list = NULL;

	//zistovanie klucu v tabulke
	for(tmp_list=fst_list;tmp_list!=NULL;tmp_list=tmp_list->next)
	{
		if(strcmp(tmp_list->key,key)==0)
			break;
	}

	if (tmp_list!=NULL)	//ak je podmienka splnena, nevytvara sa novy prvok zoznamu
		return tmp_list;
	else			//vytvara sa novy prvok
	{
		htab_listitem_list * new_list=(htab_listitem_list *)malloc((sizeof(htab_listitem_list))+(sizeof(char)*(strlen(key)+1)));
		if(new_list!=NULL)	//v pripade vydarenej alokacie
		{
			strcpy(new_list->key,key);
			new_list->next=NULL;
			new_list->data=0;
			t->n++;

			if(fst_list!=NULL)
			{
				for(tmp_list=fst_list;tmp_list->next!=NULL;tmp_list=tmp_list->next)
					;
				tmp_list->next=new_list;
			}
			else
			{
				//najde ukazovatel na posledny obsadeny prvok zoznamu
				t->ptr[hash_idx]=new_list;
			}
			return new_list;
		}
		else
			return NULL;//nevydarena alokacia
	}
}
