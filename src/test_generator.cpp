#include "test_generator.hpp"
#include "geometry.hpp"
#include <glm/ext.hpp>

namespace test_gen {

using namespace gmt;

void TEST_OBJECTS(std::vector<Triangle> &batch, std::vector<Segment3D> &segs) {
    [[maybe_unused]] Triangle tr1 = {{
        {0.0f, 0.0f, 0.0f},
        {2.0f, 2.0f, 0.0f},
        {0.0f, 2.0f, 0.0f}
    }};

    [[maybe_unused]] Triangle tr2 = {{
        {0.0f, 0.0f,  1.0f},
        {2.0f, 2.0f,  0.0f},
        {0.0f, 2.0f, -1.0f}
    }};

    #if 1
    generate_triangle_test(batch);
    #endif

    #if 0
    batch.push_back(tr1);
    batch.push_back(tr2);
    #endif

    #if 1
    for (size_t i = 0; i < batch.size(); i++) {
        for (size_t j = i + 1; j < batch.size(); j++) {
            get_intersection_points(batch[i], batch[j], segs);
        }
    }
    #endif
}

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

void generate_triangle_field(std::vector<Triangle> &triangles) {
    for (int i = 0; i < SIDE; i++) {
        for (int j = 0; j < SIDE; j++) {
            Triangle triangle;

            triangle.dots[0][0] = float(i);
            triangle.dots[0][2] = float(j);
            triangle.dots[0][1] = 0.0f;

            triangle.dots[1][0] = float(i);
            triangle.dots[1][2] = float(j);
            triangle.dots[1][1] = 1.0f;

            triangle.dots[2][0] = float(i + SIDE);
            triangle.dots[2][2] = float(j);
            triangle.dots[2][1] = 0.0f;

            triangles.push_back(triangle);
        }
    }

}

}
