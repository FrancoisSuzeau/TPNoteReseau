/*namefile : draught_game.c

purpose : src file for module draught_game where all signature of function are for this module

created by : Francois Suzeau

date : 25/10/2020

*/

#include "draught_game.h"


    

/************************************************************************************/
/* function : initialiseBoard                                                       */
/************************************************************************************/
/* Input : manage_board *                                                           */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : this function initialise the map pawn at the beginning                 */
/*                                                                                  */
/************************************************************************************/
void initialiseBoard(manage_board *manBoardI)
{
    if(manBoardI == NULL) //verifing if the object exist
    {
        fprintf(stderr, "initialiseBoard() : No manage_board pointer available");
        exit(EXIT_FAILURE);
    }

    //initialise the double input table with size
    manBoardI->height = NB_BLOCK_HEIGHT;
    manBoardI->width = NB_BLOCK_WIDTH;
    manBoardI->boardSDL = malloc(manBoardI->width * sizeof(int**));
    manBoardI->board_blackCase = malloc(manBoardI->width*sizeof(int**));
    
    if((manBoardI->boardSDL != NULL) && (manBoardI->board_blackCase != NULL))
    {
        for(int i = 0; i < manBoardI->width; i++)
        {
            manBoardI->boardSDL[i] = malloc(manBoardI->width * sizeof(int*));
            manBoardI->board_blackCase[i] = malloc(manBoardI->width*sizeof(int*));
            if((manBoardI->boardSDL[i] == NULL) || (manBoardI->board_blackCase[i] == NULL))
            {
                fprintf(stderr, "initialiseBoard() : malloc on boardSDL[%d] failure\n", i);
                exit(EXIT_FAILURE);
            }
        }
    }
    else
    {
        fprintf(stderr, "initialiseBoard() : malloc on boardSDL failure");
        exit(EXIT_FAILURE);
    }

    // add the pawns at the start
    for(int i = 0; i < manBoardI->width; i++)
    {
        for(int j = 0; j < manBoardI->height; j++)
        {
            if(i < 4) // black pawn
            {
                if(i%2 == 0) // line 0, 2
                {
                    if(j%2 == 1) //colonne 1, 3, 5, 7, 9
                    {
                        manBoardI->boardSDL[i][j] = BLACK;
                    }
                    else
                    {
                        manBoardI->boardSDL[i][j] = EMPTY;
                    }
                }
                else //line 1, 3
                {
                    if(j%2 == 0) // colonne 0, 2, 4, 6, 8
                    {
                        manBoardI->boardSDL[i][j] = BLACK;
                    }
                    else
                    {
                        manBoardI->boardSDL[i][j] = EMPTY;
                    }
                }
            }
            else if(i > 5) // white pawn
            {
                if(i%2 == 0) // ligne 6, 8
                {
                    if(j%2 == 1) // colonne 1, 3, 5, 7, 9
                    {
                        manBoardI->boardSDL[i][j] = WHITE;
                    }
                    else
                    {
                        manBoardI->boardSDL[i][j] = EMPTY;
                    } 
                }
                else // line 7, 9
                {
                    if(j%2 == 0) // colonne 0, 2, 4, 6, 8
                    {
                        manBoardI->boardSDL[i][j] = WHITE;
                    }
                    else
                    {
                        manBoardI->boardSDL[i][j] = EMPTY;
                    }
                }
            }
            else // line 4 et 5
            {
                manBoardI->boardSDL[i][j] = EMPTY;
            }
        }
    }
    manBoardI->selectIs = EMPTY;
    int count = 1;
    for(int i = 0; i < manBoardI->width; i++)
    {
        for(int j = 0; j < manBoardI->height; j++)
        {
            if(i%2 == 0)
            {
                if(j%2 == 1)
                {
                    manBoardI->board_blackCase[i][j] = count;
                    count++;
                }
                else
                {
                    manBoardI->board_blackCase[i][j] = 0;
                }
                
            }
            else
            {
                if(j%2 == 0)
                {
                    manBoardI->board_blackCase[i][j] = count;
                    count++;
                }
                else
                {
                    manBoardI->board_blackCase[i][j] = 0;
                }
                
            }
            
        }
    } 
}

