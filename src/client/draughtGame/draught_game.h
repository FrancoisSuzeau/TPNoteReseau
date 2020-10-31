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

        //SDL header
        #include <SDL/SDL.h>
        #include <SDL/SDL_image.h>


    /*********************************************************************************************************************/
    /*                                              variables needed                                                     */
    /*********************************************************************************************************************/

        typedef struct manage_board {

            int     **boardSDL;
            int     **board_blackCase;
            int     width;   //even if it's a square board i use different variable name to find myself in my program (in for loop for example) 
            int     height;
            int     selectIs;

        } manage_board;

    /*********************************************************************************************************************/
    /*                                                  prototypes                                                       */
    /*********************************************************************************************************************/
        void InGame(SDL_Surface *screen, int tourjoueur);
        void initialiseBoard(manage_board *this);
        int validateSelect(manage_board *manBoard, SDL_Rect *position);
        int validateTaken(manage_board *manBoard, SDL_Rect *pos);
        void destroyBoard(manage_board *manBoardD);

#endif