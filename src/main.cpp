// TODO Код шейдеров поместить в отдельную директорию + прога по чтению кода
// TODO Добавить проверки на шейдеры
// TODO Возможность менять позицию камеры
// TODO Управление камерой
// TODO Вынести работу с vao, vbo и ebo в отдельный файл
// TODO относительные инклюды -- минимизировать

#include "../include/glad/glad.h"
#include "../include/glm/glm.hpp"
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader.hpp"

void init_glfw_environment() {
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
}

void framebuffer_size_callback(GLFWwindow *window, int width, int height) {
    glViewport(0, 0, width, height);
}

void process_input(GLFWwindow *window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

int main(void) {
    init_glfw_environment();

    GLFWwindow *window = glfwCreateWindow(800, 600, "Triangles", NULL, NULL);
    
    if (!window) {
        std::cout << "FAILED to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    shader our_shader("src/shaders/vertex_shader.vs",
                      "src/shaders/fragment_shader.fs");

    float vertices[] = {
        /*    POSITION    */  /*     COLOR     */
         0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,
        -0.5f, -0.5f,  0.0f,  0.0f,  1.0f,  0.0f,
         0.0f,  0.5f,  0.0f,  0.0f,  0.0f,  1.0f
    };
    unsigned int vbo, vao;
    
    glGenVertexArrays(1, &vao);
    glGenBuffers(1, &vbo);
    
    glBindVertexArray(vao);

    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    // ^^^ Render ^^^

    glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 camera_target = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 camera_direction = glm::normalize(camera_pos - camera_target);

    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 camera_right = glm::normalize(glm::cross(up, camera_direction));

    glm::vec3 camera_up = glm::cross(camera_direction, camera_right);

    glm::mat4 view; 

    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        our_shader.use();
        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(1, &vbo);
    glfwTerminate();

    return 0;
}