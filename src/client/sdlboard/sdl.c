/*namefile : communication.c

purpose : src file for module draught_game where all signature of function are for this module

created by : Francois Suzeau

date : 31/10/2020

*/

#include "sdl.h"

    static SDL_Surface *img_board = NULL;
    static SDL_Surface *white_pawn = NULL;
    static SDL_Surface *black_pawn = NULL;
    static SDL_Surface *black_square = NULL;

/************************************************************************************/
/* function : initialiseBoard                                                       */
/************************************************************************************/
/* Input : manage_board *                                                           */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : this function initialise the map pawn at the beginning                 */
/*                                                                                  */
/************************************************************************************/
SDL_Surface * sdlInit()
{
    if(SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        fprintf(stderr, "Unable to initialize SDL : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    SDL_Surface     *screen = NULL;

    if((screen = SDL_SetVideoMode(WINDOW_WIDTH, WINDOW_HEIGHT, 32, SDL_HWSURFACE | SDL_DOUBLEBUF)) == NULL)
    {
        fprintf(stderr, "Unable to launch video mode : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }

    return screen;
}

/************************************************************************************/
/* function : initTexture                                                           */
/************************************************************************************/
/* Input : void                                                                     */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : create the file for each player capabilities                           */
/************************************************************************************/
void initTexture()
{
    if((img_board = IMG_Load("sdlboard/textures/board.png")) == NULL)
    {
        fprintf(stderr, "Unable to load board.png : %s\n", IMG_GetError());
        exit(EXIT_FAILURE);
    }
    if((white_pawn = IMG_Load("sdlboard/textures/white_pawn.png")) == NULL)
    {
        fprintf(stderr, "Unable to load white_pawn.png : %s\n", IMG_GetError());
        exit(EXIT_FAILURE);
    }
    if((black_pawn = IMG_Load("sdlboard/textures/black_pawn.png")) == NULL)
    {
        fprintf(stderr, "Unable to load black_pawn.png : %s\n", IMG_GetError());
        exit(EXIT_FAILURE);
    }
    if((black_square = SDL_CreateRGBSurface(SDL_HWSURFACE, 60, 60, 32, 255, 0, 0, 0)) == NULL)
    {
        fprintf(stderr, "Unable to create RGB surface black_square : %s\n", SDL_GetError());
        exit(EXIT_FAILURE);
    }
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
void updateMapPawn(SDL_Surface *screen, manage_board *man_mapPawn)
{
    if((screen == NULL) || (black_pawn == NULL) || (white_pawn == NULL) || (man_mapPawn == NULL))
    {
        fprintf(stderr, "updateMapPawn : No parameters available");
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
                    SDL_BlitSurface(black_pawn, NULL, screen, &position);
                    break;
                case WHITE:
                    SDL_BlitSurface(white_pawn, NULL, screen, &position);
                    break;
            }
        }
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
void displayBoard(SDL_Surface *screen)
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
    newTexture = black_square;

    if(manBord->selectIs == EMPTY) // the selector is empty
    {
        if(manBord->board_blackCase[posi->y][posi->x] == 0) // the selector is on a white case
        {
            newTexture = black_square;
            manBord->selectIs = EMPTY;

        }
        else if(manBord->board_blackCase[posi->y][posi->x] != 0) // the selector is on brown case
        {
            if(manBord->boardSDL[posi->y][posi->x] == BLACK) //the case is not empty and with a black paw
            {
                manBord->selectIs = BLACK;
                if((validateSelect(manBord, posi) == TRUE) || validateTaken(manBord, posi) == TRUE)
                {
                    newTexture = black_pawn;
                  
                    
                    manBord->boardSDL[posi->y][posi->x] = EMPTY;
                }
                else
                {    
                    manBord->selectIs = EMPTY;
                }
            }
            else if(manBord->boardSDL[posi->y][posi->x] == WHITE) //the selector is on white pawn
            {
                manBord->selectIs = WHITE;
                if((validateSelect(manBord, posi) == TRUE) || validateTaken(manBord, posi) == TRUE)
                {
                    newTexture = white_pawn;
                    
                    manBord->boardSDL[posi->y][posi->x] = EMPTY;
                }
                else
                {
                    manBord->selectIs = EMPTY;
                }

            }
            else if(manBord->boardSDL[posi->y][posi->x] == EMPTY)
            {
                newTexture = black_square;
                manBord->selectIs = EMPTY;
            }
        }
    }
    else if(manBord->selectIs == BLACK) // the selector had select a black pawn
    {
        if(manBord->board_blackCase[posi->y][posi->x] == 0) // is on a white case
        {
            newTexture = black_pawn;
            manBord->selectIs = BLACK;
        }
        else if(manBord->board_blackCase[posi->y][posi->x] != 0) // the selector is on brown case
        {
            if(manBord->boardSDL[posi->y][posi->x] == BLACK) //on a black pawn
            {
                newTexture = black_pawn;
                manBord->selectIs = BLACK;
                
            }
            else if(manBord->boardSDL[posi->y][posi->x] == WHITE) //is on a white pawn so his doing nothing
            {
                newTexture = black_pawn;
                manBord->selectIs = BLACK;
            }
            else if(manBord->boardSDL[posi->y][posi->x] == EMPTY) //selector on empty 
            {
                if((validateSelect(manBord, posi) == TRUE) || validateTaken(manBord, posi) == TRUE)
                {
                    newTexture = black_square;
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
            newTexture = white_pawn;
         
            manBord->selectIs = WHITE;
        }
        else if(manBord->board_blackCase[posi->y][posi->x] != 0) // the selector is on brown case
        {
            if(manBord->boardSDL[posi->y][posi->x] == BLACK) //on a black pawn
            {
                newTexture = white_pawn;

                manBord->selectIs = WHITE;
                
            }
            else if(manBord->boardSDL[posi->y][posi->x] == WHITE) //is on a white pawn so his doing nothing
            {
                newTexture = white_pawn;
                manBord->selectIs = WHITE;
            }
            else if(manBord->boardSDL[posi->y][posi->x] == EMPTY) //selector on empty 
            {
                
                if((validateSelect(manBord, posi) == TRUE) || validateTaken(manBord, posi) == TRUE)
                {
                   newTexture = black_square;
                }
                manBord->selectIs = EMPTY;
                manBord->boardSDL[posi->y][posi->x] = WHITE;
            }
        }
    }
    return newTexture;
}

/************************************************************************************/
/* function : moveSelector                                                       */
/************************************************************************************/
/* Input : int, SDL_Rect *                                                          */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : this function display the pawn selector                                */
/************************************************************************************/
void moveSelector(SDL_Rect *pos, int direction)
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
/* function : moveSelector                                                          */
/************************************************************************************/
/* Input : int, SDL_Rect *                                                          */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : this function display the pawn selector                                */
/************************************************************************************/
int handle_event(manage_player *player, SDL_Rect *position_selector, manage_board *my_manBoard, SDL_Surface **select_pawn)
{
    SDL_Event       event;
    int bool = TRUE;
    if(player->which_one == PLAYER1)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                bool = FALSE;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        bool = FALSE;
                        break;
                    case SDLK_UP:
                        moveSelector(position_selector, UP);
                        break;
                    case SDLK_DOWN:
                        moveSelector(position_selector, DOWN);
                        break;
                    case SDLK_LEFT:
                        moveSelector(position_selector, LEFT);
                        break;
                    case SDLK_RIGHT:
                        moveSelector(position_selector, RIGHT);
                        break;
                    case SDLK_SPACE:
                        *select_pawn = selectPawn(my_manBoard, position_selector);
                        break;
                    default:
                        break;
                }
        }
    }
    else if(player->which_one == PLAYER2)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                bool = FALSE;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        bool = FALSE;
                        break;
                    default:
                        break;
                }
        }
    }
    else if(player->which_one == VIEWER)
    {
        SDL_WaitEvent(&event);
        switch(event.type)
        {
            case SDL_QUIT:
                bool = FALSE;
                break;
            case SDL_KEYDOWN:
                switch(event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        bool = FALSE;
                        break;
                    default:
                        break;
                }
        }
    }
    
    return bool;
}

/************************************************************************************/
/* function : freeTexture                                                           */
/************************************************************************************/
/* Input : void                                                                     */
/* Output : void                                                                    */
/************************************************************************************/
/* purpose : create the file for each player capabilities                           */
/************************************************************************************/
void freeTexture()
{
    SDL_FreeSurface(white_pawn);
    SDL_FreeSurface(black_pawn);
    SDL_FreeSurface(black_square);
    SDL_FreeSurface(img_board);
}

SDL_Surface *getSelector()
{
    return black_square;
}