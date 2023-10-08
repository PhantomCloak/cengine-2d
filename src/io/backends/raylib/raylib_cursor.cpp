#include "../../cursor.h"
#include "../../../core/coordinate_system.h"
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

glm::vec2 Cursor::GetCursorWorldPosition(glm::vec2 screenPoint, CommancheCamera cam) {
    auto mPos = GetScreenToWorld2D(Vector2{ screenPoint.x, screenPoint.y }, cam);

    CoordinateCalculator::ConvertPixelsToMeters(mPos.x, mPos.y);
    return glm::vec2(mPos.x, mPos.y);
}

bool Cursor::HasLeftCursorClicked() {
    return IsMouseButtonPressed(0);
}

bool Cursor::HasRightCursorClicked() {
    return IsMouseButtonPressed(1);
}
