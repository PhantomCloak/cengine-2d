#include "../../../render/render.h"
#include "../../cursor.h"
#include "../../../core/coordinate_system.h"
#include <GLFW/glfw3.h>

#if EDITOR
#include "../../../editor/editor.h"
#endif

static GLFWwindow* wnd;

//extern "C" double getScreenScaleFactor();

void Cursor::Setup(void* window) {
    wnd = (GLFWwindow*)window;
}

glm::vec2 Cursor::GetCursorPosition() {
    double x = 0;
    double y = 0;

    glfwGetCursorPos(wnd, &x, &y);
    return glm::vec2(x, y);
}

glm::vec2 Cursor::GetCursorWorldPosition(glm::vec2 screenPoint, CommancheCamera cam) {
  glm::mat4& ProjectionMat = CommancheRenderer::Instance->camX.ProjectionMat;

    float left = (-1.0f - ProjectionMat[3][0]) / ProjectionMat[0][0];
    float right = (1.0f - ProjectionMat[3][0]) / ProjectionMat[0][0];
    float bottom = (-1.0f - ProjectionMat[3][1]) / ProjectionMat[1][1];
    float top = (1.0f - ProjectionMat[3][1]) / ProjectionMat[1][1];

    left *= 2;
    right *= 2;
    bottom *= 2;
    top *= 2;

    glm::vec2 screenSize;
#if EDITOR
    screenSize = Editor::Instance->GetViewPortSize();
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

    CoordinateCalculator::ConvertPixelsToMeters(worldX, worldY);
    return glm::vec2(worldX, worldY);
}

bool Cursor::HasLeftCursorClicked() {
    return glfwGetMouseButton(wnd, 0);
}

bool Cursor::HasRightCursorClicked() {
    return glfwGetMouseButton(wnd, 1);
}
