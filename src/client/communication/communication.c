/*namefile : communication.c

purpose : src file for module draught_game where all signature of function are for this module

created by : Francois Suzeau

date : 25/10/2020

*/

#include "communication.h"

/************************************************************************************/
/* function :                                                                       */
/************************************************************************************/
/* Input : manage_board *                                                           */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : this function initialise the map pawn at the beginning                 */
/*                                                                                  */
/************************************************************************************/
void connection_ask(manage_player *player, int sk)
{
    char buf[MAX] = {""};
    int count = 0;
    while(strcmp(buf, "quit"))
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
        buf[0] = '\0';
        strcat(buf, "quit");
    }

}

/************************************************************************************/
/* function :                                                        */
/************************************************************************************/
/* Input : manage_board *                                                           */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : this function initialise the map pawn at the beginning                 */
/*                                                                                  */
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
/* function :                                                        */
/************************************************************************************/
/* Input : manage_board *                                                           */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : this function initialise the map pawn at the beginning                 */
/*                                                                                  */
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
/* function :                                                        */
/************************************************************************************/
/* Input : manage_board *                                                           */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : this function initialise the map pawn at the beginning                 */
/*                                                                                  */
/************************************************************************************/
void waitValidate(int sk, manage_player *player)
{
    char buf[MAX] = {""};
    int count;
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
        }
    }
}

/************************************************************************************/
/* function :                                                        */
/************************************************************************************/
/* Input : manage_board *                                                           */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : this function initialise the map pawn at the beginning                 */
/*                                                                                  */
/************************************************************************************/
void confirmTurn(int sk, manage_player *player, manage_board *manB)
{
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
                        printf(">>>> %s it's now your turn !", player->name);
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
        }
    }
}