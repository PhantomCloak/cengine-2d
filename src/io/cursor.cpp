#include "cursor.h"
#include "../editor/editor.h"
#include "../log/log.h"
#include "../render/render.h"
#include "imgui.h"
#include "raylib.h"
#include <GLFW/glfw3.h>
#include <iostream>

GLFWwindow* Cursor::wnd;
void Cursor::Setup(void* window) {
    wnd = (GLFWwindow*)window;
}

glm::vec2 Cursor::GetCursorPosition() {
    double x = 0;
    double y = 0;
    auto v = GetMousePosition();
    x = v.x;
    y = v.y;
    return glm::vec2(x, y);
}

glm::vec2 Cursor::GetCursorWorldPosition(glm::vec2 screenPoint) {
    float left = CommancheRenderer::vo;
    float right = CommancheRenderer::screenWidth;
    float bottom = CommancheRenderer::screenHeight;
    float top = CommancheRenderer::ho;

    glm::vec2 screenSize;
    screenSize = Editor::ScreenSize;
    // Calculate scales
    float Sx = screenSize.x / (right - left);
    float Sy = screenSize.y / (top - bottom);

    // Calculate inverse scales
    float invSx = 1.0f / Sx;
    float invSy = 1.0f / Sy;


    // Transform the screen point using the inverse scales
    float worldX = screenPoint.x * invSx + left;
    float worldY = (screenSize.y - screenPoint.y) * invSy + bottom; // Invert Y-axis

    return glm::vec2(worldX, worldY);
    return glm::vec2(0, 0);
}

bool Cursor::HasLeftCursorClicked() {
    return IsMouseButtonPressed(0);
}

bool Cursor::HasRightCursorClicked() {
    return IsMouseButtonPressed(1);
}
