#include "../../cursor.h"
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

}

bool Cursor::HasLeftCursorClicked() {
    return IsMouseButtonPressed(0);
}

bool Cursor::HasRightCursorClicked() {
    return IsMouseButtonPressed(1);
}
