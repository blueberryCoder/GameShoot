//
// Created by blueberry on 2023/2/3.
//
#pragma once

#include <SDL.h>
#include "defs.h"

typedef struct Explosion {
    float x;
    float y;
    float dx;
    float dy;
    int r, g, b, a;
    struct Explosion *next;
} Explosion;

typedef struct Debris {
    float x;
    float y;
    float dx;
    float dy;
    SDL_Rect rect;
    SDL_Texture *texture;
    int life;
    struct Debris *next;
} Debris;

typedef struct {
    int x;
    int y;
    int speed;
} Star;

typedef struct {
    void (*logic)(void);

    void (*draw)(void);
} Delegate;

typedef struct {
    SDL_Renderer *renderer;
    SDL_Window *window;
    Delegate delegate;
    int keyboard[MAX_KEYBOARD_KEYS];
} App;

typedef struct Entity {
    float x;
    float y;
    int w;
    int h;
    float dx;
    float dy;

    int health;
    int reload;

    SDL_Texture *texture;
    struct Entity *next;

    int side;
} Entity;

typedef struct {
    struct Entity fighterHead, *fighterTail;
    struct Entity bulletHead, *bulletTail;

    Explosion explosionHead, *explosionTail;
    Debris debrisHead, *debrisTail;

    int score;

} Stage;
