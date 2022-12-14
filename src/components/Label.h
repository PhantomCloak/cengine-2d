#pragma once

#include "SDL_pixels.h"
#include "glm/glm.hpp"
#include <string>

struct Label {
  glm::vec2 pos;
  std::string text;
  std::string fontName;
  SDL_Color textColor;
  bool isFixed;

  Label(glm::vec2 pos = glm::vec2(0), std::string text = "", std::string fontName = "charriot-font", const SDL_Color& textColor = {0, 0, 0}, bool isFixed = true){
    this->pos = pos;
    this->text = text;
    this->fontName = fontName;
    this->textColor = textColor;
    this->isFixed = isFixed;
  }
};
