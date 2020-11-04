/*namefile : function.c

purpose : src file for module profil where all signature of function are for this module

created by : Francois Suzeau

date : 28/10/2020

*/

#include "function.h"


/************************************************************************************/
/* function : create_account                                                        */
/************************************************************************************/
/* Input : void                                                                     */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : Create new account                                                     */
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
/* Input : void                                                                     */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : Create new account                                                     */
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
/* Input : void                                                                     */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : Create new account                                                     */
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
/* Input : void                                                                     */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : Create new account                                                     */
/************************************************************************************/
int launch_party(int *sk_accept)
{
    int count = 0;
    char buf[MAX] = {""};
    for(int i = 0; i < 3; i++)
    {
        switch(sk_accept[i])
        {
            case 0:
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
/* Input : void                                                                     */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : Create new account                                                     */
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
                        coords[0] = buf[1];
                        coords[1] = buf[2];
                        coords[2] = buf[3];
                        coords[3] = buf[4];
                        coords[4] = buf[5];

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
                        coords[0] = buf[1];
                        coords[1] = buf[2];
                        coords[2] = buf[3];
                        coords[3] = buf[4];
                        coords[4] = buf[5];
                        coords[5] = buf[6];
                        coords[6] = buf[7];
                        coords[7] = buf[8];

                        if(checkTake(manB, coords))
                        {
                            sendValidateTake(4, coords, manB, sk_accept);
                        }
                        else
                        {
                            sendValidateTake(5, coords, manB, sk_accept);
                        }

                    }
                    else if(strcmp(buf, "quit"))
                    {
                        exit(6);
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
/* Input : void                                                                     */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : Create new account                                                     */
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
/* Input : void                                                                     */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : Create new account                                                     */
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
        if((abs(current_x - new_x) == 2) && (abs(current_y - new_y) == 2)) //player had indeed move on one case to the left or to the right 
        {
            manB->map_pawn[current_y][current_x] = EMPTY;
            manB->map_pawn[eaten_y][eaten_x] = EMPTY;
            switch(coords[6])
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
    else if((current_x == 0) && (abs(current_y - new_y) == 2))
    {
        manB->map_pawn[current_y][current_x] = EMPTY;
        manB->map_pawn[eaten_y][eaten_x] = EMPTY;
        switch(coords[6])
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
    else if((new_x == 0) && (abs(current_y - new_y) == 2))
    {
        manB->map_pawn[current_y][current_x] = EMPTY;
        manB->map_pawn[eaten_y][eaten_x] = EMPTY;
        switch(coords[6])
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
    else if((current_y == 0) && (abs(current_x - new_x) == 2))
    {
        manB->map_pawn[current_y][current_x] = EMPTY;
        manB->map_pawn[eaten_y][eaten_x] = EMPTY;
        switch(coords[6])
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
    else if((new_y == 0) && (abs(current_x - new_x) == 2))
    {
        manB->map_pawn[current_y][current_x] = EMPTY;
        manB->map_pawn[eaten_y][eaten_x] = EMPTY;
        switch(coords[6])
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
/* function : sendValidateMove                                                             */
/************************************************************************************/
/* Input : void                                                                     */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : Create new account                                                     */
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
/* function : sendValidateTake                                                             */
/************************************************************************************/
/* Input : void                                                                     */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : Create new account                                                     */
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
/* Input : void                                                                     */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : Create new account                                                     */
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
/* function : handle_exitTurn                                                     */
/************************************************************************************/
/* Input : void                                                                     */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : Create new account                                                     */
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

    return bool;
}

/************************************************************************************/
/* function : initialise_map                                                        */
/************************************************************************************/
/* Input : void                                                                     */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : Create new account                                                     */
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
/* Input : void                                                                     */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : Create new account                                                     */
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