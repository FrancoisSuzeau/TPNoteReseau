/*namefile : communication.c

purpose : src file for module communication used by client where all signature of function are for this module

created by : Francois Suzeau

date : 25/10/2020

*/

#include "communication.h"

/************************************************************************************/
/* function : connection_ask                                                        */
/************************************************************************************/
/* Input : manage_board *, int                                                      */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : this function is call by client. The user log by writting his name and */
/* the function verify if the user is know or not and send his result to serveur    */
/************************************************************************************/
void connection_ask(manage_player *player, int sk)
{
    char buf[MAX] = {""};
    int count = 0;
    while(TRUE)
    {
        logIn(player);
        if(readDataBase(player))
        {
            printf(">>> Welcome back %s !\n", player->name);
            buf[0] = REGISTRED;
            buf[1] = '\0';
        }
        else
        {
            puts(">>>> Waiting from serveur to create account");
            strcat(buf, player->name);
        }
        if((count = send(sk, buf, sizeof(buf), 0)) == -1)
        {
            perror("Client : send data failure : ");
            exit(EXIT_FAILURE);
        }
        else
        {
            buf[0] = '\0';
            if((count = recv(sk, buf, sizeof(buf), 0)) == -1)
            {
                perror("CLient : receive data failure : \n");
            }
            else
            {
                if(buf[0] == CONNECTED)
                {
                    puts("You are now connected");
                    player->isConnected = TRUE;
                    player->which_one = buf[1];
                    switch(player->which_one)
                    {
                        case PLAYER1:
                            player->color = W;
                            break;
                        case PLAYER2:
                            player->color = B;
                            break;
                    }
                }
            }
        }
        break;
    }
}

/************************************************************************************/
/* function : waiting_beging                                                        */
/************************************************************************************/
/* Input : int, manage_board *                                                      */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : this function is call by client. At this point the client wait the     */
/* serveur to tell him to lunch the game while there is no player 2                 */
/************************************************************************************/
void waiting_beging(int sk, manage_player *player)
{
    int count = 0;
    char buf[MAX] = {""};

    if(player->isConnected == TRUE)
    {
        while(TRUE)
        {
            buf[0] = '\0';
            if((count = recv(sk, buf, sizeof(buf), 0)) == -1)
            {
                perror("CLient : receive data failure : \n");
            }
            else
            {
                if(buf[0] == CANPLAY)
                {
                    puts(">>>> An other player has arrived, you can now play. ENJOY !");
                    break;
                }
                else
                {
                    printf(">>>> %s please wait an other player .... \n", player->name);
                }
                
            }
        }
    }
}

/************************************************************************************/
/* function : sendQuit                                                              */
/************************************************************************************/
/* Input : int *, manage_player *                                                   */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : this function his call by client. By reading the value the function can*/
/* know if the move is a simple move or a take. He send the coordonnate of the      */
/* ancient and the new position (and the position of the one who was taken)         */
/* plus the color of the player sendind this message to serveur                     */
/************************************************************************************/
void sendQuit(int *sk, manage_player *player)
{
    int count = 0;
    char msg[MAX] = {""};

    msg[0] = QUIT;
    msg[1] = player->color;
    msg[2] = '\0';

    if((count = send(*sk, msg, sizeof(msg), 0)) == -1)
    {
        perror("Client : send data failure : ");
        exit(EXIT_FAILURE);
    }
}
/************************************************************************************/
/* function : sendMove                                                              */
/************************************************************************************/
/* Input : SDL_Rect * x3, manage_player *                                           */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : this function his call by client. By reading the value the function can*/
/* know if the move is a simple move or a take. He send the coordonnate of the      */
/* ancient and the new position (and the position of the one who was taken)         */
/* plus the color of the player sendind this message to serveur                     */
/************************************************************************************/
void sendMove(SDL_Rect *current_pos, SDL_Rect *newPos, int *sk, manage_player *player, SDL_Rect *eaten)
{
    char msg[MAX] = {""};
    int count = 0;
    if((eaten->x == 0) && (eaten->y == 0))
    {
        msg[0] = MOVE;
        msg[1] = (char)current_pos->x;
        msg[2] = (char)current_pos->y;
        msg[3] = (char)newPos->x;
        msg[4] = (char)newPos->y;
        msg[5] = player->color;
        msg[6] = '\0';
    }
    else
    {
        msg[0] = TAKE;
        msg[1] = (char)current_pos->x;
        msg[2] = (char)current_pos->y;
        msg[3] = (char)newPos->x;
        msg[4] = (char)newPos->y;
        msg[5] = (char)eaten->x;
        msg[6] = (char)eaten->y;
        msg[7] = player->color;
        msg[8] = '\0';
    }

    if((count = send(*sk, msg, sizeof(msg), 0)) == -1)
    {
        perror("Client : send data failure : ");
        exit(EXIT_FAILURE);
    }
    else
    {
        puts("Client : move send to serveur");
    }
    
}

