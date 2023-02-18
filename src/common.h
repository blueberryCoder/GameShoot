//
// Created by blueberry on 2023/2/3.
//

#pragma once
#include "structs.h"
#include "defs.h"
#include <SDL_mixer.h>
App  app;
Entity* player;
Entity* bullet;
Stage  stage;
Mix_Chunk* sounds[SND_MAX];
Mix_Music* music;

#define PLAY_RES            "../res/player.png"
#define BULLET_RES          "../res/playerBullet.png"
#define ALIEN_BULLET_RES    "../res/alienBullet.png"
#define ENEMY_RES           "../res/enemy.png"
#define BACKGROUND_RES      "../res/background.png"
#define EXPLOSION_RES       "../res/explosion.png"
#define SND_PLAYER_FIRE_RES "../res/sound/334227__jradcoolness__laser.ogg"
#define SND_ALIEN_FIRE_RES  "../res/sound/196914__dpoggioli__laser-gun.ogg"
#define SND_PLAYER_DIE_RES  "../res/sound/245372__quaker540__hq-explosion.ogg"
#define SND_ALIEN_DIE_RES   "../res/sound/10 Guage Shotgun-SoundBible.com-74120584.ogg"

