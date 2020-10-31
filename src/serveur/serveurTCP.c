/*namefile : serveurTCP.c

purpose : the main file

created by : Francois Suzeau

date : 28/10/2020

*/

#include "function.h"

int main(int argc, char *argv[], char ** arge)
{
	(void) argc;
	(void) argv;
	(void) arge;

    char buf[MAX];
    struct sockaddr_in ser, cli;                                                              
    int sk, ret,count, sk_accept, connect_fd, continuer, status, exit_status, nb_connection;
    socklen_t add_size;

    memset(&ser, 0, sizeof(struct sockaddr_in));
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

    add_size = sizeof(ser);

    pid_t son_pid;

    nb_connection = 0;
    continuer = TRUE;
    
    while (continuer)
    {
        if(nb_connection < 3)
        {
            if((connect_fd = listen(sk, 3)) == -1)
            {
                perror("Serveur : Failure listen function : \n");
                exit(EXIT_FAILURE);
            }
            if((sk_accept = accept(sk, (struct sockaddr *)&cli, &add_size)) == -1)
            {
                perror("Serveur : Failure accept function : \n");
                exit(EXIT_FAILURE);
            }
            else
            {
                printf(">>> Serveur has a new connexion : %s\n", inet_ntoa(cli.sin_addr));
                nb_connection++;
            }
        }
        
        son_pid = fork();
        
        switch(son_pid)
        {
            case -1:
                perror("Error fork failure");
                exit(EXIT_FAILURE);
                break;
            case 0:
                puts("Serveur : Input info:>>>");
                buf[0] = '\0';
                if((count = recv(sk_accept, buf, sizeof(buf), 0)) == -1)
                {
                    perror("Serveur : receive data failure : \n");
                }
                else 
                {
                    if(nb_connection < 4)
                    {
                        exit_status = handle_logIn(buf, sk_accept, nb_connection);
                        exit(exit_status);
                    }
                    else if(strcmp(buf, "quit"))
                    {
                        exit(0);
                    }
                }
                break;
            default:
                wait(&status);
                continuer = handle_exitStatus(status);
        }
    }

    
    shutdown(sk_accept, sk);
    close(sk_accept);
    close(sk);

    return EXIT_SUCCESS;
}