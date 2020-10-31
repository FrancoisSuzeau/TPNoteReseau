/*namefile : constant.h

purpose : header file for module draught_game

created by : Francois Suzeau

date : 25/10/2020

*/

#ifndef CONSTANT
#define CONSTANT


    #define TRUE        1
    #define FALSE       0

    #define MAX 100

    #define BLOCK_SIZE      60
    #define NB_BLOCK_WIDTH  10
    #define NB_BLOCK_HEIGHT 10
    #define WINDOW_WIDTH    BLOCK_SIZE * NB_BLOCK_WIDTH
    #define WINDOW_HEIGHT   BLOCK_SIZE * NB_BLOCK_HEIGHT

    /*********************************************************************************************************************/
    /*                                              protocole binaire                                                    */
    /*********************************************************************************************************************/
    #define CONNECTED   0x01
    #define REGISTRED   0x02
    #define MOVE        0x03
    #define DECONNECTED 0X04

    #include <stdio.h>
    #include <stdlib.h>

    /*********************************************************************************************************************/
    /*                                              variables needed                                                     */
    /*********************************************************************************************************************/

        enum {WHITE, BLACK, EMPTY};
        enum {UP, DOWN, LEFT, RIGHT};

        enum{PLAYER1, PLAYER2, VIEWER};

#endif