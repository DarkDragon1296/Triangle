#include "Window.hpp"

namespace {
void size_callback([[maybe_unused]] GLFWwindow *window, 
                   int width, int height) {
    glViewport(0, 0, width, height); // TODO: (*) window resize fix
}
}

namespace render {

std::optional<Window> Window::create(int width, int height, const char *name) {
    Window window = Window(width, height, name);
    if (!window.handle) 
        return std::nullopt;

    glfwMakeContextCurrent(window.handle);
    glfwSetFramebufferSizeCallback(window.handle, size_callback);

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

}
