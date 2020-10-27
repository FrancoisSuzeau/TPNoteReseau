/*namefile : draught_game.c

purpose : src file for module draught_game where all signature of function are for this module

created by : Francois Suzeau

date : 25/10/2020

*/

#include "draught_game.h"

/************************************************************************************/
/* function : hello                                                                 */
/************************************************************************************/
/* Input : void                                                                     */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : say hi from here to test the compilation process                       */
/************************************************************************************/
void Hello()
{
    puts("Hello There from draught_game module");
}

/************************************************************************************/
/* function : InGame                                                                */
/************************************************************************************/
/* Input : SDL_Surface *                                                            */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : this function control the process of a party                           */
/************************************************************************************/
void InGame(SDL_Surface *screen)
{
    if(screen == NULL)
    {
        fprintf(stderr, "In InGame() : no parameter available");
        exit(EXIT_FAILURE);
    }

    SDL_Surface *img_board = NULL;
    SDL_Surface *white_pawn = NULL;
    SDL_Surface *black_pawn = NULL;
    SDL_Surface *select_pawn = NULL;
    if((img_board = IMG_Load("../files/textures/board.png")) == NULL)
    {
        fprintf(stderr, "Unable to load board.png : %s\n", IMG_GetError());
        exit(EXIT_FAILURE);
    }
    if((white_pawn = IMG_Load("../files/textures/white_pawn.png")) == NULL)
    {
        fprintf(stderr, "Unable to load white_pawn.png : %s\n", IMG_GetError());
        exit(EXIT_FAILURE);
    }
    if((black_pawn = IMG_Load("../files/textures/black_pawn.png")) == NULL)
    {
        fprintf(stderr, "Unable to load black_pawn.png : %s\n", IMG_GetError());
        exit(EXIT_FAILURE);
    }
    if((select_pawn = SDL_CreateRGBSurface(SDL_HWSURFACE, 60, 60, 32, 0, 0, 255, 0)) == NULL)
    {
        fprintf(stderr, "Unable to create RGB surface select_pawn : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    manage_board *my_manBoard = malloc(sizeof(manage_board*));
    initialiseBoard(my_manBoard);

    SDL_Rect position_selector;
    SDL_Rect localisation;
    position_selector.x = 0;
    position_selector.y = 0;

    SDL_Rect save_pos;

    displayBoard(screen, img_board);
    updateMapPawn(screen, black_pawn, white_pawn, my_manBoard);
    SDL_Flip(screen);

    int             isSelect = FALSE;

    int             continuer = TRUE;
    SDL_Event       event;
    while(continuer)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                continuer = FALSE;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        continuer = FALSE;
                        break;
                    case SDLK_UP:
                        displaySelector(&position_selector, UP);
                        break;
                    case SDLK_DOWN:
                        displaySelector(&position_selector, DOWN);
                        break;
                    case SDLK_LEFT:
                        displaySelector(&position_selector, LEFT);
                        break;
                    case SDLK_RIGHT:
                        displaySelector(&position_selector, RIGHT);
                        break;
                    case SDLK_SPACE:
                        if(!isSelect)
                        {
                            save_pos = position_selector;
                            SDL_FreeSurface(select_pawn);
                            select_pawn = selectPawn(my_manBoard, &position_selector, isSelect);
                            isSelect = TRUE;
                        }
                        else
                        {
                            SDL_FreeSurface(select_pawn);
                            select_pawn = selectPawn(my_manBoard, &position_selector, isSelect);
                            position_selector = save_pos;
                            isSelect = FALSE;
                        }
                        break;
                    default:
                        break;
                }
        }

        displayBoard(screen, img_board);
        localisation.x = position_selector.x * BLOCK_SIZE;
        localisation.y = position_selector.y * BLOCK_SIZE;
        SDL_BlitSurface(select_pawn, NULL, screen, &localisation);
        updateMapPawn(screen, black_pawn, white_pawn, my_manBoard);
        SDL_Flip(screen);
    }

    SDL_FreeSurface(white_pawn);
    SDL_FreeSurface(black_pawn);
    SDL_FreeSurface(select_pawn);
    SDL_FreeSurface(img_board);

    destroyBoard(my_manBoard);
}

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
    manBoardI->boardSDL = malloc(manBoardI->height * sizeof(int**));
    manBoardI->board_blackCase = malloc(manBoardI->height*sizeof(int**));
    
    if((manBoardI->boardSDL != NULL) && (manBoardI->board_blackCase != NULL))
    {
        for(int i = 0; i < manBoardI->height; i++)
        {
            manBoardI->boardSDL[i] = malloc(manBoardI->width * sizeof(int*));
            manBoardI->board_blackCase[i] = malloc(manBoardI->height*sizeof(int*));
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
/* function : selectPawn                                                            */
/************************************************************************************/
/* Input : manage_board *, SDL_Rect *                                               */
/* Output : SDL_Surface *                                                           */
/************************************************************************************/
/* purpose : the selector become a pawn if on it then don't                         */
/*                                                                                  */
/************************************************************************************/
SDL_Surface *selectPawn(manage_board *manBord, SDL_Rect *posi, int bool)
{
    if((manBord == NULL) || posi == NULL)
    {
        fprintf(stderr, "selectPawn() : no parameters available");
        exit(EXIT_FAILURE);
    }
    
    SDL_Surface *newTexture = NULL;
    if((newTexture = SDL_CreateRGBSurface(SDL_HWSURFACE, 60, 60, 32, 0, 0, 255, 0)) == NULL)
    {
        fprintf(stderr, "Unable to create RGB surface select_pawn : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    if(!bool)
    {
        if(manBord->boardSDL[posi->y][posi->x] == BLACK)
        {
            SDL_FreeSurface(newTexture);
            if((newTexture = IMG_Load("../files/textures/black_pawn.png")) == NULL)
            {
                fprintf(stderr, "Unable to load black_pawn.png : %s\n", IMG_GetError());
                exit(EXIT_FAILURE);
            }
            manBord->boardSDL[posi->y][posi->x] = EMPTY;
        }
    }
    else
    {
        manBord->boardSDL[posi->y][posi->x] = BLACK;   
    }
    
    return newTexture;
}

/************************************************************************************/
/* function : displaySelector                                                       */
/************************************************************************************/
/* Input : int, SDL_Rect *                                                          */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : this function display the pawn selector                                */
/************************************************************************************/
void displaySelector(SDL_Rect *pos, int direction)
{
    switch(direction)
    {
        case UP:
            if(pos->y - 1 < 0)
			{
				break;
			}
            pos->y--;
            break;
        case DOWN:
            if(pos->y + 1 > NB_BLOCK_HEIGHT - 1)// if player out of screen then don't
			{
				break;
			}
            pos->y++;
            break;
        case LEFT:
            if(pos->x - 1 < 0)// if player out of screen then don't
			{
				break;
			}
            pos->x--;
            break;
        case RIGHT:
            if(pos->x + 1 > NB_BLOCK_WIDTH - 1)// if player out of screen then don't
			{
				break;
			}
            pos->x++;
            break;
    }
}

/************************************************************************************/
/* function : displayBoard                                                          */
/************************************************************************************/
/* Input : SDL_Surface * x2                                                         */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : this function display the boardSDL and all of the variable             */
/* we need in it                                                                    */
/************************************************************************************/
void displayBoard(SDL_Surface *screen, SDL_Surface *img_board)
{
    if(screen == NULL)
    {
        fprintf(stderr, "displayBoard() : No parameters available");
        exit(EXIT_FAILURE);
    }

    SDL_Rect position_board;
    position_board.x = 0;
    position_board.y = 0;

    SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 255));
    SDL_BlitSurface(img_board, NULL, screen, &position_board);
}

/************************************************************************************/
/* function : displayPawn                                                           */
/************************************************************************************/
/* Input : SDL_Surface * x2, SDL_Rect *                                             */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : this function display one pawn at a position                           */
/*                                                                                  */
/************************************************************************************/
void updateMapPawn(SDL_Surface *screen, SDL_Surface *black_p, SDL_Surface *white_p, manage_board *man_mapPawn)
{
    if((screen == NULL) || (black_p == NULL) || (white_p == NULL) || (man_mapPawn == NULL))
    {
        fprintf(stderr, "displayPawn() : No parameters available");
        exit(EXIT_FAILURE);
    }

    SDL_Rect position;

    for(int i = 0; i < man_mapPawn->width; i++)
    {
        for(int j = 0; j<man_mapPawn->height; j++)
        {
            position.x = i * BLOCK_SIZE;
            position.y = j * BLOCK_SIZE;
            switch(man_mapPawn->boardSDL[j][i])
            {
                case BLACK:
                    SDL_BlitSurface(black_p, NULL, screen, &position);
                    break;
                case WHITE:
                    SDL_BlitSurface(white_p, NULL, screen, &position);
                    break;
            }
        }
    }
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
    free(manBoardD);
}