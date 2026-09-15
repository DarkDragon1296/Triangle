#ifndef SHADER_HPP
#define SHADER_HPP

enum Shaders {
    FRAGMENT,
    VERTEX,
};

unsigned int create_shader(void);
unsigned int get_shader(const int shader_type);

#endif