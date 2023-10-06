#include "keyboard.h"
#include "GLFW/glfw3.h"
#include "raylib.h"
#include <iostream>
#include <unordered_map>

std::unordered_map<int, bool> keyHoldMap;

void Keyboard::Setup() {
}

bool Keyboard::IsKeyPressed(int keyCode) {
  return IsKeyDown(keyCode);
    }

void Keyboard::Poll() {
    glfwPollEvents();
}

bool Keyboard::IsKeyPressing(int keyCode) {
  return IsKeyDown(keyCode);
}

void Keyboard::FlushPressedKeys() {
}
