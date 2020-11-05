/*namefile : function.c

purpose : src file for module profil where all signature of function are for this module

created by : Francois Suzeau

date : 28/10/2020

*/

#include "function.h"


/************************************************************************************/
/* function : create_account                                                        */
/************************************************************************************/
/* Input : char *, int                                                              */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : This function is call by serveur. Create new account in the file       */
/* corresponding to the connection order of the client                              */
/************************************************************************************/
void create_account(char *name, int num_file)
{
    char nameFile[3][MAX] = {""};
    strcat(nameFile[0], "../../files/infojoueurs/player1.txt");
    strcat(nameFile[1], "../../files/infojoueurs/player2.txt");
    strcat(nameFile[2], "../../files/infojoueurs/viewer.txt");

    int fd_player;
    int count;
    if((fd_player = open(nameFile[num_file -1 ], O_CREAT | O_RDWR, 0777)) == -1)
    {
        printf("create_Account() :  cannot open %s\n", nameFile[num_file - 1]);
        exit(EXIT_FAILURE);
    }

    char tmp[MAX] = "";
    if((count = read(fd_player, tmp, sizeof(tmp))) == -1)
    {
        printf("create_account :  cannot read %s\n", nameFile[num_file - 1]);
        exit(EXIT_FAILURE);
    }

    if(count > 0) //if file already used
    {
        char buf[MAX] = "rm ";
        strcat(buf, nameFile[num_file - 1]);
        system(buf);
        close(fd_player);
        if((fd_player = open(nameFile[num_file-1], O_CREAT | O_RDWR, 0777)) == -1)
        {
            printf("create_Account() :  cannot open %s\n", nameFile[num_file - 1]);
            exit(EXIT_FAILURE);
        }

    }
    
    if((count = write(fd_player, name, strlen(name))) == -1)
    {
        printf("Couldn't write in file : %s\n", nameFile[num_file - 1]);
    }

    close(fd_player);
    
}

/************************************************************************************/
/* function : handle_connection                                                     */
/************************************************************************************/
/* Input : int * x3, struct sockaddr_in *, struct socklen_t *                       */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : this function is call by serveur. It in charge of control the connection*/
/* to the server and the logs of the player. Add a third connection socket for the  */
/* viewer if the players want to.                                                   */
/************************************************************************************/
void handle_connection(int *sk, int *connect_fd, int *sk_accept, struct sockaddr_in *cli, socklen_t *add_size)
{
    
    char buf[MAX] = {""};
    char choice[MAX] = {""};
    int exit_status = 0;
    int status = 0;
    int count = 0;
    int nb_connection = 0;
    int continuer = TRUE;

    pid_t son_pid;
    
    while (continuer)
    {
        if((*connect_fd = listen(*sk, 3)) == -1)
        {
            perror("Serveur : Failure listen function : \n");
            exit(EXIT_FAILURE);
        }
        if((sk_accept[nb_connection] = accept(*sk, (struct sockaddr *)cli, add_size)) == -1)
        {
            perror("Serveur : Failure accept function : \n");
            exit(EXIT_FAILURE);
        }
        else
        {
            printf(">>> Serveur has a new connexion : %s\n", inet_ntoa(cli->sin_addr));
            nb_connection++;
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
                if((count = recv(sk_accept[nb_connection-1], buf, sizeof(buf), 0)) == -1)
                {
                    perror("Serveur : receive data failure : \n");
                }
                else 
                {
                    exit_status = handle_logIn(buf, sk_accept[nb_connection-1], nb_connection);
                    if(nb_connection == 3)
                    {
                        exit(0);
                    }
                    else if(nb_connection == 2)
                    {
                        puts("Serveur : an action may require your attention .... ");
                        while(TRUE)
                        {
                            puts("Do you want someone to watch your game ?");
                            puts(" ----- 1 : Yes");
                            puts(" ----- 2 : No");
                            scanf("%s", choice);
                            if((strcmp(choice, "1") == 0) || (strcmp(choice, "2") == 0))
                            {
                                break;
                            }
                            else
                            {
                                choice[0] = '\0';
                            }
                        }

                        if(strcmp(choice, "2") == 0)
                        {
                            exit(0);
                        }
                    }
                    else
                    {
                        exit(exit_status);

                    }

                }
                break;
            default:
                wait(&status);
                continuer = handle_exitStatus(status);
        }
    }

}

