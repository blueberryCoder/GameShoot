//
// Created by blueberry on 2023/2/3.
//
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>

#include "common.h"
#include "defs.h"
#include "highscore.h"
#include "sound.h"
#include "text.h"
#include "background.h"

void initGame(void) {
    initBackground();
    initStarfield();
    initSounds();
    initFonts();
    initHighscoreTable();
    loadMusic(MUSIC_RES);
    playMusic(1);
}

void initSDL(void)
{
	int rendererFlags, windowFlags;

	rendererFlags = SDL_RENDERER_ACCELERATED;

	windowFlags = 0;


	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		printf("Couldn't initialize SDL: %s\n", SDL_GetError());
		exit(1);
	}

	app.window = SDL_CreateWindow("Shooter 01", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, windowFlags);

	if (!app.window)
	{
		printf("Failed to open %d x %d window: %s\n", SCREEN_WIDTH, SCREEN_HEIGHT, SDL_GetError());
		exit(1);
	}

	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");

	app.renderer = SDL_CreateRenderer(app.window, -1, rendererFlags);

	if (!app.renderer)
	{
		printf("Failed to create renderer: %s\n", SDL_GetError());
		exit(1);
	}

    SDL_ShowCursor(0);
    IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG);

    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 1024) == -1) {
        printf("Couldn't initialize SDL Mixer \n");
        exit(1);
    }
    Mix_AllocateChannels(MAX_SND_CHANNELS);
}