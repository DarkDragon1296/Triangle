#pragma once

#include "geometry.hpp"
#include "shader.hpp"
#include "types.hpp"

#include <GLFW/glfw3.h>

namespace TOP_LEVEL_NAMESPACE {

enum WindowProperties {
  HEIGHT = 600,
  WIDTH  = 800
};

// TODO: Перенести это куда-то
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
  uint vbo = 0;
  uint vao = 0;
};

struct VertexObjects {
  VertexInfo triangles;
  VertexInfo segments;
};

GLFWwindow *create_window(void);
RenderObjects get_objects(void);
VertexObjects get_vertex_objects(const RenderObjects &objs);
VertexInfo get_buffers(int dots_amount, size_t size, const void *data);
void generate_triangle_test(Triangle *triangles);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void draw_objects(const RenderObjects &objs, const VertexObjects &vobjs,
                  const Shader &shader);
void setup_render(void);
void clear_window(void);
void terminate_processes(VertexInfo &vobjs);

}
