//
// Created by blueberry on 2023/2/3.
//
#pragma once
#include <SDL.h>
#include "defs.h"

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
    struct Entity * next;

// #define SIDE_PLAYER  0
// #define SIDE_ALIEN  1
// one of them
    int side;
} Entity;

typedef struct {

   struct Entity fighterHead, *fighterTail;
   struct Entity bulletHead, *bulletTail;

} Stage;
