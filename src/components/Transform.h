#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>

struct Transform{
  glm::vec2 pos;
  glm::vec2 scale;
  float rotation;

  Transform(glm::vec2 pos = glm::vec2(0, 0), glm::vec2 scale = glm::vec2(1,1), double rotation = 0)
  {
    this->pos = pos;
    this->scale = scale;
    this->rotation = rotation;
  };
};

#endif
