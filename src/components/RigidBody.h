#pragma once
#include "glm/glm.hpp"
#include "box2d/box2d.h"

struct RigidBody {
  glm::vec2 velocity;
  glm::vec2 forceAcc;
  bool isInit;
  bool isStatic;
  float restution;

  RigidBody(bool isStatic = false, float restution = 0.0f, glm::vec2 velocity = glm::vec2(0, 0))
  {
    forceAcc = glm::vec2(0,0);
    this->velocity = velocity;
    this->isStatic = isStatic;
    this->restution = restution;
    isInit = false;
  }
};
