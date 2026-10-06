#include "control.hpp"

#include <GLFW/glfw3.h>

namespace TOP_LEVEL_NAMESPACE {

void process_input(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}

}
