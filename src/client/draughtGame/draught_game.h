/*namefile : draught_game.h

purpose : header file for module draught_game

created by : Francois Suzeau

date : 25/10/2020

*/

#ifndef DRAUGHT_GAME
#define DRAUGHT_GAME
  

    /*********************************************************************************************************************/
    /*                                                      includes                                                     */
    /*********************************************************************************************************************/
    
        #include <stdio.h>
        #include <stdlib.h>
        #include <math.h>

        //local includes
        #include "../../constant/constant.h"
        #include "../profil/profil.h"

        //SDL header
        #include <SDL/SDL.h>
        #include <SDL/SDL_image.h>


    /*********************************************************************************************************************/
    /*                                              variables needed                                                     */
    /*********************************************************************************************************************/

        typedef struct manage_board {

            int     **boardSDL; //the board used by sdl where the pawns are
            int     **board_blackCase; // the board of black case use to verify if the selecteur can drop a pawn at the coordonate or no (white case)
            int     width;   //even if it's a square board i use different variable name to find myself in my program (for loop for example) 
            int     height;
            int     selectIs; // to if the selector had select a pawn or not and which one

        } manage_board;

    /*********************************************************************************************************************/
    /*                                                  prototypes                                                       */
    /*********************************************************************************************************************/
        void    InGame(SDL_Surface *screen, int tourjoueur);

        void    initialiseBoard(manage_board *this);

        int     validateSelect(manage_board *manBoard, SDL_Rect *position);

        void    validateTaken(manage_board *manBoard, SDL_Rect *ancientpos, SDL_Rect *eaten, SDL_Rect *newPos, manage_player *player);

        void    destroyBoard(manage_board *manBoardD);

#endif