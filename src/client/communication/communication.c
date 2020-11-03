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
void sendMove(SDL_Rect *current_pos, SDL_Rect *newPos, int *sk)
{
    char msg[MAX] = {""};
    int count = 0;

    msg[0] = MOVE;
    msg[1] = (char)current_pos->x;
    msg[2] = (char)current_pos->y;
    msg[3] = (char)newPos->x;
    msg[3] = (char)newPos->y;

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