#pragma once

#include "glad.h"
#include "types.h"

#include <glm/fwd.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <string>

class Shader {
  public:
  uint id = 0;

  Shader(const char *vertex_path, const char *fragment_path);

  inline void use() {
    glUseProgram(id);
  }

  inline void set_int(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(id, name.c_str()), value);
  }
  inline void set_bool(const std::string &name, bool value) const {
    set_int(name, (int)value);
  }
  inline void set_float(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(id, name.c_str()), value);
  }
  inline void set_mat4(const std::string &name, glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1,
                       GL_FALSE, &mat[0][0]);
  }
};

//uint create_shader(void);
//uint get_shader(const int shader_type);
