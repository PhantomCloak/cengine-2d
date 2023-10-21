#include "../../keyboard.h"
#include "../../../render/render.h"
#include <iostream>
#include <GLFW/glfw3.h>
#include <unordered_map>

std::unordered_map<int, bool> keyHoldMap;

void Keyboard::Setup() {
}

bool Keyboard::IsKeyPressed(int keyCode) {
  return glfwGetKey((GLFWwindow*)CommancheRenderer::Instance->wnd, keyCode);
}

void Keyboard::Poll() {
  glfwPollEvents();
}

bool Keyboard::IsKeyPressing(int keyCode) {
  return glfwGetKey((GLFWwindow*)CommancheRenderer::Instance->wnd, keyCode);
}

void Keyboard::FlushPressedKeys() {
}
