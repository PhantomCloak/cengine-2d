#include "editor_utils.h"
#include "../core/coordinate_system.h"
#include "imgui.h"

float EditorUtils::pixelCordToUvX2(float x, int width) {
    return x / width;
}

float EditorUtils::pixelCordToUvY2(float y, int height) {
    return y / height;
}

glm::vec2 EditorUtils::GetWindowSize() {
    return glm::vec2(ImGui::GetWindowWidth(), ImGui::GetWindowHeight());
}

glm::vec2 EditorUtils::InterpolateToGrid(glm::vec2 vec, float gridSize) {
    vec.x = std::round(vec.x / gridSize) * gridSize;
    vec.y = std::round(vec.y / gridSize) * gridSize;
    return vec;
}
