/* NO AI PROJECT */

// TODO: Добавить проверки на шейдеры
// TODO: Управление камерой
// TODO: тесты на винде
// TODO: написать README
// TODO: добавить константность параметров там, где необходимо
// TODO: рассмотреть случай, если какой-нибудь нехороший человек передаст точки
//       на одной прямой вместо треугольника

#include "glad.hpp"
#include "shader.hpp"
#include "render.hpp"

#include <glm/ext.hpp>
#include <GLFW/glfw3.h>
#include <iostream>

const char *vshader_path = "src/shaders/vertex_shader.vs";
const char *fshader_path = "src/shaders/fragment_shader.fs";

void process_input(GLFWwindow *window);

int main(void) {
  GLFWwindow *window = create_window();

  Shader our_shader = Shader(vshader_path, fshader_path);

  RenderObjects objs{};

  Triangle tr1 = {{
    {0.0f, 0.0f, 0.0f},
    {2.0f, 2.0f, 0.0f},
    {0.0f, 2.0f, 0.0f}
  }};
  Triangle tr2 = {{
    {0.0f, 0.0f,  1.0f},
    {2.0f, 2.0f,  0.0f},
    {0.0f, 2.0f, -1.0f}
  }};

  objs.triangles.push_back(tr1);
  objs.triangles.push_back(tr2);

  get_intersection_points(tr1, tr2, objs);

  VertexInfo vobjs_segments = get_buffers(SEGMENT, objs.segments.size()
                                          * sizeof(Segment3D),
                                          objs.segments.data());

  VertexInfo vobjs_triangles = get_buffers(TRIANGLE, objs.triangles.size()
                                           * sizeof(Triangle),
                                           objs.triangles.data());

  glLineWidth(10.0f);

  while (!glfwWindowShouldClose(window)) {
    process_input(window);
    clear_window();

    our_shader.use();

    float radius = 10.0f;
    float cam_x = static_cast<float>(sin(glfwGetTime()) * 1.25f * radius);
    float cam_z = static_cast<float>(cos(glfwGetTime()) * radius);

    // start of create_transformations(camera) --> void
    glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                            (float)WIDTH / (float)HEIGHT,
                                            0.1f, 100.0f);
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view  = glm::mat4(1.0f);

    model = glm::rotate(model, glm::radians(0.0f),
                        glm::vec3(1.0f, 1.0f, 0.0f));
    view = glm::lookAt(glm::vec3(cam_x, 0.0f, cam_z),
                       glm::vec3(0.0f , 0.0f, 0.0f ),
                       glm::vec3(0.0f , 1.0f, 0.0f ));

    int model_loc = glGetUniformLocation(our_shader.id, "model");
    int view_loc = glGetUniformLocation(our_shader.id, "view");
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, &view[0][0]);

    our_shader.set_mat4("projection", projection);
    // end

    glUniform1i(glGetUniformLocation(our_shader.id, "isIntersection"), true);
    glBindVertexArray(vobjs_segments.vao);
    glDrawArrays(GL_LINES, 0, 2 * (GLsizei)objs.segments.size());

    glUniform1i(glGetUniformLocation(our_shader.id, "isIntersection"), false);
    glBindVertexArray(vobjs_triangles.vao);
    glDrawArrays(GL_TRIANGLES, 0, 3 * (GLsizei)objs.triangles.size());


    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  terminate_processes(vobjs_triangles);
  terminate_processes(vobjs_segments);

  return 0;
}

void process_input(GLFWwindow *window) {
  if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    glfwSetWindowShouldClose(window, true);
}