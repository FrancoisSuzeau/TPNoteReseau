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

    displayBoard(screen, img_board);
    updateMapPawn(screen, black_pawn, white_pawn, my_manBoard);
    SDL_Flip(screen);

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
                        select_pawn = selectPawn(my_manBoard, &position_selector);
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
    //SDL_FreeSurface(select_pawn);
    SDL_FreeSurface(img_board);

    destroyBoard(my_manBoard);
    //free(my_manBoard);
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
/* function : selectPawn                                                            */
/************************************************************************************/
/* Input : manage_board *, SDL_Rect *                                               */
/* Output : SDL_Surface *                                                           */
/************************************************************************************/
/* purpose : the selector become a pawn if on it then don't                         */
/*                                                                                  */
/************************************************************************************/
SDL_Surface *selectPawn(manage_board *manBord, SDL_Rect *posi)
{
    if((manBord == NULL) || posi == NULL)
    {
        fprintf(stderr, "selectPawn() : no parameters available");
        exit(EXIT_FAILURE);
    }
    
    SDL_Surface *newTexture = NULL;
    if((newTexture = SDL_CreateRGBSurface(SDL_HWSURFACE, 60, 60, 32, 0, 0, 255, 0)) == NULL) //TEMPORAIRE
    {
        fprintf(stderr, "Unable to create RGB surface select_pawn : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    if(manBord->selectIs == EMPTY) // the selector is empty
    {
        if(manBord->board_blackCase[posi->y][posi->x] == 0) // the selector is on a white case
        {
            if((newTexture = SDL_CreateRGBSurface(SDL_HWSURFACE, 60, 60, 32, 0, 0, 255, 0)) == NULL) //stay black
            {
                fprintf(stderr, "Unable to create RGB surface select_pawn : %s\n", SDL_GetError());
                exit(EXIT_FAILURE);
            }
            manBord->selectIs = EMPTY;

        }
        else if(manBord->board_blackCase[posi->y][posi->x] != 0) // the selector is on brown case
        {
            if(manBord->boardSDL[posi->y][posi->x] == BLACK) //the case is not empty and with a black paw
            {
                if((validateSelect(manBord, posi) == TRUE) || validateTaken(manBord, posi) == TRUE)
                {
                    if((newTexture = IMG_Load("../files/textures/black_pawn.png")) == NULL) // Become black pawn TEMPORAIRE
                    {
                        fprintf(stderr, "selectPawn : unable to load black_pawn.png : %s", IMG_GetError());
                        exit(EXIT_FAILURE);
                    }
                    manBord->selectIs = BLACK;
                    manBord->boardSDL[posi->y][posi->x] = EMPTY;
                }
                
                
            }
            else if(manBord->boardSDL[posi->y][posi->x] == WHITE) //the selector is on white pawn
            {

                if((validateSelect(manBord, posi) == TRUE) || validateTaken(manBord, posi) == TRUE)
                {
                    if((newTexture = IMG_Load("../files/textures/white_pawn.png")) == NULL) // Become white pawn
                    {
                        fprintf(stderr, "selectPawn : unable to load black_pawn.png : %s", IMG_GetError());
                        exit(EXIT_FAILURE);
                    }
                    manBord->selectIs = WHITE;
                    manBord->boardSDL[posi->y][posi->x] = EMPTY;
                }

            }
            else if(manBord->boardSDL[posi->y][posi->x] == EMPTY)
            {
                if((newTexture = SDL_CreateRGBSurface(SDL_HWSURFACE, 60, 60, 32, 0, 0, 255, 0)) == NULL)
                {
                    fprintf(stderr, "Unable to create RGB surface select_pawn : %s\n", SDL_GetError());
                    exit(EXIT_FAILURE);
                }
                manBord->selectIs = EMPTY;
            }
        }
    }
    else if(manBord->selectIs == BLACK) // the selector had select a black pawn
    {
        if(manBord->board_blackCase[posi->y][posi->x] == 0) // is on a white case
        {
            if((newTexture = IMG_Load("../files/textures/black_pawn.png")) == NULL) //stay a black pawn
            {
                fprintf(stderr, "selectPawn : unable to load black_pawn.png : %s", IMG_GetError());
                exit(EXIT_FAILURE);
            }
            manBord->selectIs = BLACK;
        }
        else if(manBord->board_blackCase[posi->y][posi->x] != 0) // the selector is on brown case
        {
            if(manBord->boardSDL[posi->y][posi->x] == BLACK) //on a black pawn
            {
                if((newTexture = IMG_Load("../files/textures/black_pawn.png")) == NULL) //stay black pawn
                {
                    fprintf(stderr, "selectPawn : unable to load black_pawn.png : %s", IMG_GetError());
                    exit(EXIT_FAILURE);
                }
                manBord->selectIs = BLACK;
                
            }
            else if(manBord->boardSDL[posi->y][posi->x] == WHITE) //is on a white pawn so his doing nothing
            {
                if((newTexture = IMG_Load("../files/textures/black_pawn.png")) == NULL) //Stay black pawn
                {
                    fprintf(stderr, "selectPawn : unable to load black_pawn.png : %s", IMG_GetError());
                    exit(EXIT_FAILURE);
                }
                manBord->selectIs = BLACK;
            }
            else if(manBord->boardSDL[posi->y][posi->x] == EMPTY) //selector on empty 
            {
                if((newTexture = SDL_CreateRGBSurface(SDL_HWSURFACE, 60, 60, 32, 0, 0, 255, 0)) == NULL) //drop the black pawn and rebecome black
                {
                    fprintf(stderr, "Unable to create RGB surface select_pawn : %s\n", SDL_GetError());
                    exit(EXIT_FAILURE);
                }
                manBord->selectIs = EMPTY;
                manBord->boardSDL[posi->y][posi->x] = BLACK;
            }
        }
    }
    else if(manBord->selectIs == WHITE) //the selector had choose a white pawn
    //TEMPORAIRE : quand client et serveur implementer on devrai choisir si joueur blanc ou noir ou spectacteur 
    {
        if(manBord->board_blackCase[posi->y][posi->x] == 0) // is on a white case
        {
            if((newTexture = IMG_Load("../files/textures/white_pawn.png")) == NULL) //stay a black pawn
            {
                fprintf(stderr, "selectPawn : unable to load white_pawn.png : %s", IMG_GetError());
                exit(EXIT_FAILURE);
            }
            manBord->selectIs = WHITE;
        }
        else if(manBord->board_blackCase[posi->y][posi->x] != 0) // the selector is on brown case
        {
            if(manBord->boardSDL[posi->y][posi->x] == BLACK) //on a black pawn
            {
                if((newTexture = IMG_Load("../files/textures/white_pawn.png")) == NULL) //stay black pawn
                {
                    fprintf(stderr, "selectPawn : unable to load white_pawn.png : %s", IMG_GetError());
                    exit(EXIT_FAILURE);
                }
                manBord->selectIs = WHITE;
                
            }
            else if(manBord->boardSDL[posi->y][posi->x] == WHITE) //is on a white pawn so his doing nothing
            {
                if((newTexture = IMG_Load("../files/textures/white_pawn.png")) == NULL) //Stay black pawn
                {
                    fprintf(stderr, "selectPawn : unable to load white_pawn.png : %s", IMG_GetError());
                    exit(EXIT_FAILURE);
                }
                manBord->selectIs = WHITE;
            }
            else if(manBord->boardSDL[posi->y][posi->x] == EMPTY) //selector on empty 
            {
                if((newTexture = SDL_CreateRGBSurface(SDL_HWSURFACE, 60, 60, 32, 0, 0, 255, 0)) == NULL) //drop the black pawn and rebecome black
                {
                    fprintf(stderr, "Unable to create RGB surface select_pawn : %s\n", SDL_GetError());
                    exit(EXIT_FAILURE);
                }
                manBord->selectIs = EMPTY;
                if(validateSelect(manBord, posi))
                {
                    
                }
                manBord->boardSDL[posi->y][posi->x] = WHITE;
            }
        }
    }
    
    return newTexture;
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
    //beging with the edge
    if((pos->y - 2 == - 1) || (pos-> y - 1) == -1)
    {
        switch(pos->x + 2)
        {
            case NB_BLOCK_WIDTH:
                if(manBoard->boardSDL[pos->y + 2][pos->x - 2] == EMPTY)
                {
                    bool = TRUE;
                }
                break;
            default:
                if((manBoard->boardSDL[pos->y + 2][pos->x - 2] == EMPTY) || (manBoard->boardSDL[pos->y + 2][pos->x + 2] == EMPTY))
                {
                    bool = TRUE;
                }
                break;
        }
    }
    else if((pos->y + 2 == NB_BLOCK_HEIGHT) || (pos->y + 1 == NB_BLOCK_HEIGHT))
    {
        switch(pos->x - 2)
        {
            case -1:
                if(manBoard->boardSDL[pos->y - 2][pos->x + 2] == EMPTY)
                {
                    bool = TRUE;
                }
                break;
            default:
                if((manBoard->boardSDL[pos->y - 2][pos->x + 2] == EMPTY) || (manBoard->boardSDL[pos->y - 2][pos->x - 2] == EMPTY))
                {
                    bool = TRUE;
                }
                break;
        }

    }
    else if(pos->x - 1 == - 1)
    {
        if((manBoard->boardSDL[pos->y - 2][pos->x + 2] == EMPTY) || (manBoard->boardSDL[pos->y + 2][pos->x + 2] == EMPTY))
        {
            bool = TRUE;
        }

    }
    else if(pos->x + 1 == NB_BLOCK_WIDTH)
    {
        if((manBoard->boardSDL[pos->y + 2][pos->x - 2] == EMPTY) || (manBoard->boardSDL[pos->y - 2][pos->x - 2] == EMPTY))
        {
            bool = TRUE;
        }

    }
    else //all others who aren't on the edge
    {
        if((manBoard->boardSDL[pos->y + 2][pos->x-2] == EMPTY) || (manBoard->boardSDL[pos->y + 2][pos->x+2] == EMPTY)
        || (manBoard->boardSDL[pos->y - 2][pos->x - 2] == EMPTY) || (manBoard->boardSDL[pos-> y - 2][pos->x + 2] == EMPTY))
        {
            bool = TRUE;
        }   
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
    if(position->y - 1 == - 1)
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
    //free(manBoardD);
}