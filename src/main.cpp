// TODO: Управление камерой
// TODO: тесты на винде
// TODO: написать README
// TODO: добавить константность параметров там, где необходимо
// TODO: Camera class

// TODO: rename namespace

#include "glad.hpp"
#include "shader.hpp"
#include "render.hpp"
#include "camera.hpp"

#include <glm/ext.hpp>
#include <GLFW/glfw3.h>
#include <iostream>

const char *vshader_path = "src/shaders/vertex_shader.vs";
const char *fshader_path = "src/shaders/fragment_shader.fs";

using namespace TOP_LEVEL_NAMESPACE;

int main(void) {
  GLFWwindow *window = create_window();
  Shader shader = Shader(vshader_path, fshader_path);
  RenderObjects objs  = get_objects();
  VertexObjects vobjs = get_vertex_objects(objs);
  Camera camera;

  setup_render();

while (!glfwWindowShouldClose(window)) {
    process_input(window, camera);
    clear_window();
    shader.use();

    glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                            (float)WIDTH / (float)HEIGHT,
                                            0.1f, 100.0f);
    glm::mat4 model = glm::mat4(1.0f);

    model = glm::rotate(model, glm::radians(0.0f),
                        glm::vec3(1.0f, 1.0f, 0.0f));

    int model_loc = glGetUniformLocation(shader.id, "model");
    int view_loc = glGetUniformLocation(shader.id, "view");
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, &camera.view[0][0]);

    shader.set_mat4("projection", projection);

    draw_objects(objs, vobjs, shader);

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  terminate_processes(vobjs.triangles);
  terminate_processes(vobjs.segments);

  return 0;
}