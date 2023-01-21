#pragma once

#include <glm/glm.hpp>

struct RectTransform{
  glm::vec2 pos;
  glm::vec2 size;
  glm::vec2 scale;
  float rotation;

  RectTransform(glm::vec2 pos = glm::vec2(0, 0),glm::vec2 size = glm::vec2(0, 0), glm::vec2 scale = glm::vec2(1,1), double rotation = 0)
  {
    this->pos = pos;
    this->size = size;
    this->scale = scale;
    this->rotation = rotation;
  };
};
