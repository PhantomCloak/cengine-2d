#include "keyboard.h"
#include "GLFW/glfw3.h"
#include <iostream>
#include <unordered_map>

static GLFWwindow* k_window;
std::unordered_map<int, bool> keyHoldMap;

void Keyboard::Setup(void* windowPtr) {
    k_window = (GLFWwindow*)windowPtr;
}

bool Keyboard::IsKeyPressed(int keyCode) {
    bool keyPressNow = glfwGetKey(k_window, keyCode) == GLFW_PRESS;

    // if key pressed in last request

    if (keyPressNow) {
        if (keyHoldMap.find(keyCode) != keyHoldMap.end()) {
            return false;
        }else{
          keyHoldMap.insert(std::make_pair(keyCode, true));
          return true;
        }
    } else {
        if (keyHoldMap.find(keyCode) != keyHoldMap.end())
            keyHoldMap.erase(keyHoldMap.find(keyCode));
        return false;
    }

    return true;
}

void Keyboard::Poll() {
    glfwPollEvents();
}

bool Keyboard::IsKeyPressing(int keyCode) {
    return glfwGetKey(k_window, keyCode) == GLFW_PRESS;
}

void Keyboard::FlushPressedKeys() {
}
