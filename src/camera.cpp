#include "camera.hpp"

#include <GLFW/glfw3.h>

namespace TOP_LEVEL_NAMESPACE {

void process_input(GLFWwindow *window, Camera &cam) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);

  glm::vec3 mv_dir(0.0f);

  if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
    mv_dir += cam.dir;
  if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
    mv_dir -= cam.dir;
  if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
    mv_dir += cam.right;
  if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    mv_dir -= cam.right;
  if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
    mv_dir += cam.abs_up;
  if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
    mv_dir -= cam.abs_up;

  
  if (glm::length(mv_dir) > 0.001f)
    cam.mv(cam.speed * glm::normalize(mv_dir)); // TODO: (*) add multiplier (frame_time / 1s)
}

}