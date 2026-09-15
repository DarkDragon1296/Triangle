#include "../include/glad/glad.h"
#include <GLFW/glfw3.h>
#include "shader.hpp"

const char *fragment_shader_source =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "void main()\n"
    "{\n"
    "    FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
    "}\n\0";

const char *vertex_shader_source =
    "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "}\0";

unsigned int create_shader(void) {
    unsigned int shader_program  = glCreateProgram(),
                 fragment_shader = get_shader(FRAGMENT),
                 vertex_shader   = get_shader(VERTEX);

    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    glDeleteShader(vertex_shader); 
    glDeleteShader(fragment_shader);

    return shader_program;
}

unsigned int get_shader(const int shader_type) {
    if (shader_type == FRAGMENT) {
        unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment_shader, 1, &fragment_shader_source, NULL);
        glCompileShader(GL_FRAGMENT_SHADER);

        return fragment_shader;
    } else if (shader_type == VERTEX) {
        unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex_shader, 1, &vertex_shader_source, NULL);
        glCompileShader(GL_VERTEX_SHADER);

        return vertex_shader;
    }
  
    return 0;    
}