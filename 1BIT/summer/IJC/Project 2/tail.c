/*
*       Subor : tail.c
*       Riešenie IJC-DU2, priklad 1
*       Autor: Peter Grofčík, FIT 1BIT
*/

#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdarg.h>

#define CMAX 510

void Warning(const char *fmt, ...);
void Er(const char *fmt, ...);
unsigned long int Lines(const char *string,FILE * file,unsigned long int a,unsigned long int *r);

int main (int argc, char *argv[])
{
	FILE * file=stdin;
	unsigned long int rows=10,r=0;

	switch(argc)
	{
		case 1:
			rows=Lines(argv[2],file,rows,&r);
			break;
		case 2:
			if((file=fopen(argv[1],"r"))==NULL)
			{
				Er("Subor s nazvom '%s' sa nepodarilo otvorit\n",argv[1]);
			}
			rows=Lines(argv[1],file,rows,&r);
			break;
		case 3:
			if(strcmp("-n",argv[1])==0)
			{
				rows=Lines(argv[2],file,0,&r);
			}
			else
				Er("Zly vstupny argument\n");
			break;
		case 4:
			if((file=fopen(argv[3],"r"))==NULL)
                        {
                                Er("Subor s nazvom '%s' sa nepodarilo otvorit\n",argv[3]);
                        }

			 if(strcmp("-n",argv[1])==0)
                        {
                                rows=Lines(argv[2],file,0,&r);
                        }
			else
				Er("Zly vstupny argument\n");
			break;
		default:
			Er("Zle zadane agumenty");
			break;
	}


	rewind(file);
	if(r==rows)
	{
		int c;
		while((c=fgetc(file))!=EOF)
		{

			if(c=='\n')
				printf("\n");
			else
				putchar(c);
		}
	}
	else
	{
		int c,s=0,t=0;
		unsigned long int l=0;
		while((c=fgetc(file))!=EOF)
		{
			if (l>=rows && l<=r)
			{

				if (s<CMAX)
				{
					if(c=='\n')
					{
						printf("\n");
					}
					else
					{
						putchar(c);
					}
				}
				else if(c=='\n')
				{
					printf("\n");
					if(t==0)
					{
						Warning("Bol prekroceny maximalny pocet znakov na riadok (%d)\n",CMAX);
						t++;
					}
					s=0;
				}
				s++;

			if (c=='\n')
			{
				s=0;
			}
			}

			else
			{
				if(c=='\n')
				{
					l++;
					s=0;
				}
			}
		}
	}

	if(argc==2 || argc==4)
		fclose(file);

return 0;
}
unsigned long int Lines(const char *string,FILE * file,unsigned long int a,unsigned long int *r)
{
	int z;
	unsigned long int i=0,vypis;
	if (a==0)
	{

	for (;i < (unsigned long int)(strlen(string));i++)
	{

		if (string[i] > '9' || string[i] < '0')
		{
			Er("Zle zadany pocet riadkov na vypis\n");
		}
	}

	vypis=atoi(string);
		if(vypis==0)
		{
                        Er("Zle zadany pocet riadkov na vypis\n");
                }

	}
	else
	vypis=a;

	while((z=fgetc(file))!=EOF)
	{
		if(z=='\n')
		{
			++*r;
		}
	}
	if(*r>vypis)
		return (*r-vypis);
	else
		return *r;
}

void Er(const char *fmt, ...)
{
	va_list ap;
        va_start(ap, fmt);

	vfprintf(stderr,"CHYBA: ",ap);
	vfprintf(stderr,fmt,ap);

        va_end(ap);
        exit(1);
}

void Warning(const char *fmt, ...)
{
	va_list ap;
        va_start(ap, fmt);

	vfprintf(stderr,"CHYBA: ",ap);
	vfprintf(stderr,fmt,ap);

        va_end(ap);
}
