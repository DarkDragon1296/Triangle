#pragma once

#include "geometry.hpp"
#include "shader.hpp"
#include "types.hpp"
#include "camera.hpp"

#include <GLFW/glfw3.h>

namespace TOP_LEVEL_NAMESPACE {

enum WindowProperties {
  HEIGHT = 600,
  WIDTH  = 800
};

GLFWwindow *create_window(void);
RenderObjects get_objects(void);
VertexObjects get_vertex_objects(const RenderObjects &objs);
VertexInfo get_buffers(int dots_amount, size_t size, const void *data);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void draw_objects(const RenderObjects &objs, const VertexObjects &vobjs,
                  const Shader &shader);
void setup_render(void);
void process_transforms(Shader &shader, Camera cam);
void clear_window(void);
void terminate_processes(VertexInfo &vobjs);

}
