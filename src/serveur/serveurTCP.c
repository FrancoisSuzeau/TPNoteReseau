/*namefile : serveurTCP.c

purpose : the main file

created by : Francois Suzeau

date : 28/10/2020

*/#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>

#include "../client/constant/constant.h"

int main(int argc, char *argv[], char ** arge)
{
	(void) argc;
	(void) argv;
	(void) arge;

	int sk;
    char buf[MAX];
    struct sockaddr_in ser;                                                              
    int ret,count;
    int connect_fd;
    int sk_accept;
    socklen_t add_size;

    ser.sin_family = AF_INET;
    ser.sin_port = htons(12345);

    ser.sin_addr.s_addr = htonl(INADDR_ANY);

    if((sk = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Serveur : Failure create socket : \n");
        exit(EXIT_FAILURE);
    }
    else if((ret = bind(sk, (struct sockaddr *)&ser, sizeof(ser))) < 0)
    {
        perror("Serveur : Failure create bind : \n");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < 8; i++)
    {
        ser.sin_zero[i] = 0;
    }

    int status;
    add_size = sizeof(ser);
    int continuer = TRUE;

    pid_t son_pid;

    int nb_connection = 0;
    
    while (continuer)
    {
        
        if((connect_fd = listen(sk, 5)) == -1)
        {
            perror("Serveur : Failure listen function : \n");
            exit(EXIT_FAILURE);
        }
        if((sk_accept = accept(sk, (struct sockaddr *)&ser, &add_size)) == -1)
        {
            perror("Serveur : Failure accept function : \n");
            exit(EXIT_FAILURE);
        }
        else
        {
            puts(">>> Serveur has a new connexion");
        }
        
        son_pid = fork();
        
        switch(son_pid)
        {
            case -1:
                perror("Error fork failure");
                exit(EXIT_FAILURE);
            case 0:
                puts("Serveur : Input info:>>>");

                if((count = recvfrom(sk_accept, buf, sizeof(buf), 0, NULL, NULL)) == -1)
                {
                    perror("Serveur : receive data failure : \n");
                }
                else 
                {
                    printf("Serveur : Here is the message receive : %s\n", buf);
                    if(!strcmp(buf, "quit"))
                    {
                        exit(0);
                    }
                    else
                    {
                        printf("%d\n", nb_connection);
                        exit(1);
                    }
                    
                }
            default:
                wait(&status);
                if(WEXITSTATUS(status) == 0)
                {
                    puts("The son has finished well");
                    continuer = FALSE;
                }
                else if(WEXITSTATUS(status) == 1)
                {
                    puts(">>> making new slave");
                    nb_connection++;
                    printf("%d\n", nb_connection);
                    close(sk_accept);
                }
        }
    }

    
    close(sk);

    return EXIT_SUCCESS;
}