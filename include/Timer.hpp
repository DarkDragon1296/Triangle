#pragma once

#include <GLFW/glfw3.h>

namespace utils {

class Timer {
    public:

    double start = 0.0f;
    double end   = 0.0f;
    double delta = 0.0f;

    inline void get_start(void) {
        start = glfwGetTime();
    }

    inline void get_end(void) {
        end = glfwGetTime();
    }

    inline void get_delta(void) {
        delta = end - start;
    }
};

}
