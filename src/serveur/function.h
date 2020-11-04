/*

namefile : function.h

purpose : headerfile for module function

created by : Francois Suzeau

date : 28/10/2020

*/

#ifndef FUNCTION
#define FUNCTION

    

    /*********************************************************************************************************************/
    /*                                                      includes                                                     */
    /*********************************************************************************************************************/
    
        #include <stdlib.h>
        #include <stdio.h>
        #include <string.h>
        #include <unistd.h>
        #include <sys/types.h>
        #include <sys/wait.h>
        #include <sys/socket.h>
        #include <netinet/in.h>
        #include <netinet/ip.h>
        #include <arpa/inet.h>
        #include <netdb.h>
        #include <errno.h>
        #include <sys/stat.h>
        #include <fcntl.h>

        #include <math.h>
        
        //local includes
        #include "../constant/constant.h"

    /*********************************************************************************************************************/
    /*                                              variables needed                                                     */
    /*********************************************************************************************************************/

        typedef struct board {
            int     **map_pawn;
            int     width;
            int     height;
            int    color;
        } board;

    /*********************************************************************************************************************/
    /*                                                  prototypes                                                       */
    /*********************************************************************************************************************/
        void initialize_map(board *manBoard);
        void destroy_map(board *manBoard);

        void create_account(char *name, int num_file);
        void handle_connection(int *sk, int *connect_fd, int *sk_accept, struct sockaddr_in *cli, socklen_t *add_size);
        int handle_logIn(char *mrecv, int sk_accept, int nb_connection);
        int launch_party(int *sk_accept);
        int handle_exitStatus(int status);
        void handle_turn(int *sk_accept, board *manB);
        int checkMove(board *manB, char *coords);
        int checkTake(board *manB, char *coords);
        void sendValidateMove(int exit_status, char *coords, board *manB, int *sk_accept);
        void sendValidateTake(int exit_status, char *coords, board *manB, int *sk_accept);
        int handle_exitTurn(int status, board *manB);
#endif