#pragma once

#include <vector>
#include "geometry.hpp"

namespace test_gen {

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

enum TriangleSpiralPropeties{
    SPIRAL_AMOUNT = 50000,
    ANGLE_STEP = 1
};

void TEST_OBJECTS(gmt::TrData &batch, std::vector<gmt::Segment3D> &segs);
void generate_triangle_test (gmt::TrData &tr_data);
void generate_triangle_field(gmt::TrData &tr_data);

}
