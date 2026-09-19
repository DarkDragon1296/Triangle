#include "../include/glad/glad.h"
#include "../include/glm/ext.hpp"
#include <iostream>
#include <GLFW/glfw3.h>
#include "render.hpp"
#include "shader.hpp"

GLFWwindow *create_window(void) {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 

    GLFWwindow *window = glfwCreateWindow(WIDTH, HEIGHT,
                                          "Triangles",
                                          NULL, NULL);

    if (!window) {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
    }

    glEnable(GL_DEPTH_TEST);

    return window;
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void generate_triangle_test(struct triangle *triangles) {
    float deg_angle[3];
    float rad_angle[3];

    for (int i = 0; i < TRIANGLES_AMOUNT; i++) {
        deg_angle[0] = (float)i / TRIANGLES_AMOUNT * 360.0f + DEG_ROTATION_1; 
        deg_angle[1] = (float)i / TRIANGLES_AMOUNT * 360.0f + DEG_ROTATION_2; 
        deg_angle[2] = (float)i / TRIANGLES_AMOUNT * 360.0f; 

        rad_angle[0] = glm::radians(deg_angle[0]); 
        rad_angle[1] = glm::radians(deg_angle[1]); 
        rad_angle[2] = glm::radians(deg_angle[2]); 

        triangles[i].dots[0][0] = RADIUS_1 * glm::cos(rad_angle[0]);
        triangles[i].dots[0][1] = RADIUS_1 * glm::sin(rad_angle[0]);
        triangles[i].dots[0][2] = OFFSET_1;

        triangles[i].dots[1][0] = RADIUS_2 * glm::cos(rad_angle[1]);
        triangles[i].dots[1][1] = RADIUS_2 * glm::sin(rad_angle[1]);
        triangles[i].dots[1][2] = OFFSET_2;

        triangles[i].dots[2][0] = RADIUS_3 * glm::cos(rad_angle[2]);
        triangles[i].dots[2][1] = RADIUS_3 * glm::sin(rad_angle[2]);
        triangles[i].dots[2][2] = 0.0f;
    }
}

void get_buffers(unsigned int &vbo, unsigned int &vao,
                 struct triangle *triangles,
                 size_t triangles_size) {

    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);

    glBufferData(GL_ARRAY_BUFFER, triangles_size, triangles, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE,
                          3 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
}