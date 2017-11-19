/**
 * Predmet : IPK
 * Autor : Peter Grofčík
 * Login : xgrofc00
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <sys/stat.h>

#include "struct.c"

   
int main (int argc, const char * argv[]) {
	int rc;
	int welcome_socket;
	struct sockaddr_in6 sa;
	struct sockaddr_in6 sa_client;
	char str[INET6_ADDRSTRLEN];

  	char *root = (char *)malloc(sizeof(char)*5);
  	strcpy(root,"./");
    uint16_t port_number=6677;
    if(argc>5)
    {
    	fprintf(stderr, "Invalid number of args");
        exit(1);
    }
    for(int i=0;i<argc;i++)
    {
    	if(strcmp(argv[i],"-p")==0)
    		port_number=(uint16_t)atoi(argv[i+1]);
    	else if(strcmp(argv[i],"-r")==0)
    		strcpy(root,argv[i+1]);
    }
    
    socklen_t sa_client_len=sizeof(sa_client);
	if ((welcome_socket = socket(PF_INET6, SOCK_STREAM, 0)) < 0)
	{
		perror("ERROR: socket");
		exit(EXIT_FAILURE);
	}
	memset(&sa,0,sizeof(sa));
	sa.sin6_family = AF_INET6;
	sa.sin6_addr = in6addr_any;
	sa.sin6_port = htons(port_number);	
        
    
    
	if ((rc = bind(welcome_socket, (struct sockaddr*)&sa, sizeof(sa))) < 0)
	{
		perror("ERROR: bind");
		exit(EXIT_FAILURE);		
	}
	if ((listen(welcome_socket, 1)) < 0)
	{
		perror("ERROR: listen");
		exit(EXIT_FAILURE);				
	}
	while(1)
	{
		int error=400;
		char *ok=(char*)malloc(sizeof(char)*6);
		ok="OK";

		int comm_socket = accept(welcome_socket, (struct sockaddr*)&sa_client, &sa_client_len);		
		if (comm_socket > 0)
		{	int size_of=100;
			if(inet_ntop(AF_INET6, &sa_client.sin6_addr, str, sizeof(str))) {
			}
			Header *head = getH(comm_socket,SERVER);

			char *Path = (char *) malloc(sizeof(char) * (strlen(root) + strlen(head->path) + 1));
			sprintf(Path, "%s%s%s", root,head->path[0]=='/'?"":"/", head->path);

			char *resp=(char *)malloc(sizeof(char) * size_of);
			
			if (strcmp("MKD", head->meth) == 0)
			{
					error = mkdir(Path, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
					if(error<0)
						error=400;
					else
						error=200;

			}
			else if(strcmp("RMD", head->meth) == 0)
			{
				error = rmdir(Path);
					if(error<0)
						error=400;
					else
						error=200;
			}
			else if(strcmp("GET", head->meth) == 0)
			{
				
				
				FILE *file;


				file=fopen(Path,"r");
				char send[size_of];
				
				while(fgets(send,size_of-1,file)!=NULL)
				{
					size_of+=100;
					resp=(char *)realloc(resp,sizeof(char) * size_of);
					sprintf(resp,send);
				}
				
				fclose(file);
				error=200;

			}
			else if(strcmp("DEL", head->meth) == 0)
			{
				error = remove(Path);
				if(error<0)
						error=400;
					else
						error=200;
			}
			else if(strcmp("PUT", head->meth) == 0)
			{
				FILE *file;

				file=fopen(Path,"w");
				if(file!=NULL)
					error=200;
				else
					error=400;
				fclose(file);

			}

		
			if(error==200)
				ok="OK";
			else if(error==400)
				ok="Bad Request";

			char *time=time_string();
    		size_t H_size=strlen("HTTP/1.1")+strlen("Conted-Encoding:")+strlen("Content-length:")+strlen("Date:")
    				+strlen("Content-type: application/octet-stream")+strlen(time)+strlen(ok)+3;
   			H_size+=25;
    		char *head_S=(char *)malloc(sizeof(char)*H_size);
    		sprintf(head_S, "%s %d %s\n%s %s%s\n%s %li\n%s%s\n\n","HTTP/1.1",error,ok,"Date:",time,
    				"Content-Type: application/octet-stream","Content-Length:",size_of,"Contend-Encoding:","identity");
    			


    		send(comm_socket, head_S, strlen(head_S), 0);


			
    		
    		if(strcmp("GET", head->meth) == 0)
    		{
    			send(comm_socket,resp,size_of,0);
    		}
    		if(strcmp("PUT", head->meth) == 0)
    		{
    			FILE *file;

				file=fopen(Path,"w");
    			char *g_file=(char *)malloc(sizeof(char)*head->length);
    			recv(comm_socket, g_file,head->length,0);
    			fputs(g_file,file);
            	fclose(file);
    		}

	
		}
		else
		{
			printf(".");
		}
		close(comm_socket);
	}	
}
