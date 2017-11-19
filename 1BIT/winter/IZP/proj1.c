#include <stdio.h>
#include <stdlib.h>
#define __USE_XOPEN
#include <time.h>
#include <limits.h>

#define TRUE 0
#define FALSE 1

int Palindrome(char itp[101]);
int cislo(char pole[101]);
int prime(int p);
void lotime(char istime[101]);

int main(int argc, char *argv[])
{
if(argv[1]!=0)			//v prĂ­pade Ĺže bude program spustenĂ˝ s argumentom/mi, vypĂ­ĹĄe prvĂ˝ argument a poÄet argumentov a ukonÄĂ­ sa
{
    printf("Arguments found starting with %s\nNumber of arguments %d\nEnd of program\n",argv[1], argc-1);
    exit(0);
}

char pole[101];
	while(scanf("%100s",pole)!=EOF)
	{
		if(!(pole[100]!='\0'))
		{
			fprintf(stderr,"Zadal si prilis velky retazec\n");
			break;
		}

		if(cislo(pole)==FALSE && pole[4]=='-' && pole[7]=='-' && pole[9]!='\0')
		{						 //pokiaÄž je na 4. a 7.pozĂ­cii(prvĂ˝ znak je na pozĂ­cii 0) znak "-" a na 9tej
   		lotime(pole);					 //nie je ukonÄovacĂ­ znak tak sa zavolĂĄ funkcia lotime a predĂĄ sa jej pole
		continue;
		}

		if (cislo(pole)==TRUE)		//pokiaÄž funkcia cislo vyhodnotĂ­ vstup ako ÄĂ­slo  funkcia atoi prevedie vstup na integer
            	{
		int p = atoi(pole);

                    	if((prime(p)==TRUE) & (p!=1)) //ak funkcia prime vrĂĄti TRUE a zĂĄroven vstupnĂ˝ integer p sa nerovnĂĄ 1 jednĂĄ sa o prvoÄĂ­slo
                                printf("number: %d (prime)\n",p);
                        else
                                printf("number: %d\n",p);
		continue;
            	}

		else		//ak nebola splnenĂĄ Ĺžiadna z predoĹĄlĂ­ch podmienok znamenĂĄ to Ĺže vstup je branĂ˝ ako slovo/word
		{
                if(Palindrome(pole) == TRUE)   // pokiaÄž funkcia palindome vrĂĄti hodnotu TRUE jednĂĄ sa o slovo palindrom
        	{
                        printf("word: %s (palindrome)\n",pole);
                }
                else
                        printf("word: %s\n",pole);
		}
	}
}

    int Palindrome(char itp[101])		//funkcia palindrom prevezme  vstup
    {
        int i, l, length=0;
        for(i=0; itp[i]!='\0'; i++)		//funkcia for rĂĄta poÄet znakov aĹž kĂ˝m na nasledujĂşcom mieste nieje ukonÄovacĂ­ znak
            length++;
        i=0;
        l=length-1;
        while(i < length)			//funkcia while  pouĹžĂ­va dva integery priÄom integer l sa rovnĂĄ poÄtu znakov -1 (ukonÄovacĂ­ znak)
        {					//pokraÄuje kĂ˝m je premennĂĄ i menĹĄia ako poÄet znakov
            if(itp[i] != itp[l])		//porovnĂĄva znak na mieste i so znakom na mieste l
            {					//v prĂ­pade Ĺže sa znaky nerovnajĂş>>funkcia palindrome sa preruĹĄĂ­ a vrĂĄti FALSE
                return FALSE;
                break;
            }
            i++;
            l--;
        }
        return TRUE;				//ak sa rovnajĂş aĹž do chvĂ­le kedy i sa rovnĂĄ poÄtu znakov funkcia vrĂĄti TRUE
    }



int cislo(char pole[101])
{
        char *number;
                strtol(pole,&number,10);
            if(number[0]!='\0')				//pokiaÄž funkcia strtol vyhodnotĂ­ vstup ako ÄĂ­slo a zĂĄroveĹ sa na vstupe nenachĂĄdza inĂ˝
                return FALSE;				//mimoÄĂ­selnĂ˝ znak tak funkcia vrĂĄti  TRUE
            if(strtol(pole,&number,10)>INT_MAX)
                return FALSE;				//v prĂ­pade Ĺže sa na vstupe nachĂĄdza aj inĂ˝ znak, alebo danĂŠ ÄĂ­slo prekroÄilo INT_MAX
            else					//funkcia vrĂĄti FALSE
                return TRUE;
}

int prime(int p)		//funkcia prevezme integer
{
	int i=2;
        while(i<=p/2)		//v prĂ­pade Ĺže premenna i je  menĹĄia alebo rovnĂĄ vstupnĂŠmu integeru deleno 2 vykonĂĄ sa cyklus v ktorom sa zistĂ­ Äi
        {			//vstup % i ==0 (Äi po vydeleni vstupu premennou i je ÄĂ­slo bez zvyĹĄku alebo so zvyĹĄkom)
		if (p%i==0)	//ak to ÄĂ­slo vyhodnotĂ­ bez zvyĹĄku tak funkcia prime vrĂĄti FALSE a ukonÄĂ­ sa
                {
                	return FALSE;
                	break;
                }
        	i++;		//tento cyklus sa opakuje dovtedy kĂ˝m premennĂĄ i sa nerovnĂĄ  vstupu/2(Äo je poloviÄka vstupnĂŠho ÄĂ­sla a pokial
        }			//ÄĂ­slo nie je delitelnĂŠ do polky tak od polky tieĹž nie >> uĹĄetrĂ­me Äas na priebeh cyklu)
	return TRUE;		//v prĂ­pade Ĺže nenĂĄjde delitela bez zvyĹĄku vĂĄti TRUE;
}

void lotime(char istime[101])
{
struct tm date;
        char *week[] = {"Sun","Mon","Tue","Wed","Thu","Fri","Sat"};	// strptime podla danej ĹĄtruktĂşry vpĂ­ĹĄe vstup do ĹĄtruktĂşry date
strptime(istime, "%Y-%m-%d", &date);					//nĂĄsledne funkcia loltime vypĂ­ĹĄe dĂĄta podÄža danej struktĂşry date
printf("date: %s %4d-%02d-%02d\n", week[date.tm_wday], 1900+date.tm_year, date.tm_mon+1, date.tm_mday);
}
