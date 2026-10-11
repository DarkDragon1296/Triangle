#include "test_generator.hpp"
#include "geometry.hpp"
#include <glm/ext.hpp>

namespace TOP_LEVEL_NAMESPACE {

void generate_triangle_test(std::vector<Triangle> &triangles) {
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

        triangles.push_back(triangle);
    }
}

}