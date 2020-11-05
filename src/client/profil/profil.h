/*namefile : profil.h

purpose : headerfile for module profil

created by : Francois Suzeau

date : 28/10/2020

*/

#ifndef PROFIL
#define PROFIL

    

    /*********************************************************************************************************************/
    /*                                                      includes                                                     */
    /*********************************************************************************************************************/
    
        #include <stdlib.h>
        #include <stdio.h>
        #include <string.h>
        #include <unistd.h>
        #include <sys/types.h>
        #include <sys/socket.h>
        #include <netinet/in.h>
        #include <netinet/ip.h>
        #include <arpa/inet.h>
        #include <netdb.h>
        #include <errno.h>
        #include <fcntl.h>
        
        //local includes
        #include "../../constant/constant.h"

    /*********************************************************************************************************************/
    /*                                              variables needed                                                     */
    /*********************************************************************************************************************/
        typedef struct manage_player {
            char    name[MAX]; //the name of the player
            int     isRegistred; // confirm by function readDatabase
            int     which_one; // use to tell if it's the turn to play or to wait
            int     isConnected; //used after confirmation by serveur
            char    color; // the color play by the player
        } manage_player;

    /*********************************************************************************************************************/
    /*                                                  prototypes                                                       */
    /*********************************************************************************************************************/

        int     readDataBase(manage_player *player);

        void    logIn(manage_player *player);


#endif