/************************************************************************************/
/* function : handle_logIn                                                          */
/************************************************************************************/
/* Input : char *, int x2                                                           */
/* Output : int                                                                     */
/************************************************************************************/
/* purpose : this function is call by server. It handle the connection status of the*/
/* player and tell him he is connected                                              */
/************************************************************************************/
int handle_logIn(char *mrecv, int sk_accept, int nb_connection)
{
    int count = 0;
    if(mrecv[0] == REGISTRED)
    {
        mrecv[0] = CONNECTED;
        mrecv[1] = nb_connection - 1; //PLAYER1, PLAYER2, VIEWER -> 0 to 2
        mrecv[2] = '\0';
        if((count = send(sk_accept, mrecv, sizeof(mrecv), 0)) == -1)
        {
            perror("serveur : send data failure : ");
            exit(EXIT_FAILURE);
        }
        return 2;
    }
    else
    {
        create_account(mrecv, nb_connection);
        mrecv[0] = CONNECTED;
        mrecv[1] = nb_connection - 1;
        mrecv[2] = '\0';
        if((count = send(sk_accept, mrecv, sizeof(mrecv), 0)) == -1)
        {
            perror("serveur : send data failure : ");
            exit(EXIT_FAILURE);
        }
        return 1;
    }
}
/************************************************************************************/
/* function : launch_party                                                          */
/************************************************************************************/
/* Input : int *                                                                    */
/* Output : int                                                                     */
/************************************************************************************/
/* purpose : this function is call by server. When it call, it send to all client   */
/* connected to launch the party and the graphic interface                          */
/************************************************************************************/
int launch_party(int *sk_accept)
{
    int count = 0;
    char buf[MAX] = {""};
    for(int i = 0; i < 3; i++)
    {
        switch(sk_accept[i])
        {
            case 0: // connection socket (viewer client) does not exist
                break;
            default:
                buf[0] = CANPLAY;
                buf[1] = '\0';
                if((count = send(sk_accept[i], buf, sizeof(buf), 0)) == -1)
                {
                    perror("serveur : send data failure : ");
                    exit(EXIT_FAILURE);
                }
                break;
        }
    }
    
    return 3;
}

/************************************************************************************/
/* function : handle_turn                                                           */
/************************************************************************************/
/* Input : int *, board *                                                           */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : this function is call by server. It handle all a party turn by turn.   */
/* it handle the receive move and send the validate or not also the order to begin  */
/* a turn                                                                           */
/************************************************************************************/
void handle_turn(int *sk_accept, board *manB)
{
    char buf[MAX] = {""};
    char coords[MAX] = {""};
    int status = 0;
    int count = 0;
    int continuer = TRUE;

    pid_t son_pid;
    
    while (continuer)
    {
        
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
                if((count = recv(sk_accept[manB->color], buf, sizeof(buf), 0)) == -1)
                {
                    perror("Serveur : receive data failure : \n");
                }
                else 
                {
                    if(buf[0] == MOVE)
                    {
                        coords[0] = buf[1]; // ancient_x
                        coords[1] = buf[2]; // ancient_y
                        coords[2] = buf[3]; // new_x
                        coords[3] = buf[4]; // new_y
                        coords[4] = buf[5]; // color

                        if(checkMove(manB, coords))
                        {
                            sendValidateMove(4, coords, manB, sk_accept);
                        }
                        else
                        {
                            sendValidateMove(5, coords, manB, sk_accept);
                        }
                    }
                    else if(buf[0] == TAKE)
                    {
                        coords[0] = buf[1]; // ancient_x
                        coords[1] = buf[2]; // ancient_y
                        coords[2] = buf[3]; // new_x
                        coords[3] = buf[4]; // new_y
                        coords[4] = buf[5]; // eaten_x
                        coords[5] = buf[6]; // eaten_y
                        coords[6] = buf[7]; // color

                        if(checkTake(manB, coords))
                        {
                            sendEndGame(manB, sk_accept);
                            sendValidateTake(4, coords, manB, sk_accept);
                        }
                        else
                        {
                            sendValidateTake(5, coords, manB, sk_accept);
                        }

                    }
                    else if(buf[0] == QUIT)
                    {
                        sendFinalQuit(sk_accept, buf);
                    }

                }
                break;
            default:
                wait(&status);
                continuer = handle_exitTurn(status, manB);
        }
    }

}

