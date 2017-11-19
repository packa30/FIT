/*
*	Subor : htable.h
*	Riešenie IJC-DU2, priklad 2
*	Autor: Peter Grofčík, FIT 1BIT
*/


#ifndef HTABLE
	#define HTABLE

		#include <string.h>
		#include <stdio.h>
		#include <stdlib.h>

		#include "hash_function.h"

	typedef struct htab_listitem
	{
		unsigned long int data;//pocet vyskytov
		struct htab_listitem * next;//ukazatel na dalsi prvok
		char key[];//ukazatel na char
	} htab_listitem_list;

	typedef struct htab
	{
		unsigned long int htab_size;//velkost tabulky
		unsigned (* hash_fun_ptr)(const char *, unsigned);//ukazatel na hashfunction
    		unsigned long int n;//pocet prvkov
		htab_listitem_list * ptr[];
	}htab_t;


	//Prototypy funkcii

	htab_t * htab_init(unsigned long int size);
	htab_t * htab_init2(unsigned long int size, unsigned long int (*hashfunction)(const char *str, unsigned long int htab_size));
	htab_listitem_list * htab_lookup_add (htab_t *t, const char *key);

	void htab_foreach(htab_t *t, void (*f)(const char * key,unsigned long int value));
	void htab_remove(htab_t * t,const char * key);
	void htab_clear(htab_t * t);
	void htab_free(htab_t * t);

#endif
