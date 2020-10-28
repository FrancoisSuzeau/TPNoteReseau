/*namefile : profil.c

purpose : src file for module profil where all signature of function are for this module

created by : Francois Suzeau

date : 28/10/2020

*/

#include "profil.h"


/************************************************************************************/
/* function : createFile                                                           */
/************************************************************************************/
/* Input : void                                                                     */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : create the file for each player capabilities                           */
/************************************************************************************/
void readDataBase(manage_player *player)
{
    if(player == NULL)
    {
        puts("createFile() : no parameter available");
        exit(EXIT_FAILURE);
    }

    int count;
    int fd_player;
    if(player->which_one == PLAYER1)
    {
        if((fd_player = open("../../files/infojoueurs/player1.txt", O_CREAT | O_RDWR, 0777)) == -1)
        {
            perror("registry() :  cannot open player1.txt : ");
            exit(EXIT_FAILURE);
        }

        if((count = read(fd_player, player->name, MAX)) != -1)
        {
            if(count == 0)
            {
                player->isRegistred = FALSE;
            }
            else if(count > 0)
            {
                player->isRegistred = TRUE;
            }
        }
    }
    else if(player->which_one == PLAYER2)
    {
        if((fd_player = open("../../files/infojoueurs/player2.txt", O_CREAT | O_RDWR, 0777)) == -1)
        {
            perror("registry() :  cannot open player2.txt : ");
            exit(EXIT_FAILURE);
        }
        if((count = read(fd_player, player->name, MAX)) != -1)
        {
            if(count == 0)
            {
                player->isRegistred = FALSE;
            }
            else if(count > 0)
            {
                player->isRegistred = TRUE;
            }
            
        }
    }
    else if(player->which_one == VIEWER)
    {
        if((fd_player = open("../../files/infojoueurs/viewer.txt", O_CREAT | O_RDWR, 0777)) == -1)
        {
            perror("registry() :  cannot open viewer2.txt : ");
            exit(EXIT_FAILURE);
        }
        if((count = read(fd_player, player->name, MAX)) != -1)
        {
            if(count == 0)
            {
                player->isRegistred = FALSE;
            }
            else if(count > 0)
            {
                player->isRegistred = TRUE;
            }
        }
    }
    
    close(fd_player);
}

/************************************************************************************/
/* function : logIn                                                                 */
/************************************************************************************/
/* Input : void                                                                     */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : create the file for each player capabilities                           */
/************************************************************************************/
void logIn(manage_player *player)
{
    if(player == NULL)
    {
        puts("logIn() : no parameter available");
        exit(EXIT_FAILURE);
    }

    puts(" >>> Welcome : please select your login : ");
    while(TRUE)
    {
        scanf("%s", player->name);
        if(strcmp(player->name, "") && strlen(player->name) > 5)
        {
            break;
        }
        if(strlen(player->name) < 5)
        {
            puts(">>>> please enter a valide name");
        }
    }
}