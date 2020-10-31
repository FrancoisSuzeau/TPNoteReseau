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
int handle_logIn(char *mrecv, int sk_accept, int nb_connection)
{
    int count = 0;
    if(mrecv[0] == REGISTRED)
    {
        mrecv[0] = CONNECTED;
        mrecv[1] = nb_connection - 1;
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
/* function : handle_connection                                                     */
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

    return bool;
}