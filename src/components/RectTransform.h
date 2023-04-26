#pragma once

#include <glm/glm.hpp>

struct RectTransform{
  glm::vec2 pos;
  glm::vec2 size;
  glm::vec2 scale;
  float rotation;

  RectTransform(glm::vec2 pos = glm::vec2(0.0f, 0.0f),glm::vec2 size = glm::vec2(0.0f, 0.0f), glm::vec2 scale = glm::vec2(1.0f,1.0f), double rotation = 0)
  {
    this->pos = pos;
    this->size = size;
    this->scale = scale;
    this->rotation = rotation;
  };
};
