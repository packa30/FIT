/*
*       Subor : htab_init.c
*       Riešenie IJC-DU2, priklad 2
*       Autor: Peter Grofčík, FIT 1BIT
*/

#include "htable.h"
#include "hash_function.h"

	//funkcia inicializuje tabulku a nastavuje ukazatele na NULL
	htab_t * htab_init(unsigned long int size)
	{	//alokacia tabulky
		htab_t * fst_htab=(htab_t *) malloc((sizeof(htab_t))+size*sizeof(htab_listitem_list));
		if(fst_htab==NULL)
			return NULL;

		fst_htab->htab_size=size;//ulozenie velkosti do struktury
		fst_htab->n=0;
		unsigned long int i=0;
		while(i<size)
		{
			fst_htab->ptr[i]=NULL;//vsetky ukazatele ukazuju na NULL
			i++;
		}
		fst_htab->hash_fun_ptr=hash_function;
		return fst_htab;
	}
