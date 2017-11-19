#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

#define FALSE 1
#define TRUE 0

/*
* IZP 02 - Iteracni vypocty
* Autor: Peter Grofcik, xgrofc00
* Datum: 25.11.2015
* Verze: 1.00
*
*/

char strcmpMY(char *s1);
double taylor_log(double x, unsigned int n);
double cfrac_log(double x, unsigned int n);
double taylor_log_minmax(double x, double eps, unsigned int *d);
double cfrac_log_minmax(double x, double eps, unsigned int *z);

int main(int argc, char *argv[])	//v pripade ze funkcia vyhodnoti zadany argument ako 
{	unsigned int d=0;		//--log > vykona sa vypocet logaritmov 
	unsigned int z=0;		//--iter > vykona sa vypocet iteracii
	double x,min,max,eps;		//--help > vypise popis
	unsigned int n;			//cokolvek ine > vypise ze sa nachadza chyba vo vstupnom parametry
char *s1=argv[1];
switch(strcmpMY(s1))
	{
	case 'l':
		if(argc!=4)
		{
			fprintf(stderr,"Zadal si prilis vela alebo malo argumentov\n");
			break;
		}
		else
		{
		x=strtod(argv[2],0);
		n=strtoul(argv[3],0,0);
		printf("       log(%g) = %.12g\n",x,log(x));
		printf("    cf_log(%g) = %.12g\n",x,cfrac_log(x,n));
		printf("taylor_log(%g) = %.12g\n",x,taylor_log(x,n));
		break;
		}
	case 'i':
		if(argc!=5)
		{
                	fprintf(stderr,"Zadal si prilis vela alebo malo argumentov\n");
			break;
		}
		else
		{
		min=strtod(argv[2],0);
		max=strtod(argv[3],0);
		eps=strtod(argv[4],0);


		printf("       log(%g) = %.12g\n",min,log(min));
		printf("       log(%g) = %.12g\n",max,log(max));

		double cfmin = cfrac_log_minmax(min,eps,&z);
		double cfmax = cfrac_log_minmax(max,eps,&z);
		printf("continued fraction iterations = %d\n",z);
		printf("    cf_log(%g) = %.12g\n",min,cfmin);
		printf("    cf_log(%g) = %.12g\n",max,cfmax);

		double taylormin = taylor_log_minmax(min,eps,&d);
		double taylormax = taylor_log_minmax(max,eps,&d);
		printf("taylor polynomial iterations = %d\n",d);
		printf("taylor_log(%g) = %.12g\n",min,taylormin);
		printf("taylor_log(%g) = %.12g\n",max,taylormax);
		break;

		}
	case'h':
		{
		printf("*\n* IZP 02 - Iteracni vypocty\n* Autor: Peter Grofcik, xgrofc00\n* Datum: 25.11.2015\n* Verze: 1.00\n*\n");
		break;
		}
	default:
		fprintf(stderr,"Zadal si zly argument\npovolene argumenty:\n--log\n--iter\n--help\n");
	}
}


double taylor_log(double x, unsigned int n) 	//funkcia prevedie vypocet logaritmu pomocou taylorovho polynomu
{
if(x==0)
        return -INFINITY;                       //v pripade ze vstupne cislo =$
if(isinf(x)!=0)                                 //v pripade ze vstupne cislo =$
        return INFINITY;
if(0<x && x<1)					//v pripade ze vstupne cislo je od 0 po 1 (okrem 0 a 1 )
	{					//pouzije sa na vypocet algoritmus na zaklade prveho zadaneho vzorca
		double i=1;
		double g=1-x;
		double h=1;
		double j=0;
		while(i<=n)
		{
			h=h*g;
			j=j+(h/i);
			i++;
		}
		return -j;

	}
if(x>=1)					//v pripade ze vstupne cislo je vacsie ako 1
        {					//pouzije sa na vzpocet algoritmus na zaklade druheho zadaneho vzorca
		double i=1;
		double g=(x-1)/x;
		double h=1;
		double j=0;
        	while(i<=n)
		{
			h*=g;
	        	j+=(h/i);
        		i++;
		}
		return j;
	}

else
	return NAN;				//v akomkolvek dalsom pripade(a teda v pripade ze vstupne cislo je zaporne)
}						//nedochadza k vypoctu ale vysledkom je ze nan(a teda ze vysledok neexistuje)



double cfrac_log(double x, unsigned int n)
{
	if(x==0)				//v pripade ze vstupne cislo = 0  > nedochadya k vypoctu ale vysledkom je -infinity
		return -INFINITY;
	if(x<0)					//v akomkolvek dalsom pripade(a teda v pripade ze vstupne cislo je zaporne)
		return NAN;			//nedochadza k vypoctu ale vysledkom je ze nan(a teda ze vysledok neexistuje)
	if(isinf(x)!=0)				//v pripade ze vstupne cislo = infinity > nedochadza k vypoctu ale vysledkom je infinity
		return INFINITY;
	else					//v akomkolvek dalsom inom pripade(a teda ze vstupne cislo je vacsie ako 0)
	{					//pouzije sa na vypocet algoritmus na zaklade vzorca na vypocet zretazeneho zlomku
		double f=0.0;
		double c=0.0;
		double y=(1-x)/(1+x);
		double v=(y*y);
		double e;
		double l;

		e=(2*n)-1;
		l=((n-1)*(n-1));
		while(n>1)
		{
			f=c;
			c=(l*v)/(e-f);

		e-=2.0;
		l-=e;
		n--;
		}
		double r=-((2*y)/(e-c));
		if(r==-0)
			r=0;
		return r;
	}
}


double taylor_log_minmax(double x, double eps,unsigned int *d)	//funkcia pocita pocet iteracii potrebny na vypocet logaritmu pomocou taylorvno polynomu
{
		unsigned int n=0;
		double druhy=0;
		do
		{
			++n;
			druhy=taylor_log(x,n);
		}while(fabs(log(x)-druhy)>eps);			//cyklus sa vykonava pokial je rozdiel predoslej iteracie a terajsej iteracie >= eps

		if(*d<n)
			*d=n;

		return druhy;

}

double cfrac_log_minmax(double x, double eps, unsigned int *z)	//funcia pocita pocet iteracii potrebny na vypocet logaritmu pomocou zretazeneho zlomku
{
	double druhy=0;
	unsigned int n=0;
	do
	{	++n;
		druhy=cfrac_log(x,n);

	}while(fabs(log(x)-druhy)>eps);				//cyklus sa vykonava pokial rozdiel predoslej iteracie a terajsej iteracie >= eps

	if(n>*z)
		*z=n;

	return druhy;


}

char strcmpMY(char *s1)
{
	char *a1="--log",*a2="--iter",*a3="--help";	//funcia porovnava vstupny parameter[1] s 3-mi retazcami
	if(strcmp(s1,a1)==0)				//podla zhody funkcia vracia hodnoty ktore rozhoduju o tom co program vykona v maine
		return 'l';
	if(strcmp(s1,a2)==0)
		return 'i';
	if(strcmp(s1,a3)==0)
		return 'h';
	else
		return FALSE;
}
