#pragma once
#include <glm/glm.hpp>

struct CharacterController {
    glm::vec2 upVelocity;
    glm::vec2 rightVelocity;
    glm::vec2 downVelocity;
    glm::vec2 leftVelocity;

    bool left;
    bool right;
    bool up;
    bool down;

    CharacterController(float upVelocity = 0, float leftVelocity = 0, float rightVelocity = 0, float downVelocity = 0) {
        this->upVelocity = glm::vec2(0, -upVelocity);
        this->rightVelocity = glm::vec2(rightVelocity, 0);
        this->downVelocity = glm::vec2(0, downVelocity);
        this->leftVelocity = glm::vec2(-leftVelocity, 0);
    }
};
