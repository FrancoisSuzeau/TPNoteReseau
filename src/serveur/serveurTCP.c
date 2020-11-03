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

    struct sockaddr_in ser, cli;                                                              
    int sk, ret, connect_fd;
    int sk_accept[3] = {0};
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

    board *my_manBoard = malloc(sizeof(board));
    initialize_map(my_manBoard);
    
    handle_connection(&sk, &connect_fd, sk_accept, &cli, &add_size);

    launch_party(sk_accept);
 
    shutdown(sk_accept[0], sk);
    shutdown(sk_accept[1], sk);
    shutdown(sk_accept[2], sk);
    close(sk_accept[0]);
    close(sk_accept[1]);
    close(sk_accept[2]);
    close(sk);

    destroy_map(my_manBoard);
    free(my_manBoard);

    return EXIT_SUCCESS;
}