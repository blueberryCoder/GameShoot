//
// Created by blueberry on 2023/2/18.
//
#pragma once
#include "common.h"

#define GLYPH_WIDTH     18
#define GLYPH_HEIGHT    28


void initFonts(void);
void drawText(int x, int y, int r, int g, int b,int align, char *format, ...);

