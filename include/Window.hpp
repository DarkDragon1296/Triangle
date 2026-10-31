#pragma once

#include <GLFW/glfw3.h>
#include <memory>
#include <optional>

namespace render {

enum WindowProperties {
  HEIGHT = 600,
  WIDTH  = 800
};

class Window {
    private:
    struct WindowDeleter {
        void operator()(GLFWwindow *ptr) {
            if (ptr)
                glfwDestroyWindow(ptr);
        }
    };
    std::unique_ptr<GLFWwindow, WindowDeleter> handle; 

    public:
    /// Also calls glfwMakeContextCurrent 
    /// on the handle if successfully inited 
    static std::optional<Window> create(int width, int height, const char *name);

    /// I trust that the programmer will use this function responsibly
    inline GLFWwindow *get() {
        return handle.get();
    }

    private:
    Window(int width, int height, const char *name);
};

}
