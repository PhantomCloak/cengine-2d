#include "cursor.h"
#include <GLFW/glfw3.h>

GLFWwindow* wnd;
void Cursor::Setup(void* window) {
    wnd = (GLFWwindow*)window;
}

void Cursor::GetCursorPosition(double* x, double* y) {
    glfwGetCursorPos(wnd, x, y);
}
