#pragma once

#include <vector>
#include "geometry.hpp"

namespace test_gen {

enum TriangleTestProperties {
    TRIANGLES_AMOUNT = 20,
    RADIUS_1 = 8,
    RADIUS_2 = -10,
    RADIUS_3 = 10,
    OFFSET_1 = 10,
    OFFSET_2 = 10,
    DEG_ROTATION_1 = 10,
    DEG_ROTATION_2 = 60
};

enum TriangleFieldPropeties{
    SIDE = 10,
    TRIANGLE_LENGTH = 2
};

void TEST_OBJECTS(std::vector<gmt::Triangle> &batch, 
                  std::vector<gmt::Segment3D> &segs);
void generate_triangle_test (std::vector<gmt::Triangle> &triangles);
void generate_triangle_field(std::vector<gmt::Triangle> &triangles);

}
