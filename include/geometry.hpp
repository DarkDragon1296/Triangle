#pragma once

#include <glm/glm.hpp>
#include <glm/fwd.hpp>
#include <glm/detail/type_vec3.hpp>
#include <vector>

namespace gmt {

struct Line {
    glm::vec3 offset;
    glm::vec3 e;
};

struct Triangle {
    glm::vec3 dots[3];
};

struct Plane {
    glm::vec3 n;
    float offset;
};

struct Segment3D {
    glm::vec3 dots[2];
};

struct Segment1D {
    float dots[2];
};

class TrData {
    public:
    std::vector<Plane> pl{};
    std::vector<Triangle> tr{};

    void calc_pl(void) {
        Plane plane;

        for (size_t i = 0; i < tr.size(); i++) {
            glm::vec3 a = tr[i].dots[1] - tr[i].dots[0];
            glm::vec3 b = tr[i].dots[2] - tr[i].dots[0];
            plane.n      = glm::normalize(glm::cross(a, b));
            plane.offset = glm::dot(plane.n, tr[i].dots[0]);

            pl.push_back(plane);
        }
    }
};

void get_intersection_points(const TrData &tr_data, const size_t i1,
                             const size_t i2, std::vector<Segment3D> &objs);
int get_intersection_dim    (const Plane pl1, const Plane pl2);
bool get_3d_segment         (const TrData &tr_data, const size_t i1,
                             const size_t i2, Segment3D &seg_3d);
bool get_line               (const Plane &tr1, const Plane &tr2, Line &line);
bool get_1d_segment         (Triangle tr, Line line, Segment1D &seg1);
bool get_1d_intersection_segment(Segment1D seg1, Segment1D seg2,
                                 Segment1D &seg_res); // TODO: another name?
void sort_segment_points(Segment1D &seg);
Segment3D segment_transform_1d_3d(Line line, Segment1D seg_1d);

void print_vec3(glm::vec3 v);

bool halfspace_test(const Plane &pl_divider, const Triangle &tr_subject);
void sort_triangles(std::vector<Triangle> &triangle_list);
int dir_z_sort_cmp(const void *x, const void *y);
float get_min_z_tr_pos(const Triangle &tr);
float get_max_z_tr_pos(const Triangle &tr);

}
