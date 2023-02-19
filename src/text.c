//
// Created by blueberry on 2023/2/18.
//
#include "text.h"
#include "draw.h"
#include "defs.h"

#define GLYPH_WIDTH     18
#define GLYPH_HEIGHT    28

char drawTextBuffer[MAX_LINE_LENGTH];
SDL_Texture * fontTexture;

void initFonts(void) {
    fontTexture = loadTexture(SND_FONT_RES);
}

void drawText(int x, int y, int r, int g, int b, char *format, ...) {
    int      i, len, c;
    SDL_Rect rect;
    va_list  args;

    memset(&drawTextBuffer, '\0', sizeof(drawTextBuffer));

    va_start(args, format);
    vsprintf(drawTextBuffer, format, args);
    va_end(args);

    len = strlen(drawTextBuffer);

    rect.w = GLYPH_WIDTH;
    rect.h = GLYPH_HEIGHT;
    rect.y = 0;

    SDL_SetTextureColorMod(fontTexture, r, g, b);

    for (i = 0; i < len; i++) {
        c = drawTextBuffer[i];

        if (c >= ' ' && c <= 'Z') {
            rect.x = (c - ' ') * GLYPH_WIDTH;

            blitRect(fontTexture, &rect, x, y);

            x += GLYPH_WIDTH;
        }
    }
}
