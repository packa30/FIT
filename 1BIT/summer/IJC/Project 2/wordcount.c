/*
*       Subor : wordcount.c
*       Riešenie IJC-DU2, priklad 2
*       Autor: Peter Grofčík, FIT 1BIT
*/

#include "io.h"
#include "htable.h"
#include "hash_function.h"

#define MAX 123

void Print(const char * key, unsigned long int data);

int main()
{
	htab_t * table = NULL;
	const unsigned long int size = 700;

	if((table=htab_init(size))!=NULL)
	{
		char s[MAX];
		int chars,warning=0;
		while((chars=get_word(s,MAX,stdin))!=EOF)
		{
			if(((int)strlen(s)<chars) && (warning==0))
			{
				warning=1;
				fprintf(stderr,"Na vstupe sa nachadza dlhsie slovo ako maximum\n");
			}
			htab_listitem_list * fst_list=NULL;
			if((fst_list=htab_lookup_add(table,s))==NULL)
			{
				fprintf(stderr,"Alokacia sa nepodarila\n");
				htab_free(table);
				exit(1);
			}
			fst_list->data+=1;
		}
		htab_foreach(table,Print);
		htab_free(table);
		exit(0);

	}
	else
	{
		fprintf(stderr,"Nepodarila sa alokacia tabulky\n");
		exit(1);
	}
}

void Print(const char * key, unsigned long int data)
{
	printf ("%s\t%ld\n",key,data);
	return;
}

