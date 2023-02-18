//
// Created by blueberry on 2023/2/1.
//

//#include <iostream>
// https://www.parallelrealities.co.uk/tutorials/shooter/shooter1.php


#include <SDL.h>
#include <SDL_thread.h>
#include "sound.h"

#include "structs.h"
#include "draw.h"
#include "init.h"
#include "input.h"
#include "stage.h"

void cleanup(void) {
    printf(" Function called --> exit\n");
    return;
}
static void capFrameRate(long *then, float *remainder)
{
    long wait, frameTime;

    wait = 16 + *remainder;

    *remainder -= (int)*remainder;

    frameTime = SDL_GetTicks() - *then;

    wait -= frameTime;

    if (wait < 1)
    {
        wait = 1;
    }

    SDL_Delay(wait);

    *remainder += 0.667;

    *then = SDL_GetTicks();
}

int main(int argc, char *argv[]) {

    long then;
    float remainder;
    memset(&app, 0, sizeof(App));


    initSDL();
    atexit(cleanup);
    initStage();
    then = SDL_GetTicks();
    remainder = 0;

    initSounds();

    while (1) {
        prepareScene();
        doInput();
        app.delegate.logic();
        app.delegate.draw();
        presentScene();
        capFrameRate(&then, &remainder);

    }

    return 0;
}
