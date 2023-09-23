#ifndef CURSOR_H
#define CURSOR_H

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Cursor {
public:
    static void Setup(void* window);
    static glm::vec2 GetCursorPosition();
    static glm::vec2 GetCursorWorldPosition(glm::vec2 screenPoint);
                                
private:
    static GLFWwindow* wnd;
};

#endif

