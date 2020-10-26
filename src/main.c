/*namefile : main.c

purpose : the main file

created by : Francois Suzeau

date : 25/10/2020

*/

#include "draughtGame/draught_game.h"

int main(int argc, char *argv[])
{
    (void) argc;
	(void) argv;

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
                    case SDLK_SPACE:
                        InGame(screen);
                        break;
                    default:
                        break;
                }
        }
        SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
        SDL_Flip(screen);
    }

    SDL_FreeSurface(screen);
    SDL_Quit();

    

    return EXIT_SUCCESS;
}