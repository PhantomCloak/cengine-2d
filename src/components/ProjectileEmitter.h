#pragma once

//#include "SDL_timer.h"
#include "glm/glm.hpp"

struct ProjectileEmitter {
    glm::vec2 particleVelocity;
    bool isFriendly;
    int particleDuration;
    int repeatFrequency;
    int lastEmitTime;
    int projectileDamage;

    ProjectileEmitter(bool isFriendly = false, glm::vec2 particleVelocity = glm::vec2(0), int particleDuration = 0, int repeatFrequency = 0, int damage = 0) {
        this->isFriendly = isFriendly;
        this->particleVelocity = particleVelocity;
        this->particleDuration = particleDuration;
        this->repeatFrequency = repeatFrequency;
        this->projectileDamage = damage;
        //this->lastEmitTime = SDL_GetTicks();
    }
};
