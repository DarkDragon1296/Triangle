#pragma once

#include <vector>
#include "geometry.hpp"

namespace TOP_LEVEL_NAMESPACE {

enum TriangleTestProperties {
    TRIANGLES_AMOUNT = 2000,
    RADIUS_1 = 8,
    RADIUS_2 = -10,
    RADIUS_3 = 10,
    OFFSET_1 = 10,
    OFFSET_2 = 10,
    DEG_ROTATION_1 = 10,
    DEG_ROTATION_2 = 60
};

void generate_triangle_test(std::vector<Triangle> &triangles);

}
