/*namefile : communication.h

purpose : header file for module draught_game

created by : Francois Suzeau

date : 25/10/2020

*/

#ifndef COMMUNICATION
#define COMMUNICATION
  

    /*********************************************************************************************************************/
    /*                                                      includes                                                     */
    /*********************************************************************************************************************/

        #include <stdio.h>
        #include <stdlib.h>

        //local includes
        #include "../../constant/constant.h"
        #include "../profil/profil.h"
        #include "../sdlboard/sdl.h"


    /*********************************************************************************************************************/
    /*                                              variables needed                                                     */
    /*********************************************************************************************************************/


    /*********************************************************************************************************************/
    /*                                                  prototypes                                                       */
    /*********************************************************************************************************************/

        void connection_ask(manage_player *player, int sk);
        void waiting_beging(int sk, manage_player *player);
        void sendMove(SDL_Rect *current_pos, SDL_Rect *newPos, int *sk);

#endif