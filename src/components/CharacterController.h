#pragma once
#include <glm/glm.hpp>

struct CharacterController {
    glm::vec2 upVelocity;
    glm::vec2 rightVelocity;
    glm::vec2 downVelocity;
    glm::vec2 leftVelocity;

    CharacterController (float velocity = 0) {
        this->upVelocity = glm::vec2(0, -velocity);
        this->rightVelocity = glm::vec2(velocity, 0);
        this->downVelocity = glm::vec2(0, velocity);
        this->leftVelocity = glm::vec2(-velocity,0);
    }
};

