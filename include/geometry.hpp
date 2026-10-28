#pragma once

#include <glm/glm.hpp>
#include <glm/fwd.hpp>
#include <glm/detail/type_vec3.hpp>
#include <vector>
#include <algorithm>
#include <utility>

namespace gmt {

struct Line {
    glm::vec3 offset;
    glm::vec3 e;
};

struct Triangle {
    glm::vec3 dots[3];
};

struct Plane {
    glm::vec3 n{};
    float offset{};

    Plane(const Triangle &tr) { 
        glm::vec3 a  = tr.dots[1] - tr.dots[0];
        glm::vec3 b  = tr.dots[2] - tr.dots[0];
        n      = glm::normalize(glm::cross(a, b));
        offset = glm::dot(n, tr.dots[0]);
    }
};

struct Segment3D {
    glm::vec3 dots[2];
};

struct Segment1D {
    float dots[2];
};

struct Box { // TODO: hide values
    glm::vec3 dots[2]{};

    Box(const Triangle &tr) {
        dots[0][0] = std::min({tr.dots[0][0], tr.dots[1][0], tr.dots[2][0]});
        dots[0][1] = std::min({tr.dots[0][1], tr.dots[1][1], tr.dots[2][1]});
        dots[0][2] = std::min({tr.dots[0][2], tr.dots[1][2], tr.dots[2][2]});
        dots[1][0] = std::max({tr.dots[0][0], tr.dots[1][0], tr.dots[2][0]});
        dots[1][1] = std::max({tr.dots[0][1], tr.dots[1][1], tr.dots[2][1]});
        dots[1][2] = std::max({tr.dots[0][2], tr.dots[1][2], tr.dots[2][2]});
    }
};

int comp_z(const std::pair<size_t, float> &a,
           const std::pair<size_t, float> &b); /* TODO: Move that function anywhere */

class SortValue {
    std::vector<std::pair<size_t, float>> data{};

    public:

    void push_value(const float &value) {
        data.push_back(std::make_pair(data.size(), value));
    }

    size_t get_index(const size_t &i) const & {
        return data[i].first;
    }

    void sort(void) {
        std::sort(data.begin(), data.end(), comp_z);
    }

    void clear(void) {
        data.clear();
    }
};

class TrData {
    std::vector<Plane>    pl{};
    std::vector<Triangle> tr{};

    public:

    std::vector<Box> bx{};
    SortValue srt{};

    void push_tr(Triangle triangle) {
        tr.push_back(triangle);
        pl.push_back(Plane{triangle});
        bx.push_back(Box  {triangle});
        srt.push_value(bx[bx.size() - 1].dots[0][2]);
    }

    const Triangle &get_tr(size_t i) const & {
        return tr[srt.get_index(i)];
    }

    const Plane &get_pl(size_t i) const & {
        return pl[srt.get_index(i)];
    }

    std::vector<Triangle> &&move_tr(void) {
        pl.clear();
        bx.clear();
        srt.clear();
        return std::move(tr);
    }

    size_t get_size(void) const {
        return tr.size();
    }
};

struct ObjsData {
    TrData tr_data;
    std::vector<Triangle> int_tr;
    std::vector<Segment3D> int_seg;
};

void calculate_intersections(ObjsData &objs);
void get_intersection_points(ObjsData &objs, const size_t i1, const size_t i2);
int  get_intersection_dim   (const Plane pl1, const Plane pl2);
bool get_3d_segment         (const TrData &tr_data, const size_t i1,
                             const size_t i2, Segment3D &seg_3d);
bool get_line               (const Plane &tr1, const Plane &tr2, Line &line);
bool get_1d_segment         (Triangle tr, Line line, Segment1D &seg1);
bool get_1d_intersection_segment(Segment1D seg1, Segment1D seg2,
                                 Segment1D &seg_res); // TODO: another name?
Segment3D segment_transform_1d_3d(Line line, Segment1D seg_1d);

bool get_int_triangle(const TrData &tr_data, const size_t i1, const size_t i2,
                      std::vector<Triangle> &int_tr);

void print_vec3(glm::vec3 v);

void sort_segment_points(Segment1D &seg);
bool halfspace_test(const Plane &pl_divider, const Triangle &tr_subject);
size_t find_max_bx_index(const TrData &tr_data, size_t i);

}