/************************************************************************************/
/* function : waitValidate                                                          */
/************************************************************************************/
/* Input : int, manage_board *                                                      */
/* Output : int                                                                     */
/************************************************************************************/
/* purpose : this function is call by client. At this point the client wait a       */
/* confirmation of the serveur of his move. If the confirmation is good the client  */
/* has finish is turn                                                               */
/************************************************************************************/
int waitValidate(int sk, manage_player *player)
{
    char buf[MAX] = {""};
    int count;
    int bool = TRUE;
    while(TRUE)
    {
        buf[0] = '\0';
        if((count = recv(sk, buf, sizeof(buf), 0)) == -1)
        {
            perror("CLient : receive data failure : \n");
        }
        else
        {
            if(buf[0] == MOVEOK)
            {
                puts(">>>> Your move is correct");
                player->which_one = PLAYER2; //now it's the others player to make a move
                break;
            }
            else if(buf[0] == MOVENOTOK)
            {
                printf(">>>> Your move is not correct please retry !");
                break;
            }
            else if(buf[0] == TAKEOK)
            {
                puts(">>>> Your take is correct");
                player->which_one = PLAYER2; //now it's the others player to make a move
                break;
            } 
            else if(buf[0] == TAKENOTOK)
            {
                printf(">>>> Your take is not correct please retry !");
                break;
            }
            else if(buf[0] == YOUWIN)
            {
                puts("Thanks for your support !");
                bool = FALSE;
                break;
            }
        }
    }
    return bool;
}

/************************************************************************************/
/* function : confirmTurn                                                           */
/************************************************************************************/
/* Input : int, manage_board *, manage_board *                                      */
/* Output : int                                                                     */
/************************************************************************************/
/* purpose : this function is call by client and by the client who not play         */
/* the serveur send to this client the order of begin his turn plus the new         */
/* coordonnate resulting by the move of the precedent player                        */
/************************************************************************************/
int confirmTurn(int sk, manage_player *player, manage_board *manB)
{
    int bool = TRUE;
    char buf[MAX] = {""};
    int count;
    int ancient_x;
    int ancient_y;
    int new_x;
    int new_y;
    int eaten_x;
    int eaten_y;

    while(TRUE)
    {
        buf[0] = '\0';
        if((count = recv(sk, buf, sizeof(buf), 0)) == -1)
        {
            perror("CLient : receive data failure : \n");
        }
        else
        {
            if(buf[0] == YOURTURN)
            {
                ancient_x = buf[1];
                ancient_y = buf[2];
                new_x = buf[3];
                new_y = buf[4];
                
                manB->boardSDL[ancient_y][ancient_x] = EMPTY;
                switch(player->color)
                {
                    case W:
                        manB->boardSDL[new_y][new_x] = BLACK;
                        break;
                    case B:
                        manB->boardSDL[new_y][new_x] = WHITE;
                        break;
                }
                switch(player->which_one)
                {
                    case PLAYER2:
                        player->which_one = PLAYER1;
                        printf(">>>> %s it's now your turn !\n", player->name);
                        break;
                    case VIEWER:
                        puts("Incredible move !");
                        break;
                }
                break;
            }
            if(buf[0] == YOURTURNAT)
            {
                ancient_x = buf[1];
                ancient_y = buf[2];
                new_x = buf[3];
                new_y = buf[4];
                eaten_x = buf[5];
                eaten_y = buf[6];

                manB->boardSDL[ancient_y][ancient_x] = EMPTY;
                manB->boardSDL[eaten_y][eaten_x] = EMPTY;
                switch(player->color)
                {
                    case W:
                        manB->boardSDL[new_y][new_x] = BLACK;
                        break;
                    case B:
                        manB->boardSDL[new_y][new_x] = WHITE;
                        break;
                }
                switch(player->which_one)
                {
                    case PLAYER2:
                        player->which_one = PLAYER1;
                        printf(">>>> %s it's now your turn !\nYou loose one pawn !\n", player->name);
                        break;
                    case VIEWER:
                        puts("Incredible move !");
                        break;
                }
                break;
            }
            else if(buf[0] == YOULOOSE)
            {
                puts("Congrats to the winner !");
                bool = FALSE;
                break;
            }
            else if(buf[0] == QUIT)
            {
                puts("The other player leave the game !");
                bool = FALSE;
                break;
            }
        }
    }

    return bool;
}