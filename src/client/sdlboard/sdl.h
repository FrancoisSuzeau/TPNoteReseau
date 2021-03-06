/*namefile : sdl.h

purpose : header file for module draught_game

created by : Francois Suzeau

date : 25/10/2020

*/

#ifndef SDLBOARD
#define SDLBOARD
  

    /*********************************************************************************************************************/
    /*                                                      includes                                                     */
    /*********************************************************************************************************************/

        #include <stdio.h>
        #include <stdlib.h>

        //local includes
        #include "../../constant/constant.h"
        #include "../draughtGame/draught_game.h"
        #include "../profil/profil.h"
        #include "../communication/communication.h"

        //sdl includes
        #include <SDL/SDL.h>
        #include <SDL/SDL_image.h>

    /*********************************************************************************************************************/
    /*                                              variables needed                                                     */
    /*********************************************************************************************************************/

       


    /*********************************************************************************************************************/
    /*                                                  prototypes                                                       */
    /*********************************************************************************************************************/

        SDL_Surface     *sdlInit(manage_player *player);

        void            initTexture();

        void            displayBoard(SDL_Surface *screen);

        void            updateMapPawn(SDL_Surface *screen, manage_board *man_mapPawn);

        SDL_Surface     *selectPawn(manage_board *manBord, SDL_Rect *posi, SDL_Rect *ancientPos, SDL_Rect *newPos, manage_player *player, SDL_Rect *eaten);

        void            moveSelector(SDL_Rect *pos, int direction);

        int             handle_event(manage_player *player, SDL_Rect *position_selector, manage_board *my_manBoard, SDL_Surface **select_pawn, SDL_Rect *ancientPos, SDL_Rect *newPos, SDL_Rect *eaten);

        void            freeTexture();
        
        SDL_Surface     *getSelector();

#endif