#include "Window.hpp"

namespace {
void size_callback([[maybe_unused]] GLFWwindow *window, 
                   int width, int height) {
    glViewport(0, 0, width, height); // TODO: (*) window resize fix
}
}

namespace render {

std::optional<Window> Window::create(int width, int height, const char *name) {
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    Window window = Window(width, height, name);
    if (!window.handle.get()) 
        return std::nullopt;

    glfwMakeContextCurrent(window.handle.get());
    glfwSetFramebufferSizeCallback(window.handle.get(), size_callback);

    return window;
}

Window::Window(int width, int height, const char *name) 
    : handle{glfwCreateWindow(width, height, name, nullptr, nullptr)} {}

}
