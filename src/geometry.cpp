#include "geometry.hpp"

#include <glm/ext.hpp>
#include <GLFW/glfw3.h>
#include <cmath>
#include <algorithm>
#include <iostream>

namespace gmt {

static inline bool fequals(float a, float b);

void get_intersection_points(const TrData &tr_data, const size_t i1,
                             const size_t i2, std::vector<Segment3D> &objs)
{
    if (halfspace_test(tr_data.pl[i1], tr_data.tr[i2]) ||
        halfspace_test(tr_data.pl[i2], tr_data.tr[i1]))
            return;

    int dim = get_intersection_dim(tr_data.pl[i1], tr_data.pl[i2]);

    if (dim == 1) {
        Segment3D segment;
        bool is_exist = get_3d_segment(tr_data, i1, i2, segment);
        if (is_exist)
            objs.push_back(segment);

    } else if (dim == 2) {
        /* TODO: */
    }
}

/// Returns 'true' if halfspace_test has 
/// determined that subject is entirely in one 
/// of the halfspaces created by divider
bool halfspace_test(const Plane &pl_divider, const Triangle &tr_subject) {
    bool first_iteration   = true;
    bool in_left_halfspace = false;

    for (uint i = 0; i < 3; i++) {
        float plane_equation_res = glm::dot(pl_divider.n, tr_subject.dots[i]) -
                                            pl_divider.offset;

        if (fequals(plane_equation_res, 0.0f))
            return false;

        if (plane_equation_res < 0.0f) {
            if (!first_iteration && !in_left_halfspace)
                return false;

            in_left_halfspace = true;
        } else /* (plane_equation_res > 0.0f) */ {
            if (!first_iteration && in_left_halfspace)
                return false;

            in_left_halfspace = false;
        }

        first_iteration = false;
    }

    return true;
}

void sort_triangles(std::vector<Triangle> &triangle_list) {
    std::qsort(triangle_list.data(), triangle_list.size(),
               sizeof(Triangle), dir_z_sort_cmp);
}

int dir_z_sort_cmp(const void *x, const void *y) {
    const Triangle arg1 = *static_cast<const Triangle *>(x);
    const Triangle arg2 = *static_cast<const Triangle *>(y);

    float min_z_pos_1 = get_min_z_tr_pos(arg1);
    float min_z_pos_2 = get_min_z_tr_pos(arg2);

    if (min_z_pos_1 < min_z_pos_2) {
        return 1;
    } else {
        return -1;
    }
}

float get_min_z_tr_pos(const Triangle &tr) {
    float pos = std::min(std::min(tr.dots[0][2], tr.dots[1][2]), tr.dots[2][2]);
    return pos;
}

float get_max_z_tr_pos(const Triangle &tr) {
    float pos = std::max(std::max(tr.dots[0][2], tr.dots[1][2]), tr.dots[2][2]);
    return pos;
}

int get_intersection_dim(const Plane pl1, const Plane pl2) {
    float cross_value = glm::length(glm::cross(pl1.n, pl2.n));

    if (!fequals(cross_value, 0.0f))
        return 1;

    if (!fequals(pl1.offset - pl2.offset, 0.0f))
        return 0;

    return 2;
}

bool get_3d_segment(const TrData &tr_data, const size_t i1,
                    const size_t i2, Segment3D &seg_3d)
{
    Segment1D seg1{}, seg2{}, seg_1d_res{};
    Line line{};
    bool is_line_exist = get_line(tr_data.pl[i1], tr_data.pl[i2], line);

    if (!is_line_exist)
        return false;

    bool is_seg1_exist = get_1d_segment(tr_data.tr[i1], line, seg1);
    bool is_seg2_exist = get_1d_segment(tr_data.tr[i2], line, seg2);

    if (!is_seg1_exist || !is_seg2_exist)
        return false;

    bool is_exist = get_1d_intersection_segment(seg1, seg2, seg_1d_res);

    if (is_exist)
        seg_3d = segment_transform_1d_3d(line, seg_1d_res);

    return true;
}

bool get_line(const Plane &pl1, const Plane &pl2, Line &line) {
    glm::vec3 e = glm::cross(pl1.n, pl2.n);
    float len   = glm::length(e);

    if (fequals(len, 0.0f))
        return false;

    line.e = e / len;
    float dot_value = glm::dot(pl1.n, pl2.n);
    float det = float(pow(len, 2.0f));

    if (fequals(det, 0.0f))
        return false;

    float a = (pl1.offset - pl2.offset * dot_value) / det;
    float b = (pl2.offset - pl1.offset * dot_value) / det;

    line.offset = a * pl1.n + b * pl2.n;

    return true;
}

bool get_1d_segment(Triangle tr, Line line, Segment1D &seg) {
    int segment_counter = 0;

    for (int i = 0; i < 3; i++) {
        glm::vec3 b = tr.dots[i % 3] - line.offset;
        glm::vec3 dir = tr.dots[(i + 1) % 3] - tr.dots[i % 3];
        glm::vec3 v = glm::cross(dir, line.e);
        float det = glm::dot(v, v);

        if (fequals(det, 0.0f)) {
            std::cout << "Error in segment finding!" << std::endl;
            return false;
        }

        float s = -glm::dot(glm::cross(b, line.e), v) / det;
        float t = -glm::dot(glm::cross(b, dir   ), v) / det;

        if (!fequals(glm::length(b + s * dir - t * line.e), 0.0f))
            continue;

        if (s <= 1.0f && s >= 0.0f) {
            seg.dots[segment_counter] = t;
            segment_counter++;
        }

        if (segment_counter == 2 && fequals(seg.dots[0], seg.dots[1]))
            segment_counter--;

        if (segment_counter == 2)
            break;
    }

    if (segment_counter < 2)
        return false;

    return true;
}

bool get_1d_intersection_segment(Segment1D seg1, Segment1D seg2,
        Segment1D &seg_res) {
    sort_segment_points(seg1);
    sort_segment_points(seg2);
    if (seg1.dots[1] < seg2.dots[0] || seg2.dots[1] < seg1.dots[0])
        return false;
    // TODO: std::min and std:max
    seg_res.dots[0] = seg1.dots[0] > seg2.dots[0] ? seg1.dots[0] : seg2.dots[0];
    seg_res.dots[1] = seg1.dots[1] < seg2.dots[1] ? seg1.dots[1] : seg2.dots[1];

    return true;
}

Segment3D segment_transform_1d_3d(Line line, Segment1D seg_1d) {
    Segment3D seg_3d{};

    seg_3d.dots[0] = line.offset + seg_1d.dots[0] * line.e;
    seg_3d.dots[1] = line.offset + seg_1d.dots[1] * line.e;

    return seg_3d;
} 

void sort_segment_points(Segment1D &seg) {
    if (seg.dots[0] > seg.dots[1]) {
        float tmp = seg.dots[0];
        seg.dots[0] = seg.dots[1];
        seg.dots[1] = tmp;
    }
}

void print_vec3(glm::vec3 v) {
    std::cout << "(" << v[0] << ", " << v[1] << ", " << v[2] << ")";
}

static inline bool fequals(float a, float b) {
    return std::abs(a - b) < 1.0e-6f; //TODO: name the tolerance value
}

}
