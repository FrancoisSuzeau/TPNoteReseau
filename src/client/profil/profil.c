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
int readDataBase(manage_player *player)
{
    if(player == NULL)
    {
        puts("createFile() : no parameter available");
        exit(EXIT_FAILURE);
    }

    char nameFile[3][MAX] = {""};
    strcat(nameFile[0], "../../files/infojoueurs/player1.txt");
    strcat(nameFile[1], "../../files/infojoueurs/player2.txt");
    strcat(nameFile[2], "../../files/infojoueurs/viewer.txt");
    int count;
    int fd_player;
    char buf[MAX] = "";
    int counter = 0;

    while (TRUE)
    {
        if((fd_player = open(nameFile[counter], O_CREAT | O_RDWR, 0777)) == -1)
        {
            printf("registry() :  cannot open %s \n: ", nameFile[counter]);
            perror("here is why : ");
            exit(EXIT_FAILURE);
        }

        if((count = read(fd_player, buf, strlen(player->name))) != -1)
        {
            if(count == 0)
            {
                player->isRegistred = FALSE;
            }
            else if(count > 0)
            {
                if(strcmp(buf, player->name) == 0)
                {
                    player->isRegistred = TRUE;
                    break;
                }
                else
                {
                    player->isRegistred = FALSE;
                }
                
            }
        }

        close(fd_player);
        if(counter == 2)
        {
            break;
        }
        else
        {
            counter++;
        }
    }

    return player->isRegistred;
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

