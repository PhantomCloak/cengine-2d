#include "cursor.h"
#include "../render/render.h"
#include "../log/log.h"
#include "imgui.h"
#include <GLFW/glfw3.h>
#include <iostream>
#if EDITOR
#include "../editor/editor.h"
#endif

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

glm::vec2 Cursor::GetCursorWorldPosition(glm::vec2 screenPoint) {
    glm::mat4& ProjectionMat = CommancheRenderer::ProjectionMat;

    float left = (-1.0f - ProjectionMat[3][0]) / ProjectionMat[0][0];
    float right = (1.0f - ProjectionMat[3][0]) / ProjectionMat[0][0];
    float bottom = (-1.0f - ProjectionMat[3][1]) / ProjectionMat[1][1];
    float top = (1.0f - ProjectionMat[3][1]) / ProjectionMat[1][1];

    left = CommancheRenderer::vo * 2;
    right *= 2;
    bottom *= 2;
    top = CommancheRenderer::ho * 2;

    glm::vec2 screenSize;
#if EDITOR
    screenSize = Editor::ScreenSize;
#endif
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
