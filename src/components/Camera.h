#pragma once

struct Camera{
  // in meters
  int fovWidth;
  // in meters
  int fovHeight;
  Camera(int fovWidth = 0, int fovHeight = 0){
    this->fovWidth = fovWidth;
    this->fovWidth = fovWidth;
  }
};
