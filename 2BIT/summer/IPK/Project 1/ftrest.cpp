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
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <unistd.h>

#include "struct.c"

#define DEL 1000
#define PUT 1001
#define MKD 1002
#define RMD 1003
#define GET 1004
#define LST 1005

#define BUFSIZE 1024
int main (int argc, char * argv[]) {
    int client_socket, port_number, bytestx, bytesrx;
    socklen_t serverlen;
    const char *server_hostname;
    struct hostent *server;
    struct sockaddr_in server_address;
    char buf[BUFSIZE];
    

    if (argc < 3 || argc > 4)
    {
        fprintf(stderr, "ERROR args");
        exit(1);
    }

    char *meth;char *path; 
    meth=argv[1];
    path=argv[2];
     
    char *server_s;


    if (sscanf(path, "http://%s", path) != 1) {
        fprintf(stderr, "Wrong path");
        exit(1);
    }

    char *port_number_S;
    server_s=strtok(path,":");
    port_number_S=strtok(NULL,"/");
    port_number=atoi(port_number_S);
    path=strtok(NULL,"\0");
    
    char *path_h=(char *)malloc(sizeof(char)*strlen(path)+1);
    sprintf(path_h, "/%s", path);
    path=path_h;
    char *meth_h=(char *)malloc(sizeof(char)*3);

    port_number = atoi(port_number_S);
    int action;
    server_hostname=server_s;
    int size=100;
    char *resp=(char *)malloc(sizeof(char) * size);

    if(strcmp(meth,"get")==0)
    {
        action=GET;
        sprintf(meth_h, "%s", "GET");
    }
    else if(strcmp(meth,"put")==0)
    {
        if(argc!=4)
            exit(1);
        char *path_S;
        path_S=argv[3];


        char *Path = (char *) malloc(sizeof(char) * (strlen("./") + strlen(path_S) + 1));
        sprintf(Path, "%s%s%s", "./",path_S[0]=='/'?"":"/", path_S);
        FILE *file;
        file=fopen(Path,"r");
        char send[size];
        
        while(fgets(send,size-1,file)!=NULL)
                {
                    size+=100;
                    resp=(char *)realloc(resp,sizeof(char) * size);
                    sprintf(resp,send);
                }
        fclose(file);

        action=PUT;
        sprintf(meth_h, "%s", "PUT");
    }
    else if(strcmp(meth,"mkd")==0)
    {
        action=MKD;
        sprintf(meth_h, "%s", "MKD");
    }
    else if(strcmp(meth,"lst")==0)
    {
        action=LST;
        sprintf(meth_h, "%s", "LST");
    }
    else if(strcmp(meth,"rmd")==0)
    {
        action=RMD;
        sprintf(meth_h, "%s", "RMD");
    } 
    else if(strcmp(meth,"del")==0)
    {
        action=DEL;
        sprintf(meth_h, "%s", "DEL");
    }
    else
    {
        fprintf(stderr, "%s\n","unknown client action" );
        exit(1);
    }

    if ((server = gethostbyname(server_hostname)) == NULL) {
        fprintf(stderr,"ERROR: no such host as %s\n", server_hostname);
        exit(EXIT_FAILURE);
    }

    char *time=time_string();
    size_t H_size=strlen("HTTP/1.1")+strlen("Host:")+strlen("Accept-Encoding:")+strlen("Content-length:")+strlen("Date:")+strlen("Accept: text/plain")
    +strlen("Content-type: application/octet-stream")+strlen(meth)+strlen(server_s)+strlen(time)+strlen(path);
    H_size+=25;
    if(action==PUT)
        H_size+=strlen(resp);
    char *head_S=(char *)malloc(sizeof(char)*H_size);
    sprintf(head_S, "%s %s?type=%s %s\n%s %s\n%s %s%s\n%s %s\n%s\n%s %li\n\n",meth_h,path,"type","HTTP/1.1","Host:",server_s,"Date:",time,
                            "Accept: text/plain","Accept-Encoding:","identity","Content-Type: application/octet-stream","Content-Length:",H_size);

    
    bzero((char *) &server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    bcopy((char *)server->h_addr, (char *)&server_address.sin_addr.s_addr, server->h_length);
    server_address.sin_port = htons(port_number);
   
    

    if ((client_socket = socket(AF_INET, SOCK_STREAM, 0)) <= 0)
    {
        perror("ERROR: socket");
        exit(EXIT_FAILURE);
    }
        
   
    
    if (connect(client_socket, (const struct sockaddr *) &server_address, sizeof(server_address)) != 0)
    {
        perror("ERROR: connect");
        exit(EXIT_FAILURE);        
    }

    bytestx = send(client_socket, head_S, strlen(head_S), 0);
    if (bytestx < 0)
      perror("ERROR in sendto");

    Header *response=getH(client_socket,CLIENT);

    switch(action){
        case GET :
        {
            char *Path = (char *) malloc(sizeof(char) * (strlen("./") + strlen(path) + 1));
            sprintf(Path, "%s%s%s", "./",path[0]=='/'?"":"/", path);

            char *g_file=(char *)malloc(sizeof(char)*response->length);

                recv(client_socket, g_file,response->length,0);

            FILE *file;
            file=fopen(Path,"w");
            fputs(g_file,file);
            fclose(file);
            break;
        }
        case PUT :
        {
            if(response->resp==200)
                send(client_socket, resp, strlen(resp), 0);
            else
                exit(response->resp);
            break;
        }
        default:
            break;

    }

        
    close(client_socket);
    return 0;
}
