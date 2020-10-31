/*namefile : communication.c

purpose : src file for module draught_game where all signature of function are for this module

created by : Francois Suzeau

date : 25/10/2020

*/

#include "communication.h"

/************************************************************************************/
/* function : initialiseBoard                                                       */
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
                perror("Serveur : receive data failure : \n");
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