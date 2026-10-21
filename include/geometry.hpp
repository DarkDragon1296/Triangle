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
    std::vector<Plane> pl{};
    std::vector<Triangle> tr{};
    public:

    void push_tr(Triangle triangle) {
        tr.push_back(triangle);
        Plane plane;
        glm::vec3 a  = triangle.dots[1] - triangle.dots[0];
        glm::vec3 b  = triangle.dots[2] - triangle.dots[0];
        plane.n      = glm::normalize(glm::cross(a, b));
        plane.offset = glm::dot(plane.n, triangle.dots[0]);

        pl.push_back(plane);
    }

    const Triangle &get_tr(size_t i) const {
        return tr[i];
    }

    const Plane &get_pl(size_t i) const {
        return pl[i];
    }

    std::vector<Triangle> move_tr(void) const {
        return std::move(tr);
    }

    size_t get_size(void) const {
        return tr.size();
    }

};

void get_intersection_points(const TrData &tr_data, const size_t i1,
                             const size_t i2, std::vector<Segment3D> &objs);
int  get_intersection_dim   (const Plane pl1, const Plane pl2);
bool get_3d_segment         (const TrData &tr_data, const size_t i1,
                             const size_t i2, Segment3D &seg_3d);
bool get_line               (const Plane &tr1, const Plane &tr2, Line &line);
bool get_1d_segment         (Triangle tr, Line line, Segment1D &seg1);
bool get_1d_intersection_segment(Segment1D seg1, Segment1D seg2,
                                 Segment1D &seg_res); // TODO: another name?
Segment3D segment_transform_1d_3d(Line line, Segment1D seg_1d);

void print_vec3(glm::vec3 v);

void sort_segment_points(Segment1D &seg);
bool halfspace_test(const Plane &pl_divider, const Triangle &tr_subject);
void sort_triangles(std::vector<Triangle> &triangle_list);
int dir_z_sort_cmp(const void *x, const void *y);
float get_min_z_tr_pos(const Triangle &tr);
float get_max_z_tr_pos(const Triangle &tr);

}