/************************************************************************************/
/* function : checkMove                                                             */
/************************************************************************************/
/* Input : board *, char *                                                          */
/* Output : int                                                                     */
/************************************************************************************/
/* purpose : this function is call by server and by the son process validate the move*/
/* or not return TRUE if good or FALSE if not                                       */
/************************************************************************************/
int checkMove(board *manB, char *coords)
{
    int bool = FALSE;
    int current_x = coords[0];
    int current_y = coords[1];
    int new_x = coords[2];
    int new_y = coords[3];

    if((current_x != 0) && (current_y != 0) && (new_x != 0) && (new_y != 0))
    {
        if((abs(current_x - new_x) == 1) && (abs(current_y - new_y) == 1)) //player had indeed move on one case to the left or to the right 
        {
            manB->map_pawn[current_y][current_x] = EMPTY;
            switch(coords[4])
            {
                case W:
                    manB->map_pawn[new_y][new_x] = WHITE;
                    break;
                case B:
                    manB->map_pawn[new_y][new_x] = BLACK;
                    break;
            }
            bool = TRUE;
            
        }
    }
    else if((current_x == 0) && (abs(current_y - new_y) == 1))
    {
        manB->map_pawn[current_y][current_x] = EMPTY;
        switch(coords[4])
        {
            case W:
                manB->map_pawn[new_y][new_x] = WHITE;
                break;
            case B:
                manB->map_pawn[new_y][new_x] = BLACK;
                break;
        }
        bool = TRUE;
    }
    else if((new_x == 0) && (abs(current_y - new_y) == 1))
    {
        manB->map_pawn[current_y][current_x] = EMPTY;
        switch(coords[4])
        {
            case W:
                manB->map_pawn[new_y][new_x] = WHITE;
                break;
            case B:
                manB->map_pawn[new_y][new_x] = BLACK;
                break;
        }
        bool = TRUE;
    }
    else if((current_y == 0) && (abs(current_x - new_x) == 1))
    {
        manB->map_pawn[current_y][current_x] = EMPTY;
        switch(coords[4])
        {
            case W:
                manB->map_pawn[new_y][new_x] = WHITE;
                break;
            case B:
                manB->map_pawn[new_y][new_x] = BLACK;
                break;
        }
        bool = TRUE;
    }
    else if((new_y == 0) && (abs(current_x - new_x) == 1))
    {
        manB->map_pawn[current_y][current_x] = EMPTY;
        switch(coords[4])
        {
            case W:
                manB->map_pawn[new_y][new_x] = WHITE;
                break;
            case B:
                manB->map_pawn[new_y][new_x] = BLACK;
                break;
        }
        bool = TRUE;
    }
    return bool;
}