/************************************************************************************/
/* function : validateTaken                                                         */
/************************************************************************************/
/* Input : manage_board *, SDL_Rect *                                               */
/* Output : int                                                                     */
/************************************************************************************/
/* purpose : validate if the shot + 2 case in diagonal is correct or no             */
/*                                                                                  */
/************************************************************************************/
int validateTaken(manage_board *manBoard, SDL_Rect *pos)
{
    int bool = FALSE;
    switch(pos->y) // we verifying first the 2 line at the with x with the different value of x
    {
        case 0: //upper line
            if(manBoard->boardSDL[pos->y][pos->x] == BLACK)
            {
                switch (pos->x)
                {
                    case 1: // case number 1 on board_black case
                        if((manBoard->boardSDL[pos->y + 2][pos->x + 2] == EMPTY) && (manBoard->boardSDL[pos->y + 1][pos->x + 1] == WHITE))
                        {
                            bool = TRUE;
                        }
                        break;
                    case NB_BLOCK_WIDTH - 2: // case number 5 on board_black case
                        if((manBoard->boardSDL[pos->y + 2][pos->x - 2] == EMPTY) && (manBoard->boardSDL[pos->y + 1][pos->x - 1] == WHITE))
                        {
                            bool = TRUE;
                        }
                        break;
                    default: //others on that line
                        if((manBoard->boardSDL[pos->y + 2][pos->x - 2] == EMPTY) && (manBoard->boardSDL[pos->y + 1][pos->x - 1] == WHITE) )
                        {
                            bool = TRUE;
                        }
                        else if((manBoard->boardSDL[pos->y + 2][pos->x + 2] == EMPTY) && (manBoard->boardSDL[pos->y + 1][pos->x + 1] == WHITE))
                        {
                            bool = TRUE;
                        }
                        break;
                }
            }
            break;
        case 1: //line 1
            if(manBoard->boardSDL[pos->y][pos->x] == BLACK)
            {
                switch (pos->x)
                {
                    case 0: // case number 6 on board_black case
                        if((manBoard->boardSDL[pos->y + 2][pos->x + 2] == EMPTY) && (manBoard->boardSDL[pos->y + 1][pos->x + 1] == WHITE))
                        {
                            bool = TRUE;
                        }
                        break;
                    case NB_BLOCK_WIDTH - 2: // case number 10 on board_black case
                        if((manBoard->boardSDL[pos->y + 2][pos->x - 2] == EMPTY) && (manBoard->boardSDL[pos->y + 1][pos->x - 1] == WHITE))
                        {
                            bool = TRUE;
                        }
                        break;
                    default: //others on that line
                        if((manBoard->boardSDL[pos->y + 2][pos->x - 2] == EMPTY) && (manBoard->boardSDL[pos->y + 1][pos->x - 1] == WHITE) )
                        {
                            bool = TRUE;
                        }
                        else if((manBoard->boardSDL[pos->y + 2][pos->x + 2] == EMPTY) && (manBoard->boardSDL[pos->y + 1][pos->x + 1] == WHITE))
                        {
                            bool = TRUE;
                        }
                        break;
                }
            }
            break;
        case NB_BLOCK_HEIGHT - 2: //line 8
            if(manBoard->boardSDL[pos->y][pos->x] == WHITE)
            {
                switch(pos->x) 
                {
                    case 1: // case number 41 on board_black case
                        if((manBoard->boardSDL[pos->y - 2][pos->x + 2] == EMPTY) && (manBoard->boardSDL[pos->y - 1][pos->x + 1] == BLACK))
                        {
                            bool = TRUE;
                        }
                        break;
                    case NB_BLOCK_WIDTH - 1: // case number 45 on board_black case
                        if((manBoard->boardSDL[pos->y - 2][pos->x - 2] == EMPTY) && (manBoard->boardSDL[pos->y - 1][pos->x - 1] == BLACK))
                        {
                            bool = TRUE;
                        }
                        break;
                    default: //others on that line
                        if((manBoard->boardSDL[pos->y - 2][pos->x - 2] == EMPTY) && (manBoard->boardSDL[pos->y - 1][pos->x - 1] == BLACK))
                        {
                            bool = TRUE;
                        }
                        else if((manBoard->boardSDL[pos->y - 2][pos->x + 2] == EMPTY) && (manBoard->boardSDL[pos->y - 1][pos->x + 1] == BLACK))
                        {
                            bool = TRUE;
                        }
                        break;
                }
            }
            break;
        case NB_BLOCK_HEIGHT - 1: //line 9
            if(manBoard->boardSDL[pos->y][pos->x] == WHITE)
            {
                switch(pos->x)
                {
                    case 0: // case number 46 on board_black case
                        if((manBoard->boardSDL[pos->y - 2][pos->x + 2] == EMPTY) && (manBoard->boardSDL[pos->y - 1][pos->x + 1] == BLACK)) // case number 11 on board_black case
                        {
                            bool = TRUE;
                        }
                        break;
                    case NB_BLOCK_WIDTH - 2: // case number 50 on board_black case
                        if((manBoard->boardSDL[pos->y - 2][pos->x - 2] == EMPTY) && (manBoard->boardSDL[pos->y - 1][pos->x - 1] == BLACK))
                        {
                            bool = TRUE;
                        }
                        break;
                    default: //others on that line
                        if((manBoard->boardSDL[pos->y - 2][pos->x - 2] == EMPTY) && (manBoard->boardSDL[pos->y - 1][pos->x - 1] == BLACK))
                        {
                            bool = TRUE;
                        }
                        else if((manBoard->boardSDL[pos->y - 2][pos->x + 2] == EMPTY) && (manBoard->boardSDL[pos->y - 1][pos->x + 1] == BLACK))
                        {
                            bool = TRUE;
                        }
                        break;
                }
            }
            break;
        default:
            switch(pos->x)
            {
                case 0: // case number 16, 26, 36 on board_black case
                    if((manBoard->boardSDL[pos->y + 2][pos->x + 2] == EMPTY) && (manBoard->boardSDL[pos->y][pos->x] == BLACK))
                    {
                        if(manBoard->boardSDL[pos->y + 1][pos->x + 1] == WHITE)
                        {
                            bool = TRUE;
                        }
                    }
                    else if(manBoard->boardSDL[pos->y - 2][pos->x + 2] == EMPTY && manBoard->boardSDL[pos->y][pos->x] == WHITE)
                    {
                        if(manBoard->boardSDL[pos->y - 1][pos->x + 1] == BLACK)
                        {
                            bool = TRUE;
                        }
                    }
                    break;
                case 1: // case number 21, 31 on board_black case
                    if((manBoard->boardSDL[pos->y + 2][pos->x + 2] == EMPTY) && (manBoard->boardSDL[pos->y][pos->x] == BLACK))
                    {
                        if(manBoard->boardSDL[pos->y + 1][pos->x + 1] == WHITE)
                        {
                            bool = TRUE;
                        }
                    }
                    else if(manBoard->boardSDL[pos->y - 2][pos->x + 2] == EMPTY && manBoard->boardSDL[pos->y][pos->x] == WHITE)
                    {
                        if(manBoard->boardSDL[pos->y - 1][pos->x + 1] == BLACK)
                        {
                            bool = TRUE;
                        }
                    }
                    break;
                case NB_BLOCK_WIDTH - 2: // case number 20, 30, 40 on board_black case
                    if((manBoard->boardSDL[pos->y + 2][pos->x - 2] == EMPTY) && (manBoard->boardSDL[pos->y][pos->x] == BLACK))
                    {
                        if(manBoard->boardSDL[pos->y + 1][pos->x - 1] == WHITE)
                        {
                            bool = TRUE;
                        }
                    }
                    else if(manBoard->boardSDL[pos->y - 2][pos->x - 2] == EMPTY && manBoard->boardSDL[pos->y][pos->x] == WHITE)
                    {
                        if(manBoard->boardSDL[pos->y - 1][pos->x - 1] == BLACK)
                        {
                            bool = TRUE;
                        }
                    }
                    break;
                case NB_BLOCK_WIDTH - 1: // case number 25, 35 on board_black case
                    if((manBoard->boardSDL[pos->y + 2][pos->x - 2] == EMPTY) && (manBoard->boardSDL[pos->y][pos->x] == BLACK))
                    {
                        if(manBoard->boardSDL[pos->y + 1][pos->x - 1] == WHITE)
                        {
                            bool = TRUE;
                        }
                    }
                    else if(manBoard->boardSDL[pos->y - 2][pos->x - 2] == EMPTY && manBoard->boardSDL[pos->y][pos->x] == WHITE)
                    {
                        if(manBoard->boardSDL[pos->y - 1][pos->x - 1] == BLACK)
                        {
                            bool = TRUE;
                        }
                    }
                    break;
                default: //we are completely turn arround the board now it's only the current pawn and the pawn in front who make the difference
                    if((manBoard->boardSDL[pos->y + 2][pos->x - 2] == EMPTY) && (manBoard->boardSDL[pos->y][pos->x] == BLACK)) 
                    {
                        if((manBoard->boardSDL[pos->y + 1][pos->x - 1] == WHITE) || (manBoard->boardSDL[pos->y + 1][pos->x + 1] == WHITE))
                        {
                            bool = TRUE;
                        }
                    }
                    else if((manBoard->boardSDL[pos->y + 2][pos->x + 2] == EMPTY) && (manBoard->boardSDL[pos->y][pos->x] == BLACK))
                    {
                        if((manBoard->boardSDL[pos->y + 1][pos->x - 1] == WHITE) || (manBoard->boardSDL[pos->y + 1][pos->x + 1] == WHITE))
                        {
                            bool = TRUE;
                        }
                    }
                    else if((manBoard->boardSDL[pos->y - 2][pos->x - 2] == EMPTY) && (manBoard->boardSDL[pos->y][pos->x] == WHITE))
                    {
                        if((manBoard->boardSDL[pos->y - 1][pos->x - 1] == BLACK) || (manBoard->boardSDL[pos->y - 1][pos->x + 1] == BLACK))
                        {
                            bool = TRUE;
                        }
                    }
                    else if((manBoard->boardSDL[pos->y - 2][pos->x + 2] == EMPTY) && (manBoard->boardSDL[pos->y][pos->x] == WHITE))
                    {
                        if((manBoard->boardSDL[pos->y - 1][pos->x - 1] == BLACK) || (manBoard->boardSDL[pos->y - 1][pos->x + 1] == BLACK))
                        {
                            bool = TRUE;
                        }
                    }
                    break;
            }
            break;
    }
    
    return bool;
}

