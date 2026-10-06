#pragma once

#include "utils.hpp"

#include <GLFW/glfw3.h>
#include <glm/ext.hpp>

namespace TOP_LEVEL_NAMESPACE {

#if 0

class Camera {
    public:
    Camera() :   dir(glm::normalize(pos - target)),
               right(glm::normalize(glm::cross(abs_up, dir))),
                  up(glm::normalize(glm::cross(dir, right))).
                view(glm::lookAt(pos, pos + front, up)) {}

    glm::vec3 pos{0.0f};

    private:
    glm::vec3 target{0.0f, 0.0f, -1.0f};
    glm::vec3 abs_up{0.0f, 1.0f, 0.0f};
    glm::vec3 dir;
    glm::vec3 right;
    glm::vec3 up;
    glm::mat4 view;
};

#endif

void process_input(GLFWwindow *window);

}

