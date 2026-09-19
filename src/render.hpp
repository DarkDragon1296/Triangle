#ifndef RENDER_HPP
#define RENDER_HPP

enum WindowProperties {
    HEIGHT = 600,
    WIDTH  = 800
};

enum TriangleTestProperties {
    TRIANGLES_AMOUNT = 20,
    RADIUS_1 = 9,
    RADIUS_2 = 6,
    RADIUS_3 = 3,
    OFFSET_1 = 10,
    OFFSET_2 = -10,
    DEG_ROTATION_1 = 45,
    DEG_ROTATION_2 = 90
};

struct triangle {
    glm::vec3 dots[3];
};

GLFWwindow *create_window(void);
void framebuffer_size_callback(GLFWwindow *window, int width, int height);
void generate_triangle_test(struct triangle *triangles);
void get_buffers(unsigned int &vbo, unsigned int &vao);
void clear_window(void);
void terminate_processes(unsigned int &vbo, unsigned int &vao);

#endif