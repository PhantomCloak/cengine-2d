#ifndef SPRITE_H
#define SPRITE_H

#include "../render/render_primitives.h"
#include <string>

struct Sprite {
    int texture;
    int zIndex;
    bool isFixed;
    CommancheRect srcRect;
    CommancheColorRGBA color;

    Sprite(int texture = -1, int zIndex = 0, float srcRectX = 0, float srcRectY = 0, float srcWidth = 0, float srcHeight = 0) {
        this->texture = texture;
        this->zIndex = zIndex;
        this->srcRect = { srcRectX, srcRectY, srcWidth, srcHeight };
        color = CommancheColorRGBA { 255, 255, 255, 255 };
    }
};

#endif
