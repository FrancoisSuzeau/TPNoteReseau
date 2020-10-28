/*namefile : main.c

purpose : the main file

created by : Francois Suzeau

date : 25/10/2020

*/

#include "draughtGame/draught_game.h"
#include "profil/profil.h"

int main(int argc, char *argv[])
{
    (void) argc;
	(void) argv;

    manage_player *player = malloc(sizeof(manage_player*));
    player->which_one = PLAYER1;
    player->isConnected = FALSE;
    readDataBase(player);

    struct sockaddr_in ser_addr;
	int sk;
    char buf[MAX];                                                            
    int count;
    int connect_fd;
    

    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(12345);

    if((inet_aton("127.0.0.1", (struct in_addr *)&ser_addr.sin_addr)) == 0)
    {
    	perror(" Client : Failure inet_aton");
    	exit(EXIT_FAILURE);
    }

    if((sk = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("Client : Failure create socket : ");
        exit(EXIT_FAILURE);
    }

    
    if((connect_fd = connect(sk, (struct sockaddr *)&ser_addr, sizeof(ser_addr))) == -1)
    {
        perror("Client : Failure connect function : \n");
        exit(EXIT_FAILURE);
    }

    for(int i = 0; i < 8; i++)
    {
        ser_addr.sin_zero[i] = 0;
    }

    while (strcmp(buf, "quit"))
    {
        buf[0] = '\0';
        if((player->isConnected == FALSE) && (nb_connection == PLAYER1))
        {
            printf("%d\n", nb_connection);
            logIn(player);
            strcat(buf, player->name);
            player->isConnected = TRUE;
        }
        if((count = sendto(sk, buf, sizeof(buf), 0, (struct sockaddr*)&ser_addr, sizeof(ser_addr))) == -1)
        {
            perror("Client : send data failure : ");
            exit(EXIT_FAILURE);
        }

        else
        {
            printf(" Client : Here is the message send : %s and send count = %d\n", buf, count);
            buf[0] = '\0';
            strcat(buf, "quit");
        }
    }
    puts(">>> Client : End of transmission");
    /*if(SDL_Init(SDL_INIT_VIDEO) != 0)
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
    SDL_Quit();*/

    

    free(player);
    return EXIT_SUCCESS;
}