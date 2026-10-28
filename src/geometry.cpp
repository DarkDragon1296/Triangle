#include "geometry.hpp"
#include "Timer.hpp"

#include <glm/ext.hpp>
#include <GLFW/glfw3.h>
#include <cmath>
#include <algorithm>
#include <iostream>

namespace gmt {

static inline bool fequals(float a, float b);

void calculate_intersections(ObjsData &obj) {
    size_t sz = obj.tr_data.get_size();

    obj.tr_data.srt.sort();

    for (size_t i = 0; i < sz; i++) {
        size_t max_index = find_max_bx_index(obj.tr_data, i);

        for (size_t j = i + 1; j <= max_index; j++) {
            size_t i1 = obj.tr_data.srt.get_index(i); 
            size_t j1 = obj.tr_data.srt.get_index(j);

            if (obj.tr_data.bx[i1].dots[1][0] < obj.tr_data.bx[j1].dots[0][0] ||
                obj.tr_data.bx[i1].dots[0][0] > obj.tr_data.bx[j1].dots[1][0] ||
                obj.tr_data.bx[i1].dots[1][1] < obj.tr_data.bx[j1].dots[0][1] ||
                obj.tr_data.bx[i1].dots[0][1] > obj.tr_data.bx[j1].dots[1][1])
            {
                continue;
            }

            if (halfspace_test(obj.tr_data.get_pl(i), obj.tr_data.get_tr(j)) ||
                halfspace_test(obj.tr_data.get_pl(j), obj.tr_data.get_tr(i)))
            {
                continue;
            }

            get_intersection_points(obj, i, j);
        }
    }
}

size_t find_max_bx_index(const TrData &tr_data, size_t i) {
    size_t i_srt = tr_data.srt.get_index(i);
    size_t sz = tr_data.get_size() - i - 1;
    size_t max_index = i;
    size_t divider = 1;

    while (sz / divider != 0) {
        size_t tmp_index = max_index + sz / divider;

        if (tmp_index < tr_data.get_size() && tr_data.bx[i_srt].dots[1][2] >
            tr_data.bx[tr_data.srt.get_index(tmp_index)].dots[0][2])
        { 
            max_index = tmp_index;
        }

        divider *= 2;
    }

    return max_index;
}

void get_intersection_points(ObjsData &objs, const size_t i1, const size_t i2) {
    int dim = get_intersection_dim(objs.tr_data.get_pl(i1),
                                   objs.tr_data.get_pl(i2));
    if (dim == 1) {
        Segment3D segment;
        bool is_exist = get_3d_segment(objs.tr_data, i1, i2, segment);

        if (is_exist)
            objs.int_seg.push_back(segment);

    } else if (dim == 2) {
#if 1
        std::vector<Triangle> int_tr;
        bool is_exist = get_int_triangle(objs.tr_data, i1, i2, int_tr);

        if (is_exist)
            objs.int_tr.insert(objs.int_tr.end(), int_tr.begin(), int_tr.end());
#endif
    }
}

bool get_int_triangle(const TrData &tr_data, const size_t i1, const size_t i2,
                      std::vector<Triangle> &int_tr)
{
    std::vector<glm::vec2> dots{};

    #if 0
    convert_tr_3d_2d();
    get_int_tr_dots(tr_data, i1, i2, dots);

    #endif
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
    bool is_line_exist = get_line(tr_data.get_pl(i1), tr_data.get_pl(i2), line);

    if (!is_line_exist)
        return false;

    bool is_seg1_exist = get_1d_segment(tr_data.get_tr(i1), line, seg1);
    bool is_seg2_exist = get_1d_segment(tr_data.get_tr(i2), line, seg2);

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

int comp_z(const std::pair<size_t, float> &a,
           const std::pair<size_t, float> &b) {
    return b.second > a.second;
}

}
