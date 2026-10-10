#pragma once

#include "utils.hpp"

#include <glm/ext.hpp>
#include <GLFW/glfw3.h>

namespace TOP_LEVEL_NAMESPACE {

class Camera {
    public:
    Camera() :   dir(glm::normalize(target - pos)),
               right(glm::normalize(glm::cross(dir, abs_up))),
                  up(glm::normalize(glm::cross(right, dir))),
                view(glm::lookAt(pos, pos + dir, up)) {}

    float speed{0.1f};
    float yaw  {0.0f};
    float yaw_speed{0.5f};
    float pitch{0.0f};
    float pitch_speed{0.5f};

    glm::vec3 abs_up{0.0f, 1.0f,   0.0f};
    glm::vec3 target{0.0f, 0.0f,   1.0f}; // TODO: del
    glm::vec3 pos   {0.0f, 0.0f, -10.0f};
    glm::vec3 dir;
    glm::vec3 right;
    glm::vec3 up;
    glm::mat4 view;

    inline void mv(glm::vec3 mv) {
        pos += mv;
        view = glm::lookAt(pos, pos + dir, up); 
    }

    inline void tp(glm::vec3 tp_pos) {
        pos = tp_pos;
        view = glm::lookAt(pos, pos + dir, up); 
    }

    inline void rot(float yaw_deg, float pitch_deg) {
        yaw   += yaw_deg;
        pitch += pitch_deg;

        if (pitch > 89.0f) {
            pitch = 89.0f;
        } else if (pitch < -89.0f) {
            pitch = -89.0f;
        }

        dir[0] = float(-sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
        dir[1] = float( sin(glm::radians(pitch)));
        dir[2] = float( cos(glm::radians(yaw)) * cos(glm::radians(pitch)));

        right = glm::normalize(glm::cross(dir, abs_up));
        up    = glm::normalize(glm::cross(right, dir));
        view = glm::lookAt(pos, pos + dir, up); 
    }
};

void process_input(GLFWwindow *window, Camera &cam);

}