/************************************************************************************/
/* function : checkTake                                                             */
/************************************************************************************/
/* Input : board *, char *                                                          */
/* Output : int                                                                     */
/************************************************************************************/
/* purpose : this function is call by serveur and by the son process, and do the same*/ 
/* things as checkMove but for a take                                               */
/************************************************************************************/
int checkTake(board *manB, char *coords)
{
    int bool = FALSE;
    int current_x = coords[0];
    int current_y = coords[1];
    int new_x = coords[2];
    int new_y = coords[3];
    int eaten_x = coords[4];
    int eaten_y = coords[5];
    
    if((current_x != 0) && (current_y != 0) && (new_x != 0) && (new_y != 0))
    {
        if((abs(current_x - new_x) == 2) && (abs(current_y - new_y) == 2)) //player had indeed move on two case to the left or to the right 
        {
            manB->map_pawn[current_y][current_x] = EMPTY;
            manB->map_pawn[eaten_y][eaten_x] = EMPTY;
            switch(coords[6])
            {
                case W:
                    manB->map_pawn[new_y][new_x] = WHITE;
                    manB->black_pawn_left--;
                    break;
                case B:
                    manB->map_pawn[new_y][new_x] = BLACK;
                    manB->white_pawn_left--;
                    break;
            }
            bool = TRUE;
            
        }
    }
    else if((current_x == 0) && (abs(current_y - new_y) == 2))
    {
        manB->map_pawn[current_y][current_x] = EMPTY;
        manB->map_pawn[eaten_y][eaten_x] = EMPTY;
        switch(coords[6])
        {
            case W:
                manB->map_pawn[new_y][new_x] = WHITE;
                manB->black_pawn_left--;
                break;
            case B:
                manB->map_pawn[new_y][new_x] = BLACK;
                manB->white_pawn_left--;
                break;
        }
        bool = TRUE;
    }
    else if((new_x == 0) && (abs(current_y - new_y) == 2))
    {
        manB->map_pawn[current_y][current_x] = EMPTY;
        manB->map_pawn[eaten_y][eaten_x] = EMPTY;
        switch(coords[6])
        {
            case W:
                manB->map_pawn[new_y][new_x] = WHITE;
                manB->black_pawn_left--;
                break;
            case B:
                manB->map_pawn[new_y][new_x] = BLACK;
                manB->white_pawn_left--;
                break;
        }
        bool = TRUE;
    }
    else if((current_y == 0) && (abs(current_x - new_x) == 2))
    {
        manB->map_pawn[current_y][current_x] = EMPTY;
        manB->map_pawn[eaten_y][eaten_x] = EMPTY;
        switch(coords[6])
        {
            case W:
                manB->map_pawn[new_y][new_x] = WHITE;
                manB->black_pawn_left--;
                break;
            case B:
                manB->map_pawn[new_y][new_x] = BLACK;
                manB->white_pawn_left--;
                break;
        }
        bool = TRUE;
    }
    else if((new_y == 0) && (abs(current_x - new_x) == 2))
    {
        manB->map_pawn[current_y][current_x] = EMPTY;
        manB->map_pawn[eaten_y][eaten_x] = EMPTY;
        switch(coords[6])
        {
            case W:
                manB->map_pawn[new_y][new_x] = WHITE;
                manB->black_pawn_left--;
                break;
            case B:
                manB->map_pawn[new_y][new_x] = BLACK;
                manB->white_pawn_left--;
                break;
        }
        bool = TRUE;
    }
    return bool;
}

/************************************************************************************/
/* function : sendFinalQuit                                                         */
/************************************************************************************/
/* Input : int *, char *msg                                                         */
/* Output :                                                                         */
/************************************************************************************/
/* purpose : this function is call by server and by the son process, the return is  */
/* a status of exit for the father. it send a message of quit to a player due to a  */
/* leave of the other player                                                        */
/************************************************************************************/
void sendFinalQuit(int *sk_accept, char *msg)
{
    int count = 0;
    char tmp[MAX] = {""};
    tmp[0] = QUIT;
    tmp[1] = '\0';
    switch(msg[1])
    {
        case W:
            if((count = send(sk_accept[1], tmp, sizeof(tmp), 0)) == -1)
            {
                perror("serveur : send data failure : ");
                exit(EXIT_FAILURE);
            }
            else
            {
                puts("Send quit to black");
            }
            
            break;
        case B:
            if((count = send(sk_accept[0], tmp, sizeof(tmp), 0)) == -1)
            {
                perror("serveur : send data failure : ");
                exit(EXIT_FAILURE);
            }
            else
            {
                puts("Send quit to white");
            }
            
            break;
    }

    exit(6);
}

