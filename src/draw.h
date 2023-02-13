//
// Created by blueberry on 2023/2/3.
//
#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include "structs.h"
#include "init.h"

SDL_Texture * loadTexture(char *filename);
void blit(SDL_Texture * texture,int x,int y);

void prepareScene(void);

void presentScene(void);
