#pragma once

#include <GLFW/glfw3.h>
#include <optional>

namespace render {

enum WindowProperties {
  HEIGHT = 600,
  WIDTH  = 800
};

class Window {
    private:
    GLFWwindow *handle = nullptr;

    public:
    /// Also calls glfwMakeContextCurrent 
    /// on the handle if successfully inited 
    static std::optional<Window> create(int width, int height, const char *name);

    Window(const Window &rhs) = delete;
    Window &operator=(const Window &rhs) = delete;
    Window(Window &&rhs) noexcept;
    Window &operator=(Window &&rhs) noexcept; 
    ~Window(); 

    /// I trust that the programmer will use this function responsibly
    inline GLFWwindow *get() {
        return handle;
    }

    private:
    Window(int width, int height, const char *name);
    void delete_window();
};

}
