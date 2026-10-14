#pragma once

#include <vector>
#include "geometry.hpp"

namespace TOP_LEVEL_NAMESPACE {

enum TriangleTestProperties {
    TRIANGLES_AMOUNT = 10,
    RADIUS_1 = 5,
    RADIUS_2 = 10,
    RADIUS_3 = 5,
    OFFSET_1 = 10,
    OFFSET_2 = 10,
    DEG_ROTATION_1 = -90,
    DEG_ROTATION_2 = 90
};

void generate_triangle_test(std::vector<Triangle> &triangles);

}