#include "common.h"
#include <chrono>
#include <thread>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

double getTime() {
  return glfwGetTime() * 1000;
}

void sleepProgram(long amount){
  std::this_thread::sleep_for(std::chrono::milliseconds(amount));
}
