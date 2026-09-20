#include <glm/ext.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "geometry.hpp"

int intersect_dim(struct triangle &tr1, struct triangle &tr2) {
    glm::vec3 a_tr1 = tr1.dots[2] - tr1.dots[0];
    glm::vec3 b_tr1 = tr1.dots[2] - tr1.dots[1];
    glm::vec3 a_tr2 = tr2.dots[2] - tr2.dots[0];
    glm::vec3 b_tr2 = tr2.dots[2] - tr2.dots[1];

    glm::vec3 n_tr1 = glm::cross(a_tr1, b_tr1);
    glm::vec3 n_tr2 = glm::cross(a_tr2, b_tr2);

    float cross_res = glm::length(glm::cross(n_tr1, n_tr2));

    if (fabsf(cross_res) > 0.001f)
        return 1;

    glm::vec3 pl1_offset = tr1.dots[0] - glm::dot(tr1.dots[0], a_tr1) * a_tr1
                                       - glm::dot(tr1.dots[0], b_tr1) * b_tr1;

    glm::vec3 pl2_offset = tr2.dots[0] - glm::dot(tr2.dots[0], a_tr2) * a_tr2
                                       - glm::dot(tr2.dots[0], b_tr2) * b_tr2;

    if (fabsf(glm::length(pl1_offset - pl2_offset)) > 0.001f)
        return 0;

    return 2;
}
