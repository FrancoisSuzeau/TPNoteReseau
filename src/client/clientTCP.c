/*namefile : main.c

purpose : the main file

created by : Francois Suzeau

date : 25/10/2020

*/

#include "draughtGame/draught_game.h"
#include "profil/profil.h"
#include "communication/communication.h"
#include "sdlboard/sdl.h"
#include <assert.h>

    

int main(int argc, char *argv[])
{
    (void) argc;
	(void) argv;

    manage_player *player = malloc(sizeof(manage_player));
    player->isConnected = FALSE;

    struct sockaddr_in ser_addr;
	int sk;
    char buf[MAX] = {""};                                                            
    int count;
    int connect_fd;
    
    bzero((char*) &ser_addr, sizeof(ser_addr));
    ser_addr.sin_family = AF_INET;
    ser_addr.sin_port = htons(12345);
    ser_addr.sin_addr.s_addr = inet_addr("127.0.0.1");

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

    connection_ask(player, sk);

    SDL_Surface *screen = sdlInit();

    initTexture();
    SDL_Surface *select_pawn = getSelector();
   
    manage_board *my_manBoard = malloc(sizeof(manage_board));
    initialiseBoard(my_manBoard);
    

    SDL_Rect position_selector;

    SDL_Rect localisation;
    position_selector.x = 0;
    position_selector.y = 0;
    
    displayBoard(screen);
    updateMapPawn(screen, my_manBoard);
    SDL_Flip(screen);

    int             continuer = TRUE;

    while(continuer)
    {
        continuer = handle_event(player, &position_selector, my_manBoard, &select_pawn);
        displayBoard(screen);
        localisation.x = position_selector.x * BLOCK_SIZE;
        localisation.y = position_selector.y * BLOCK_SIZE;
        SDL_BlitSurface(select_pawn, NULL, screen, &localisation);
        updateMapPawn(screen, my_manBoard);
        SDL_Flip(screen);
    }

    freeTexture();

    destroyBoard(my_manBoard);
    free(my_manBoard);

    SDL_FreeSurface(screen);
    SDL_Quit();

    free(player);

    buf[0] = '\0';
    strcat(buf, "quit");
    if((count = send(sk, buf, sizeof(buf), 0)) == -1)
    {
        perror("Client : send data failure : ");
        exit(EXIT_FAILURE);
    }
    puts(">>> Client : End of transmission");

    shutdown(sk, SHUT_RDWR);
    close(sk);
    
    return EXIT_SUCCESS;
}