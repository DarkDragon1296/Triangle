#pragma once

#include "glad.hpp"
#include "types.hpp"
#include "utils.hpp"

#include <glm/fwd.hpp>
#include <glm/detail/type_mat4x4.hpp>
#include <string>

namespace TOP_LEVEL_NAMESPACE {

class Shader {
  public:
  uint id = 0;

  Shader(const char *vertex_path, const char *fragment_path);

  inline void use() {
    glUseProgram(id);
  }

  inline void set_mat4(const std::string &name, glm::mat4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(id, name.c_str()), 1,
                       GL_FALSE, &mat[0][0]);
  }
};

}
