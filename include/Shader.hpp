#pragma once

#include "glad.hpp"
#include "utils.hpp"

#include <glm/fwd.hpp>
#include <glm/detail/type_mat4x4.hpp>

namespace TOP_LEVEL_NAMESPACE {

class Shader {
    private:
    GLuint id = 0;

    public:
    Shader(const char *vertex_path, const char *fragment_path);
    Shader(const Shader &rhs) = delete;
    Shader &operator=(const Shader &rhs) = delete;
    Shader(Shader &&rhs);
    Shader &operator=(Shader &&rhs); 
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