/************************************************************************************/
/* function : sendValidateMove                                                      */
/************************************************************************************/
/* Input : int, int *, char *, board *                                              */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : this function is call by server and by the son process, the return of  */
/* checkMove say if it send validate (and to the other client to begin his turn) or */
/* not                                                                              */
/************************************************************************************/
void sendValidateMove(int exit_status, char *coords, board *manB, int *sk_accept)
{
    int count = 0;
    int bool = FALSE;
    char msg[MAX] = {""};
    switch(exit_status)
    {
        case 4: //valide move
            msg[0] = MOVEOK;
            msg[1] = '\0';
            if((count = send(sk_accept[manB->color], msg, sizeof(msg), 0)) == -1) //send to current player the end of his turn
            {
                perror("serveur : send data failure : ");
                exit(EXIT_FAILURE);
            }
            switch(coords[4])
            {
                case W:
                    manB->color = BLACK;
                    break;
                case B:
                    manB->color = WHITE;
                    break;
            }
            msg[0] = YOURTURN;
            msg[1] = coords[0]; // current x
            msg[2] = coords[1]; //current y
            msg[3] = coords[2]; //new x
            msg[4] = coords[3]; //new y
            msg[5] = '\0';
            if((count = send(sk_accept[manB->color], msg, sizeof(msg), 0)) == -1) //sent the new player to beging his turn
            {
                perror("serveur : send data failure : ");
                exit(EXIT_FAILURE);
            }
            bool = TRUE;
            break;
        case 5:
            msg[0] = MOVENOTOK;
            msg[1] = '\0';
            if((count = send(sk_accept[manB->color], msg, sizeof(msg), 0)) == -1) //send to current player the end of his turn
            {
                perror("serveur : send data failure : ");
                exit(EXIT_FAILURE);
            }
            break;
    }

    if(bool)
    {
        exit(manB->color);
    }
    else
    {
        exit(exit_status);
    }
   
}
/************************************************************************************/
/* function : sendEndGame                                                           */
/************************************************************************************/
/* Input : board *, int *                                                           */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : this function is call by server and by the son process.                */
/* look if there is nomore pawn in one color. If yes say it to the winner and the   */
/* looser                                                                           */
/************************************************************************************/
void sendEndGame(board *manB, int *sk_accept)
{
    char msg[MAX] = {""};
    int count = 0;
    if(manB->black_pawn_left == 0) // white win and black loose
    {
        msg[0] = YOUWIN;
        msg[1] = '\0';
        if((count = send(sk_accept[0], msg, sizeof(msg), 0)) == -1)
        {
            perror("serveur : send data failure : ");
            exit(EXIT_FAILURE);
        }

        msg[0] = YOULOOSE;
        msg[1] = '\0';
        if((count = send(sk_accept[1], msg, sizeof(msg), 0)) == -1) //send to current player the end of his turn
        {
            perror("serveur : send data failure : ");
            exit(EXIT_FAILURE);
        }
        exit(7);
    }
    else if(manB->white_pawn_left == 0) // black win and white loose
    {
        msg[0] = YOUWIN;
        msg[1] = '\0';
        if((count = send(sk_accept[1], msg, sizeof(msg), 0)) == -1)
        {
            perror("serveur : send data failure : ");
            exit(EXIT_FAILURE);
        }

        msg[0] = YOULOOSE;
        msg[1] = '\0';
        if((count = send(sk_accept[0], msg, sizeof(msg), 0)) == -1) //send to current player the end of his turn
        {
            perror("serveur : send data failure : ");
            exit(EXIT_FAILURE);
        }
        exit(8);
    }

}

