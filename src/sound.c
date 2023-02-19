
#include <SDL_mixer.h>
#include "common.h"
#include "sound.h"

void loadSounds(void);

void initSounds(void) {
    memset(sounds, 0, sizeof(Mix_Chunk *) * SND_MAX);
    loadSounds();
}

void loadSounds(void) {
    sounds[SND_PLAYER_FIRE] = Mix_LoadWAV(SND_PLAYER_FIRE_RES);
    sounds[SND_ALIEN_FIRE]  = Mix_LoadWAV(SND_ALIEN_FIRE_RES);
    sounds[SND_PLAYER_DIE]  = Mix_LoadWAV(SND_PLAYER_DIE_RES);
    sounds[SND_ALIEN_DIE]   = Mix_LoadWAV(SND_ALIEN_DIE_RES);
    sounds[SND_POINTS]      = Mix_LoadWAV(SND_POINTS_RES);
}

void loadMusic(char *filename) {
    if (music != NULL) {
        Mix_HaltMusic();
        Mix_FreeMusic(music);
        music = NULL;
    }
    music = Mix_LoadMUS(filename);
}

void playMusic(int loop) {
    Mix_PlayMusic(music, loop ? -1 : 0);
}

void playSound(int id, int channel) {
    Mix_PlayChannel(channel, sounds[id], 0);
}