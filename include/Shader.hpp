#pragma once

#include "glad.hpp"
#include <cassert>

namespace render {

class Shader {
    private:
    GLuint id = 0;

    public:
    Shader(const char *vertex_path, const char *fragment_path);
    Shader(const Shader &rhs) = delete;
    Shader &operator=(const Shader &rhs) = delete;
    Shader(Shader &&rhs) noexcept;
    Shader &operator=(Shader &&rhs) noexcept; 
    ~Shader(); 

    inline void use() const {
        glUseProgram(id);
    }

    inline GLint get_uniform_location(const char *name) const {
        assert(name);
        return glGetUniformLocation(id, name);
    }

    private:
    void delete_shader();
};

}