/************************************************************************************/
/* function : sendValidateTake                                                      */
/************************************************************************************/
/* Input : int, char *, board *, int *                                              */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : this function is call by server and by the son process.                */
/* Do the same as sendValidateMove but for a take and with the return of checkTake  */
/************************************************************************************/
void sendValidateTake(int exit_status, char *coords, board *manB, int *sk_accept)
{
    int count = 0;
    int bool = FALSE;
    char msg[MAX] = {""};
    switch(exit_status)
    {
        case 4: //valide take
            msg[0] = TAKEOK;
            msg[1] = '\0';
            if((count = send(sk_accept[manB->color], msg, sizeof(msg), 0)) == -1) //send to current player the end of his turn
            {
                perror("serveur : send data failure : ");
                exit(EXIT_FAILURE);
            }
            switch(coords[6])
            {
                case W:
                    manB->color = BLACK;
                    break;
                case B:
                    manB->color = WHITE;
                    break;
            }
            msg[0] = YOURTURNAT;
            msg[1] = coords[0]; // current x
            msg[2] = coords[1]; //current y
            msg[3] = coords[2]; //new x
            msg[4] = coords[3]; //new y
            msg[5] = coords[4]; //eaten x
            msg[6] = coords[5]; //eaten y
            msg[7] = '\0';

            if((count = send(sk_accept[manB->color], msg, sizeof(msg), 0)) == -1) //sent the new player to beging his turn
            {
                perror("serveur : send data failure : ");
                exit(EXIT_FAILURE);
            }
            bool = TRUE;
            break;
        case 5:
            msg[0] = TAKENOTOK;
            msg[1] = '\0';
            if((count = send(sk_accept[manB->color], msg, sizeof(msg), 0)) == -1) //send to current player the end of his turn
            {
                perror("serveur : send data failure : ");
                exit(EXIT_FAILURE);
            }
            break;
    }

    if(bool)
    {
        exit(manB->color);
    }
    else
    {
        exit(exit_status);
    }
   
}

/************************************************************************************/
/* function : handle_exitStatus                                                     */
/************************************************************************************/
/* Input : int                                                                      */
/* Output : int                                                                     */
/************************************************************************************/
/* purpose : this function is call by server and by the father process to verify the*/
/* status of the end of the son process. return TRUE to continue the mother function*/
/* or false if stop                                                                 */
/************************************************************************************/
int handle_exitStatus(int status)
{
    int bool = FALSE; 
    if(WEXITSTATUS(status) == 0) //the game is ended
    {
        bool = FALSE;
    }
    else if(WEXITSTATUS(status) == 1)
    {
       puts("New account created");
       puts(">>> waiting for others connection");
       bool = TRUE;    
    }
    else if(WEXITSTATUS(status) == 2)
    {
        puts("Player registered");
        puts(">>> waiting for others connection");
        bool = TRUE;
    }
    else if(WEXITSTATUS(status) == 3)
    {
        puts("The game can now begin !");
        bool = TRUE;
    }

    return bool;
}

/************************************************************************************/
/* function : handle_exitTurn                                                       */
/************************************************************************************/
/* Input : int, board *                                                             */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : this function is call by server and by the father process. It is like  */
/* handle_exitStatus but change the index use for the connect socket tab if it is the*/
/* end of this turn and the beginning of new one                                    */
/************************************************************************************/
int handle_exitTurn(int status, board *manB)
{
    int bool;
    if((WEXITSTATUS(status) == BLACK) || (WEXITSTATUS(status) == WHITE))
    {
        puts("Correct move !");
        switch(WEXITSTATUS(status))
        {
            case BLACK:
                puts("Now it's the BLACK pawn to play");
                manB->color = BLACK;
                break;
            case WHITE:
                puts("Now it's the WHITE pawn to play");
                manB->color = WHITE;
                break;
        }
        bool = TRUE;
    }
    else if(WEXITSTATUS(status) == 5)
    {
        puts("Incorrect move !");
        bool = TRUE;
    }
    else if(WEXITSTATUS(status) == 6)
    {
        puts("by !");
        bool = FALSE;
    }
    else if(WEXITSTATUS(status) == 7)
    {
        puts("White pawn have win ! Congrats !");
        bool = FALSE;
    }
    else if(WEXITSTATUS(status) == 8)
    {
        puts("Black pawn have win ! Congrats !");
        bool = FALSE;
    }
    return bool;
}

