#include "cursor.h"
#include "../log/log.h"
#include "imgui.h"
#include <GLFW/glfw3.h>
#include <iostream>

GLFWwindow* Cursor::wnd;
void Cursor::Setup(void* window) {
    wnd = (GLFWwindow*)window;
}

glm::vec2 Cursor::GetCursorPosition() {
    double x = 0;
    double y = 0;
    glfwGetCursorPos(wnd, &x, &y);
    return glm::vec2(x, y);
}

glm::vec2 Cursor::GetCursorWorldPosition() {
    //float left = worldBounds[0];
    //float right = worldBounds[1];
    //float bottom = worldBounds[2];
    //float top = worldBounds[3];

    //// Calculate scales
    //float Sx = screenSize.x / (right - left);
    //float Sy = screenSize.y / (top - bottom);

    //// Calculate inverse scales
    //float invSx = 1.0f / Sx;
    //float invSy = 1.0f / Sy;

    //// Transform the screen point using the inverse scales
    //float worldX = screenPoint.x * invSx + left;
    //float worldY = (screenSize.y - screenPoint.y) * invSy + bottom; // Invert Y-axis

    //return glm::vec2(worldX, worldY);
    return glm::vec2(0, 0);
}
