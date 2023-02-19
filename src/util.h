//
// Created by blueberry on 2023/2/11.
//

#pragma once

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

int collision(int x1, int y1, int w1, int h1, int x2, int y2, int w2, int h2);

void calcSlope(int x1, int y1, int x2, int y2, float *dx, float *dy);
