#pragma once

#include "Shader.hpp"
#include "camera.hpp"

#include <GLFW/glfw3.h>

namespace TOP_LEVEL_NAMESPACE {

enum WindowProperties {
  HEIGHT = 600,
  WIDTH  = 800
};

void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void setup_render(void);
void process_transforms(Shader &shader, Camera &cam);
void clear_window(void);

}
