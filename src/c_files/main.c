#include "settings.h"
#include "window.h"
#include "game.h"
#include <stdio.h>

int main()
{
    readSettings();

    //main function, that starts every important piece of game
    if(!windowInit())
    {
        printf("Failed to create window\n");
        return 1;
    }

    if(!gameInit())
    {
        printf("Failed to init game\n");
        return 1;
    }

    if(!startGameLoop())
    {
        printf("Failed to start game loop\n");
        return 1;
    }

    return 0;
}
