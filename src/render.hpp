#ifndef RENDER_HPP
#define RENDER_HPP

enum WindowProperties {
    HEIGHT = 600,
    WIDTH  = 800
};

GLFWwindow *create_window(void);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);

#endif