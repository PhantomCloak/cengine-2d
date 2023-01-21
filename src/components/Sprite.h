#ifndef SPRITE_H
#define SPRITE_H

#include <string>
#include "../render/render_primitives.h"

struct Sprite {
    int texture;
    int width;
    int height;
    int zIndex;
    bool isFixed;
    CommancheRect srcRect;

    Sprite(int texture = -1, int width = 1, int height = 1, int zIndex = 0, int srcRectX = 0, int srcRectY = 0) {
        this->texture = texture;
        this->width = width;
        this->height = height;
        this->zIndex = zIndex;
        this->srcRect = { (float)srcRectX, (float)srcRectY, (float)width, (float)height };
    }
};

#endif
