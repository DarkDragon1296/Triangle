#include "glad.hpp"
#include "render.hpp"
#include "shader.hpp"
#include "camera.hpp"
#include "test_generator.hpp"

#include <glm/ext.hpp>
#include <iostream>
#include <GLFW/glfw3.h>

namespace TOP_LEVEL_NAMESPACE {

GLFWwindow *create_window(void) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT, "triangles",
    NULL, NULL);

    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
        std::cout << "Failed to initialize GLAD" << std::endl;

    return window;
}

RenderObjects get_objects(void) {
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

#if 0
    generate_triangle_field(objs.triangles);
#endif

#if 1
    generate_triangle_test(objs.triangles);
#endif

#if 0
    objs.triangles.push_back(tr1);
    objs.triangles.push_back(tr2);
#endif

    sort_triangles(objs.triangles);

    for (size_t i = 0; i < objs.triangles.size(); i++) {
        for (size_t j = i + 1; j < objs.triangles.size(); j++) {

            if (get_max_z_tr_pos(objs.triangles[i]) <
                get_min_z_tr_pos(objs.triangles[j])) {
                    break;
            }

            get_intersection_points(objs.triangles[i], objs.triangles[j], objs);
        }
    }

    return objs;
}

VertexObjects get_vertex_objects(const RenderObjects &objs) {
  VertexObjects vobjs{};

    vobjs.segments = get_buffers(SEGMENT, objs.segments.size()
                                          * sizeof(Segment3D),
                                          objs.segments.data());

    vobjs.triangles = get_buffers(TRIANGLE, objs.triangles.size()
                                           * sizeof(Triangle),
                                           objs.triangles.data());

  return vobjs;
}

VertexInfo get_buffers([[maybe_unused]] int dots_amount, // TODO: (*) actually unused
                       size_t size, const void *data) {
  VertexInfo vobjs{};

  glGenVertexArrays(1, &vobjs.vao);
  glBindVertexArray(vobjs.vao);

  glGenBuffers(1, &vobjs.vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vobjs.vbo);

  glBufferData(GL_ARRAY_BUFFER, (GLsizeiptr)size, data, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                        3 * sizeof(float), NULL);
  glEnableVertexAttribArray(0);

  return vobjs;
}

void draw_objects(const RenderObjects &objs, const VertexObjects &vobjs,
                  const Shader &shader) {
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0f, 8.0f);
    glUniform1i(glGetUniformLocation(shader.id, "isIntersection"), true);
    glBindVertexArray(vobjs.segments.vao);
    glDrawArrays(GL_LINES, 0, 2 * (GLsizei)objs.segments.size());

    glDisable(GL_POLYGON_OFFSET_FILL);
    glUniform1i(glGetUniformLocation(shader.id, "isIntersection"), false);
    glBindVertexArray(vobjs.triangles.vao);
    glDrawArrays(GL_TRIANGLES, 0, 3 * (GLsizei)objs.triangles.size());
}

void clear_window(void) {
  glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void terminate_processes(VertexInfo& vobjs) {
  glDeleteVertexArrays(1, &vobjs.vao);
  glDeleteBuffers(1, &vobjs.vbo);
}

void framebuffer_size_callback([[maybe_unused]] GLFWwindow *window, 
                               int width, int height) {
  glViewport(0, 0, width, height); // TODO: (*) window resize fix
}

void setup_render(void) {
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LESS);
  glLineWidth(10.0f);
  glEnable(GL_LINE_SMOOTH);
}

void process_transforms(Shader &shader, Camera cam) {
    shader.use();

    glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                            (float)WIDTH / (float)HEIGHT,
                                            0.1f, 100.0f);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    int model_loc = glGetUniformLocation(shader.id, "model");
    int view_loc = glGetUniformLocation(shader.id, "view");
    glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(view_loc, 1, GL_FALSE, &cam.view[0][0]);

    shader.set_mat4("projection", projection);
}
}
