/*

namefile : function.h

purpose : headerfile for module function

created by : Francois Suzeau

date : 28/10/2020

*/

#ifndef FUNCTION
#define FUNCTION

    

    /*********************************************************************************************************************/
    /*                                                      includes                                                     */
    /*********************************************************************************************************************/
    
        #include <stdlib.h>
        #include <stdio.h>
        #include <string.h>
        #include <unistd.h>
        #include <sys/types.h>
        #include <sys/wait.h>
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

        

    /*********************************************************************************************************************/
    /*                                                  prototypes                                                       */
    /*********************************************************************************************************************/

        void create_account(char *name, int num_file);
        int handle_logIn(char *mrecv, int sk_accept, int nb_connection);
        int handle_exitStatus(int status);

#endif