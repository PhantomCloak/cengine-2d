#pragma once
#include <glm/glm.hpp>

struct PhysicsMetadata {
    int identifier;
};

struct PhysicsResult {
  glm::vec2 velocityLinear;
  float velocityAngular;
  float rotation;
  float mass;
  glm::vec2 position;
};
