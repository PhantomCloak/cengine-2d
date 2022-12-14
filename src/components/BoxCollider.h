#ifndef BOXCOLLIDER_H
#define BOXCOLLIDER_H

#include "glm/glm.hpp"

struct BoxCollider {
    int height;
    int width;
    glm::vec2 offset;
    BoxCollider(int height = 1, int width = 1, glm::vec2 offset = glm::vec2(0)) {
        this->height = height;
        this->width = width;
        this->offset = offset;
    }
};
#endif
