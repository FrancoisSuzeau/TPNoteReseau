/*namefile : profil.h

purpose : headerfile for module draught_game

created by : Francois Suzeau

date : 25/10/2020

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
        #include <sys/stat.h>
        #include <fcntl.h>
        
        //local includes
        #include "../constant/constant.h"

    /*********************************************************************************************************************/
    /*                                              variables needed                                                     */
    /*********************************************************************************************************************/
        typedef struct manage_player {
            char    name[MAX];
            int     isRegistred;
            int     which_one;
            int     isConnected;
        } manage_player;

        enum{PLAYER1, PLAYER2, VIEWER};
        

    /*********************************************************************************************************************/
    /*                                                  prototypes                                                       */
    /*********************************************************************************************************************/

        void readDataBase(manage_player *player);
        void logIn(manage_player *player);


#endif