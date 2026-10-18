#include "glad.hpp"
#include "render.hpp"
#include "Renderer.hpp"
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



void clear_window(void) {
  glClearColor(0.2f, 0.3f, 0.4f, 1.0f);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void framebuffer_size_callback([[maybe_unused]] GLFWwindow *window, 
                               int width, int height) {
  glViewport(0, 0, width, height); // TODO: (*) window resize fix
}

void Renderer::draw() const {
    glEnable(GL_POLYGON_OFFSET_FILL);
    glPolygonOffset(1.0f, 8.0f);
    glUniform1i(glGetUniformLocation(shader.id, "isIntersection"), true);
    segments.draw();

    glDisable(GL_POLYGON_OFFSET_FILL);
    glUniform1i(glGetUniformLocation(shader.id, "isIntersection"), false);
    triangles.draw();
}

void process_transforms(Shader &shader, Camera &cam) {
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
