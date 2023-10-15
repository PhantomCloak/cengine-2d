#ifndef CURSOR_H
#define CURSOR_H

#include "../render/render_primitives.h"
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

class Cursor {
public:
    static void Setup(void* window);
    static glm::vec2 GetCursorPosition();
    static glm::vec2 GetCursorWorldPosition(glm::vec2 screenPoint, CommancheCamera cam);
    static bool HasLeftCursorClicked();
    static bool HasRightCursorClicked();
                                
private:
    static GLFWwindow* wnd;
};

#endif