/************************************************************************************/
/* function : validateSelect                                                        */
/************************************************************************************/
/* Input : manage_board *, SDL_Rect *                                               */
/* Output : int                                                                     */
/************************************************************************************/
/* purpose : validate if the shot is correct or no                                  */
/*                                                                                  */
/************************************************************************************/
int validateSelect(manage_board *manBoard, SDL_Rect *position)
{
    int bool = FALSE;
    //beging with the edge
    if(position->y - 1 == - 1) // upper line
    {
        switch(position->x + 1)
        {
            case NB_BLOCK_WIDTH:
                if(manBoard->boardSDL[position->y + 1][position->x - 1] == EMPTY)
                {
                    bool = TRUE;
                }
                break;
            default:
                if((manBoard->boardSDL[position->y + 1][position->x - 1] == EMPTY) || (manBoard->boardSDL[position->y + 1][position->x + 1] == EMPTY))
                {
                    bool = TRUE;
                }
                break;
        }
    }
    else if(position->y + 1 == NB_BLOCK_HEIGHT)
    {
        switch(position->x - 1)
        {
            case -1:
                if(manBoard->boardSDL[position->y - 1][position->x + 1] == EMPTY)
                {
                    bool = TRUE;
                }
                break;
            default:
                if((manBoard->boardSDL[position->y - 1][position->x + 1] == EMPTY) || (manBoard->boardSDL[position->y - 1][position->x + 1] == EMPTY))
                {
                    bool = TRUE;
                }
                break;
        }

    }
    else if(position->x - 1 == - 1)
    {
        if((manBoard->boardSDL[position->y - 1][position->x + 1] == EMPTY) || (manBoard->boardSDL[position->y + 1][position->x + 1] == EMPTY))
        {
            bool = TRUE;
        }

    }
    else if(position->x + 1 == NB_BLOCK_WIDTH)
    {
        if((manBoard->boardSDL[position->y + 1][position->x - 1] == EMPTY) || (manBoard->boardSDL[position->y - 1][position->x - 1] == EMPTY))
        {
            bool = TRUE;
        }

    }
    else //all others who aren't on the edge
    {
        if((manBoard->boardSDL[position->y + 1][position->x-1] == EMPTY) || (manBoard->boardSDL[position->y + 1][position->x+1] == EMPTY)
        || (manBoard->boardSDL[position->y - 1][position->x - 1] == EMPTY) || (manBoard->boardSDL[position-> y - 1][position->x + 1] == EMPTY))
        {
            bool = TRUE;
        }   
    }
    
    return bool;
}



/************************************************************************************/
/* function : destroyBoard                                                          */
/************************************************************************************/
/* Input : manage_board *                                                           */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : this function destroy the boardSDL and all of the variable             */
/* we need in it                                                                    */
/************************************************************************************/
void destroyBoard(manage_board *manBoardD)
{
    if(manBoardD == NULL)
    {
        fprintf(stderr, "destroyBoard() : No manage_board pointer available");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < manBoardD->height; i++)
    {
        free(manBoardD->boardSDL[i]);
        free(manBoardD->board_blackCase[i]);
    }

    free(manBoardD->boardSDL);
    free(manBoardD->board_blackCase);
}