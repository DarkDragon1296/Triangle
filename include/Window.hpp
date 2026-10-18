#pragma once

#include <GLFW/glfw3.h>
#include <optional>

namespace TOP_LEVEL_NAMESPACE {

class Window {
    private:
    GLFWwindow *handle = nullptr;

    public:
    static std::optional<Window> create(int width, int height, const char *name);
    Window(const Window &rhs) = delete;
    Window &operator=(const Window &rhs) = delete;
    Window(Window &&rhs);
    Window &operator=(Window &&rhs); 
    ~Window(); 

    private:
    Window(int width, int height, const char *name);
    void delete_window();
};

}
