#pragma once
#include "glm/glm.hpp"

class EditorUtils {
    public:
    static float pixelCordToUvX2(float x, int width);
    static float pixelCordToUvY2(float y, int height);
    static glm::vec2 GetWindowSize();
    static glm::vec2 InterpolateToGrid(glm::vec2 vec, int gridSize);
};
