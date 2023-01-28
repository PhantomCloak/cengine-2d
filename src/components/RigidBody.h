#pragma once
#include "glm/glm.hpp"
#include "box2d/box2d.h"

struct RigidBody {
  int rigId;
  glm::vec2 velocityLinear;
  float velocityAngular;
  glm::vec2 forceAcc;
  bool isInit;
  bool isStatic;
  bool isFixedRot;
  float mass;
  float restution;
  float density;
  float friction;

  RigidBody(bool isStatic = false, float restution = 0.0f, bool isFixedRot = false, glm::vec2 velocity = glm::vec2(0, 0))
  {
    forceAcc = glm::vec2(0,0);
    this->velocityLinear = velocity;
    this->isStatic = isStatic;
    this->restution = restution;
    this->isFixedRot = isFixedRot;
    friction = 1;
    mass = 1;
    density = 1;
    isInit = false;
  }
};
