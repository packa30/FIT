/**
 * Predmet : IPK
 * Autor : Peter Grofčík
 * Login : xgrofc00
 */

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

#define CLIENT 0
#define SERVER 1

typedef struct HeaderS{
	char *meth;
	char *path;
	char *type;
	char *date;
	char *host;
	char *accept;
	char *encode;
	size_t length;
	int resp;
}Header;

Header *getH();
char *time_string();



Header *getH(int socket,int CLIENT_SERVER)
{
	int size=100;
	char *answer=(char *)malloc(sizeof(char)*size);
	char buffer[2];
	int end='\0';
	answer[0]='\0';answer[1]='\0';
	ssize_t recv_n;

	while(1)
	{
		bzero(buffer,2);
		recv_n=recv(socket,buffer,1,0);

		if(recv_n < 0)
		{
            exit(1);
		}
		if(strlen(answer)>size-10)
		{
			size+=100;
			answer=(char *)realloc(answer,sizeof(char)*size);
		}
		answer[strlen(answer)]=buffer[0];
		answer[strlen(answer)+1]='\0';
		if(end=='\n' && buffer[0]=='\n')
			break;

		end=buffer[0];
	}

	printf("%s",answer);



	Header *head = (Header*)malloc(sizeof(Header));
	
	char *help;

	

	help=strtok(answer,"\n");

	answer=strtok(NULL,"\0");




	if(CLIENT_SERVER==SERVER)
	{

		head->meth=strtok(help," ");

		head->path=strtok(NULL,"?");

		strtok(NULL,"=");
		head->type=strtok(NULL," ");



	}
	else
	{
		strtok(help," ");
		char *error_s=strtok(NULL," ");
		if(strcmp(error_s,"200")==0)
			head->resp=200;
		else
			head->resp=400;
	

	}
	
	help=strtok(answer,"\n");


	char *key;char *size_k;
	while(1)
	{

		
		answer=strtok(NULL,"\0");

		key=strtok(help,": ");
	
		size_k=strtok(NULL,"\0");

		if(strcmp("Head",key)==0)
			head->host=size_k;
		else if(strcmp("Date",key)==0)
			head->date=size_k;
		else if(strcmp("Accept",key)==0)
			head->accept=size_k;
		else if(strcmp("Accept-Encoding",key)==0)
			head->encode=size_k;
		else if(strcmp("Content-Length",key)==0)
		{
			head->length=(size_t)size_k;
		}
		help=strtok(answer,"\n");
		
		
		if(help==NULL)
		{
			break;
		}
	}


	return head;
}


char *time_string() {
    time_t date = time(NULL);
    char *time_to_string;

    if (date == ((time_t) -1)) {
        fprintf(stderr, "Problem with current time\n");
        exit(1);
    }

    time_to_string = ctime(&date);
    if (time_to_string == NULL)
     {
        fprintf(stderr, "Problem with current time\n");
        exit(1);
    }
    return time_to_string;
}