#include "glad.hpp"
#include "render.hpp"
#include "Renderer.hpp"
#include "Shader.hpp"
#include "camera.hpp"
#include "Window.hpp"
#include "test_generator.hpp"

#include <glm/ext.hpp>
#include <iostream>
#include <GLFW/glfw3.h>

namespace TOP_LEVEL_NAMESPACE {

std::optional<Window> Window::create(int width, int height, const char *name) {
    Window window = Window(width, height, name);
    if (!window.handle) 
        return std::nullopt;

    glfwMakeContextCurrent(window.handle);
    glfwSetFramebufferSizeCallback(window.handle, framebuffer_size_callback);

    return window;
}

Window::Window(int width, int height, const char *name) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    handle = glfwCreateWindow(width, height, name, nullptr, nullptr);
}

Window::~Window() {
    delete_window();
}

Window::Window(Window &&rhs) noexcept : handle{rhs.handle} {
    rhs.handle = nullptr;
}

Window &Window::operator=(Window &&rhs) noexcept {
    if (this == &rhs)
        return *this;

    delete_window();
    handle = rhs.handle;
    rhs.handle = nullptr;

    return *this;
}

void Window::delete_window() {
    if (handle)
        glfwDestroyWindow(handle);
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
    glUniform1i(shader.get_uniform_location("isIntersection"), true);
    segments.draw();

    glDisable(GL_POLYGON_OFFSET_FILL);
    glUniform1i(shader.get_uniform_location("isIntersection"), false);
    triangles.draw();
}

void Renderer::configure_gl_state() const {
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glLineWidth(10.0f);
    glEnable(GL_LINE_SMOOTH);
}

void process_transforms(Shader &shader, Camera &cam) {
    shader.use();

    glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                            (float)WIDTH / (float)HEIGHT,
                                            0.1f, 100.0f);

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    glUniformMatrix4fv(shader.get_uniform_location("model"), 
                       1, GL_FALSE, glm::value_ptr(model));
    glUniformMatrix4fv(shader.get_uniform_location("view"), 
                       1, GL_FALSE, glm::value_ptr(cam.view));
    glUniformMatrix4fv(shader.get_uniform_location("projection"), 
                       1, GL_FALSE, glm::value_ptr(projection));
}

}
