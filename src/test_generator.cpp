#include "test_generator.hpp"
#include "geometry.hpp"
#include <glm/ext.hpp>

namespace test_gen {

using namespace gmt;

void TEST_OBJECTS(gmt::TrData &batch, std::vector<Segment3D> &segs) {
    #if 0
    generate_triangle_test(batch);
    #endif

    #if 1
    generate_triangle_field(batch);
    #endif

    calculate_intersections(batch, segs);
}

void generate_triangle_test(gmt::TrData &tr_data) {
    float deg_angle[3];
    float rad_angle[3];

    for (int i = 0; i < TRIANGLES_AMOUNT; i++) {
        Triangle triangle;

        deg_angle[0] = (float)i / TRIANGLES_AMOUNT * 360.0f + DEG_ROTATION_1; 
        deg_angle[1] = (float)i / TRIANGLES_AMOUNT * 360.0f + DEG_ROTATION_2; 
        deg_angle[2] = (float)i / TRIANGLES_AMOUNT * 360.0f; 

        rad_angle[0] = glm::radians(deg_angle[0]); 
        rad_angle[1] = glm::radians(deg_angle[1]); 
        rad_angle[2] = glm::radians(deg_angle[2]); 

        triangle.dots[0][0] = RADIUS_1 * glm::cos(rad_angle[0]);
        triangle.dots[0][1] = RADIUS_1 * glm::sin(rad_angle[0]);
        triangle.dots[0][2] = OFFSET_1;

        triangle.dots[1][0] = RADIUS_2 * glm::cos(rad_angle[1]);
        triangle.dots[1][1] = RADIUS_2 * glm::sin(rad_angle[1]);
        triangle.dots[1][2] = OFFSET_2;

        triangle.dots[2][0] = RADIUS_3 * glm::cos(rad_angle[2]);
        triangle.dots[2][1] = RADIUS_3 * glm::sin(rad_angle[2]);
        triangle.dots[2][2] = 0.0f;

        tr_data.push_tr(triangle);
    }
}

void generate_triangle_field(gmt::TrData &tr_data) {
    for (int i = 1; i < SPIRAL_AMOUNT + 1; i++) {
        Triangle triangle;
        float angle = glm::radians(float(ANGLE_STEP * i));
        float r = 0.05f * float(i);

        triangle.dots[0][0] = 1.2f * r * glm::cos(angle);
        triangle.dots[0][2] = 1.2f * r * glm::sin(angle);
        triangle.dots[0][1] = 0.0f;

        triangle.dots[1][0] = r * glm::cos(angle);
        triangle.dots[1][2] = r * glm::sin(angle);
        triangle.dots[1][1] = 0.01f * float(i);

        triangle.dots[2][0] = 1.3f * r * glm::cos(angle);
        triangle.dots[2][2] = 1.3f * r * glm::sin(angle);
        triangle.dots[2][1] = 0.02f * float(i);

        tr_data.push_tr(triangle);
    }

}

}
