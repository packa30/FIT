/*
*       Subor : htab_remove.c
*       Riešenie IJC-DU2, priklad 2
*       Autor: Peter Grofčík, FIT 1BIT
*/

#include "htable.h"

	//funkcia odstrani prvky zo zoznamu
	void htab_remove(htab_t * t,const char * key)
	{

		if(t!=NULL)
		{
			htab_listitem_list * fst_list = t->ptr[hash_function(key,t->htab_size)];//ukazatel na prvy prvok zoznamu
			htab_listitem_list * tmp_list = fst_list;

			while(fst_list!=NULL)
			{
				if(strcmp(fst_list->key,key)==0)//ak bol najdeny ukazatel
				{
					if(tmp_list!=fst_list)//v pripade nerovnosti existuje nasledujuci prvok na ktory sa posunie
					{
						tmp_list->next = fst_list->next;
					}
					else
					{
						t->ptr[hash_function(key,t->htab_size)]=fst_list->next;
					}
					free(fst_list);//uvolnenie ukazovatela
					break;
				}
				else//ak nebol najdeny fst_list odkazuje na dalsi prvok
				{
					tmp_list=fst_list;
					fst_list=fst_list->next;
				}
			}
		}
		return;
	}
