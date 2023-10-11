#ifndef SPRITE_H
#define SPRITE_H

#include "../render/render_primitives.h"
#include <string>

struct Sprite {
    std::string textureId;
    int zIndex;
    bool isFixed;
    CommancheRect srcRect;
    CommancheColorRGBA color;
    int texture;

    Sprite(std::string textureStr = "", int zIndex = 0, float srcRectX = 0, float srcRectY = 0, float srcWidth = 0, float srcHeight = 0) {
        this->textureId = textureStr;
        this->zIndex = zIndex;
        this->srcRect = { srcRectX, srcRectY, srcWidth, srcHeight };
        texture = 0;
        color = CommancheColorRGBA { 255, 255, 255, 255 };
    }
};

#endif
