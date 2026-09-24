#pragma once

#include "geometry.hpp"
#include "types.hpp"

#include <GLFW/glfw3.h>

enum WindowProperties {
  HEIGHT = 600,
  WIDTH  = 800
};

enum TriangleTestProperties {
  TRIANGLES_AMOUNT = 20,
  RADIUS_1 = 9,
  RADIUS_2 = 6,
  RADIUS_3 = 3,
  OFFSET_1 = 10,
  OFFSET_2 = -10,
  DEG_ROTATION_1 = 45,
  DEG_ROTATION_2 = 90
};

struct VertexInfo {
  uint vbo = 0; // vertex buffer object - colors and position
  uint vao = 0; // vertex array  object - states for the pipeline
};

GLFWwindow *create_window(void);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void generate_triangle_test(Triangle *triangles);
VertexInfo get_buffers();
void clear_window(void);
void terminate_processes(VertexInfo &vobjs);
