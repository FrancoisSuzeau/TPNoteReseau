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
        //printf("CONNECTED : %d\n", mrecv[0]);
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
int handle_turn()
{
    
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
    char tmp[2] = {""};
    int current_x = 0;
    int current_y = 0;
    int new_x = 0;
    int new_y = 0;

    sprintf(tmp,"%c", coords[0]);
    current_x = atoi(tmp);
    tmp[0] = '\0';

    sprintf(tmp,"%c", coords[1]);
    current_y = atoi(tmp);
    tmp[0] = '\0';

    sprintf(tmp,"%c", coords[3]);
    new_x = atoi(tmp);
    tmp[0] = '\0';

    sprintf(tmp,"%c", coords[4]);
    new_y = atoi(tmp);
    tmp[0] = '\0';

    if((abs(current_x - new_x) == 1) && (abs(current_y - new_y) == 1))
    {
        //if(/**/ == PLAYER1)
        /*{
            bool = TRUE;
            manB->map_pawn[current_y][current_x] = EMPTY;
            manB->map_pawn[new_y][new_x] = WHITE;
        }*/
        //else if(/**/ == PLAYER2)
        /*{
            bool = TRUE;
            manB->map_pawn[current_y][current_x] = EMPTY;
            manB->map_pawn[new_y][new_x] = BLACK;
        }*/
         
    }
    return bool;
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
    int bool; 
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