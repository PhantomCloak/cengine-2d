#ifndef ANIMATION_H
#define ANIMATION_H

#include "SDL_timer.h"
#include "glm/glm.hpp"

struct Animation {
  int numFrames;
  int currentFrame;
  int frameSpeed;
  bool isLoop;
  int startTime;

  Animation(int numFrames = 1, int frameSpeed = 1, bool isLoop = true)
  {
    this->numFrames = numFrames;
    this->frameSpeed = frameSpeed;
    this->isLoop = isLoop;
    startTime = SDL_GetTicks();
  };
};
#endif
