#pragma once

#include <vector>
#include "geometry.hpp"

namespace TOP_LEVEL_NAMESPACE {

enum TriangleTestProperties {
    TRIANGLES_AMOUNT = 3,
    RADIUS_1 = 5,
    RADIUS_2 = 5,
    RADIUS_3 = -3,
    OFFSET_1 = 10,
    OFFSET_2 = 10,
    DEG_ROTATION_1 = 10,
    DEG_ROTATION_2 = 50
};

void generate_triangle_test(std::vector<Triangle> &triangles);

}