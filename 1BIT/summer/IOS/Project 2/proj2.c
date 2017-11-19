/**
*      Subor : proj2.c
*      Riešenie IOS, projekt c.2
*      Autor: Peter Grofčík, FIT 1BIT
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <ctype.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string.h>

//Pomocna funkcia na overenie ze jednotlive argumenty su skutocne cisla
int integer(const char *string)
{
        for (int f=0;f < (int)(strlen(string));f++)
        {

                if (string[f] > '9' || string[f] < '0')
                {
                        return 0;
                }
        }
        return 1;
}

//Pomocna funkcia na overenie spravnosti (vyplivajucej zo zadania) vstupnych argumentov
int argumenty(int argc, char* argv[],int *p,int *c,int *pt,int *rt)
{
	if(argc==5 && integer(argv[1]) && integer(argv[2]) && integer(argv[3]) && integer(argv[4]))
	{
		*p=strtol(argv[1],NULL,10);
		*c=strtol(argv[2],NULL,10);
		*pt=strtol(argv[3],NULL,10);
		*rt=strtol(argv[4],NULL,10);
		if(*p>0 && *c>0 && *p>*c && (*p%*c)==0 && *pt>=0 && *pt<5001 && *rt>=0 && *rt<5001)
			return 0;
		else
			return 1;
	}
	else
		return 1;
}



int main(int argc, char* argv[])
{
	int p,c,pt,rt;
	if(argumenty(argc,argv,&p,&c,&pt,&rt))
	{
		fprintf(stderr,"Spatne argumenty\n");
		exit(1);
	}

	FILE *file;
	if((file = fopen("proj2.out","w"))==NULL)
	{
		fprintf(stderr,"Nepodarilo sa otvorenie suboru\n");
		exit(2);
	}

	//inicializacia zdielanych pameti
	int *A;
        A = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        *A=1;

	int *nas;
        nas = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        *nas=1;

	int *board;
        board = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        *board=1;

	int *unboard;
        unboard = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED | MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        *unboard=1;


	//inicializacia semaforov
	#define shmSIZE sizeof(sem_t)

        sem_t *sem;
        sem = mmap(NULL, shmSIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        sem_init(sem, 1, 0);

	sem_t *sem1;
        sem1 = mmap(NULL, shmSIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        sem_init(sem1, 1, 0);

	sem_t *sem3;
	sem3 = mmap(NULL, shmSIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
	sem_init(sem3, 1, 0);

	sem_t *sem4;
        sem4 = mmap(NULL, shmSIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        sem_init(sem4, 1, 0);

	sem_t *sem5;
        sem5 = mmap(NULL, shmSIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        sem_init(sem5, 1, 0);

	sem_t *sem6;
        sem6 = mmap(NULL, shmSIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        sem_init(sem6, 1, 0);

	sem_t *sem7;
        sem7 = mmap(NULL, shmSIZE, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
        sem_init(sem7, 1, 0);

	long i;
	int passangers,passanger,random_time,random_time2,hlavny,car;
	char name;

	hlavny=fork();//pomocny proces na vytvorenie procesu passanger
	if(hlavny!=0)
	{
		car=fork();//vytvorenie procesu pre car
		if(car!=0)
                {
                        while(wait(NULL)>0);
                        exit(0);
                }
		else
		{
			i=1;
			name='C';
		}
	}

	if (hlavny==0)
	{
		passanger=fork();//pomocny proces na tvorbu jednotlivych passangers
		if(passanger==0) //v nahodnych casovych intervaloch
		{
			for (i=1; i<=p; i++)
			{
    				random_time = rand() % (pt+1);
    				usleep(random_time*1000);

		   		passangers = fork();
	    			if (passangers==0)
				{
					name='P';
					break;
				}
    			}

			if(passangers!=0)
			{
				while(wait(NULL)>0);
				exit(0);
			}

		}
		else
                {
                        while(wait(NULL)>0);
                        exit(0);
                }

	}


	if(car==0 && hlavny!=0)//v pripade car procesu
        {
                fprintf(file," %i	: %c %li	: started\n",*A,name,i);
		fflush(file);
		*A+=1;
		    for(int d=0;d<p/c;d++)
                {
                        fprintf(file," %i	: %c %li	: load\n",*A,name,i);//car sa startne,loadne a vysle signal
                        fflush(file);						     //pre pokracovanie procesov passangers
			*A+=1;

			sem_post(sem);
                        sem_wait(sem3);

                        fprintf(file," %i	: %c %li	: run\n",*A,name,i);//po uspesnom loadingu sa car vyda na run
                        fflush(file);						    //s nahodnym casovim intervalom
			*A+=1;

			random_time2 = rand() % (rt+1);
			usleep(random_time2*1000);

                        fprintf(file," %i	: %c %li	: unload\n",*A,name,i);//po ukonceni run vysle signal pre unload pre passagers
			fflush(file);						       //a caka az vsetci vystupia nez pokracuje na load alebo finish
                        *A+=1;

			sem_post(sem1);
			sem_wait(sem4);
                }
	 }


	if(passangers==0 && hlavny==0)
	{

		sem_wait(sem);

		fprintf(file," %i	: %c %li	: started\n",*A,name,i);
		fflush(file);
		*A+=1;

		fprintf(file," %i	: %c %li	: board\n",*A,name,i);//jednotlive procesy sa boarduju az po obdrzani signalu load
		fflush(file);
		*A+=1;

//vyhodnotenie jednotlivych moznych vypisov na zaklade poradia procesov passangers pri boardingu
		if(c==1)
		{
			fprintf(file," %i	: %c %li	: board order %i\n",*A,name,i,*nas);
                        fflush(file);
                        *A+=1;

			fprintf(file," %i	: %c %li	: board last\n",*A,name,i);
                        fflush(file);
                        *A+=1;

		}
		else if(*board<c)
                {
                	fprintf(file," %i	: %c %li	: board order %i\n",*A,name,i,*nas);
                	fflush(file);
			*A+=1;
                }
                else
                {
                	fprintf(file," %i	: %c %li	: board last\n",*A,name,i);
                	fflush(file);
			*A+=1;
                }

		if(*board<c)
		{
			sem_post(sem);
		}

		if(*board==c)
		{
			*nas=1;
			*board=1;
			sem_post(sem3);
		}
		else
		{
			*board+=1;
			*nas+=1;
		}

		sem_wait(sem1);//jednotlive procesy passangers ocakavaju pokyn pre unload
		usleep(random_time*1000);

		fprintf(file," %i	: %c %li	: unboard\n",*A,name,i);
		fflush(file);
		*A+=1;
//vyhodnotenie jednotlivych moznych vypisov na zaklade poradia procesov passangers pri unboardingu
		if(c==1)
		{
			fprintf(file," %i	: %c %li	: unboard order %i\n",*A,name,i,*nas);
                        fflush(file);
                        *A+=1;

			fprintf(file," %i	: %c %li	: unboard last\n",*A,name,i);
                        fflush(file);
                        *A+=1;

		}
		else if(*unboard<c)
		{
			fprintf(file," %i	: %c %li	: unboard order %i\n",*A,name,i,*nas);
                        fflush(file);
			*A+=1;
		}
		else
                {
			fprintf(file," %i	: %c %li	: unboard last\n",*A,name,i);
                        fflush(file);
			*A+=1;
		}

		if(*unboard<c)
		{
			sem_post(sem1);
		}

		if(*unboard==c)
                {
			*nas=1;
                        *unboard=1;
                        sem_post(sem4);
                }
		else
		{
			*unboard+=1;
			*nas+=1;
		}

		sem_wait(sem6);//jednotlivy passangers cakaju az vsetci dokoncia svoju ulohu a proces car sa ukonci
	}

fprintf(file," %i	: %c %li	: finished\n",*A,name,i);
fflush(file);
*A+=1;
	if(car==0 && hlavny!=0)
	{
		for(int g=1;g<=p+1;g++)
                {
			usleep(1000);
                        sem_post(sem6);
		}
	}

	while(wait(NULL)>0);

//zrusenie zdrojov
	fclose(file);

	sem_destroy(sem);
	sem_destroy(sem1);
	sem_destroy(sem3);
	sem_destroy(sem4);
	sem_destroy(sem5);
	sem_destroy(sem6);
	sem_destroy(sem7);

        munmap(nas, sizeof(int));
	munmap(A, sizeof(int));
	munmap(board, sizeof(int));
        munmap(unboard, sizeof(int));

	munmap(sem7, shmSIZE);
	munmap(sem6, shmSIZE);
	munmap(sem5, shmSIZE);
	munmap(sem4, shmSIZE);
	munmap(sem3, shmSIZE);
	munmap(sem, shmSIZE);
	munmap(sem1, shmSIZE);

return 0;



}
