#pragma once

#include "glm/glm.hpp"
#include "../render/render_primitives.h"
#include <string>

struct Label {
  glm::vec2 pos;
  std::string text;
  int size;
  CommancheColorRGB textColor;
  bool isFixed;

  Label(glm::vec2 pos = glm::vec2(0), std::string text = "", int size = 20, const CommancheColorRGB& textColor = {0, 0, 0}, bool isFixed = true){
    this->pos = pos;
    this->text = text;
    this->textColor = textColor;
    this->isFixed = isFixed;
    this->size = size;
  }
};