/************************************************************************************/
/* function : initialise_map                                                        */
/************************************************************************************/
/* Input : board *                                                                  */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : this function is call by server and initialise all the component of the*/
/* board structure                                                                  */
/************************************************************************************/
void initialize_map(board *manBoard)
{
    if(manBoard == NULL) //verifing if the object exist
    {
        fprintf(stderr, "initialize_map() : No board pointer available");
        exit(EXIT_FAILURE);
    }

    //initialise the double input table with size
    manBoard->height = NB_BLOCK_HEIGHT;
    manBoard->width = NB_BLOCK_WIDTH;
    manBoard->color = WHITE;
    manBoard->white_pawn_left = 20;
    manBoard->black_pawn_left = 20;
    manBoard->map_pawn = malloc(manBoard->width * sizeof(int**));
    
    if(manBoard->map_pawn != NULL)
    {
        for(int i = 0; i < manBoard->width; i++)
        {
            manBoard->map_pawn[i] = malloc(manBoard->width * sizeof(int*));
        
            if(manBoard->map_pawn[i] == NULL)
            {
                fprintf(stderr, "initialize_map() : malloc on map_pawn[%d] failure\n", i);
                exit(EXIT_FAILURE);
            }
        }
    }
    else
    {
        fprintf(stderr, "initialize_map() : malloc on map_pawn failure");
        exit(EXIT_FAILURE);
    }

    // add the pawns at the start
    for(int i = 0; i < manBoard->width; i++)
    {
        for(int j = 0; j < manBoard->height; j++)
        {
            if(i < 4) // black pawn
            {
                if(i%2 == 0) // line 0, 2
                {
                    if(j%2 == 1) //colonne 1, 3, 5, 7, 9
                    {
                        manBoard->map_pawn[i][j] = BLACK;
                    }
                    else
                    {
                        manBoard->map_pawn[i][j] = EMPTY;
                    }
                }
                else //line 1, 3
                {
                    if(j%2 == 0) // colonne 0, 2, 4, 6, 8
                    {
                        manBoard->map_pawn[i][j] = BLACK;
                    }
                    else
                    {
                        manBoard->map_pawn[i][j] = EMPTY;
                    }
                }
            }
            else if(i > 5) // white pawn
            {
                if(i%2 == 0) // ligne 6, 8
                {
                    if(j%2 == 1) // colonne 1, 3, 5, 7, 9
                    {
                        manBoard->map_pawn[i][j] = WHITE;
                    }
                    else
                    {
                        manBoard->map_pawn[i][j] = EMPTY;
                    } 
                }
                else // line 7, 9
                {
                    if(j%2 == 0) // colonne 0, 2, 4, 6, 8
                    {
                        manBoard->map_pawn[i][j] = WHITE;
                    }
                    else
                    {
                        manBoard->map_pawn[i][j] = EMPTY;
                    }
                }
            }
            else // line 4 et 5
            {
                manBoard->map_pawn[i][j] = EMPTY;
            }
        }
    }
}

/************************************************************************************/
/* function : destroy_map                                                           */
/************************************************************************************/
/* Input : board *                                                                  */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : this function is call by server. Release the memory                    */
/************************************************************************************/
void destroy_map(board *manBoard)
{
    if(manBoard == NULL)
    {
        fprintf(stderr, "destroyBoard() : No manage_board pointer available");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < manBoard->height; i++)
    {
        free(manBoard->map_pawn[i]);
    }

    free(manBoard->map_pawn);

}