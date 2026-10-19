#pragma once

#include "utils.hpp"

#include <glm/fwd.hpp>
#include <glm/detail/type_vec3.hpp>
#include <vector>

namespace TOP_LEVEL_NAMESPACE {

struct Plane {
  glm::vec3 offset;
  glm::vec3 e[2];
};

struct Line {
  glm::vec3 offset;
  glm::vec3 e;
};

struct Triangle {
  glm::vec3 dots[3];
};

struct Segment3D {
  glm::vec3 dots[2];
};

struct Segment1D {
  float dots[2];
};

void get_intersection_points(const Triangle &tr1, const Triangle &tr2,
                             std::vector<Segment3D> &objs);
int  get_intersection_dim   (Triangle tr1, Triangle tr2);
bool get_3d_segment   (const Triangle &tr1, const Triangle &tr2,
                             Segment3D &seg_3d);
void get_line               (Triangle tr1, Triangle tr2, Line &line);
void get_plane              (Triangle tr, glm::vec3 &plane_offset,
                             glm::vec3 &dir1, glm::vec3 &dir2); // TODO: add Plane type
bool get_1d_segment         (Triangle tr, Line line, Segment1D &seg1);
bool get_1d_intersection_segment(Segment1D seg1, Segment1D seg2,
                                 Segment1D &seg_res); // TODO: another name?
void sort_segment_points(Segment1D &seg);
Segment3D segment_transform_1d_3d(Line line, Segment1D seg_1d);

void print_vec3(glm::vec3 v);

bool halfspace_test(const Triangle &divider, const Triangle &subject);
void sort_triangles(std::vector<Triangle> &triangle_list);
int dir_z_sort_cmp(const void *x, const void *y);
float get_min_z_tr_pos(const Triangle &tr);
float get_max_z_tr_pos(const Triangle &tr);

}