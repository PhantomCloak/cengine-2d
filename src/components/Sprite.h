#ifndef SPRITE_H
#define SPRITE_H

#include "SDL_render.h"
#include <string>

struct Sprite {
    int texture;
    int width;
    int height;
    int zIndex;
    bool isFixed;
    SDL_Rect srcRect;

    Sprite(int texture = -1, int width = 1, int height = 1, int zIndex = 0, int srcRectX = 0, int srcRectY = 0) {
        this->texture = texture;
        this->width = width;
        this->height = height;
        this->zIndex = zIndex;
        this->srcRect = { srcRectX, srcRectY, width, height };
    }
};

#endif
