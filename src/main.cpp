// TODO Добавить проверки на шейдеры
// TODO Возможность менять позицию камеры
// TODO Управление камерой
// TODO Вынести работу с vao, vbo и ebo в отдельный файл
// TODO относительные инклюды -- минимизировать
// TODO make run
// TODO фикс относительных путей директорий

#include "../include/glad/glad.h"
#include "../include/glm/ext.hpp"
//#include "../include/glm/matrix_transform.hpp"
//#include "../include/glm/type_ptr.hpp"
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

    int wight = 800, height = 600;


    glm::ortho(0.0f, 800.0f, 0.0f, 600.0f, 0.1f, 100.0f);
    glm::mat4 proj = glm::perspective(glm::radians(45.0f), (float)wight/(float)height, 0.1f, 100.0f);

/*
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

    glm::mat4 projection;
    projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);


    glm::vec3 camera_pos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 camera_target = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 camera_direction = glm::normalize(camera_pos - camera_target);

    glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 camera_right = glm::normalize(glm::cross(up, camera_direction));

    glm::vec3 camera_up = glm::cross(camera_direction, camera_right);
*/
    //glm::mat4 view; 


    glm::mat4 projection = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f, 100.0f);

    while (!glfwWindowShouldClose(window)) {
        process_input(window);

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        our_shader.use();

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view  = glm::mat4(1.0f);

        float radius = 3.0f;
        float cam_x = static_cast<float>(sin(glfwGetTime()) * 1.25f * radius);
        float cam_z = static_cast<float>(cos(glfwGetTime()) * radius);

        model = glm::rotate(model, glm::radians(0.0f), glm::vec3(1.0f, 1.0f, 0.0f));
        view = glm::lookAt(glm::vec3(cam_x, 0.0f, cam_z), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        //view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        int model_loc = glGetUniformLocation(our_shader.id, "model");
        int view_loc = glGetUniformLocation(our_shader.id, "view");
        glUniformMatrix4fv(model_loc, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(view_loc, 1, GL_FALSE, &view[0][0]);

        our_shader.set_mat4("projection", projection);

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