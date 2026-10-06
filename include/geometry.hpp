#pragma once

#include "utils.hpp"

#include <glm/fwd.hpp>
#include <glm/detail/type_vec3.hpp>
#include <vector>

namespace TOP_LEVEL_NAMESPACE {

enum Figures {
  POINT    = 1,
  SEGMENT  = 2,
  TRIANGLE = 3
};

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

struct RenderObjects {
  std::vector<Triangle> triangles;
  std::vector<Segment3D> segments;
  std::vector<glm::vec3> points;
};

void get_intersection_points(Triangle tr1, Triangle tr2, RenderObjects &objs);
int get_intersection_dim(Triangle tr1, Triangle tr2);
Segment3D get_3d_segment(Triangle tr1, Triangle tr2);
void get_line(Triangle tr1, Triangle tr2, Line &line);
bool get_1d_segment(Triangle tr, Line line, Segment1D &seg1);
bool get_1d_intersection_segment(Segment1D seg1,
                                 Segment1D seg2,
                                 Segment1D &seg_res);
Segment3D segment_transform_1d_3d(Line line, Segment1D seg_1d);
void get_plane(Triangle tr, glm::vec3 &plane_offset,
               glm::vec3 &dir1, glm::vec3 &dir2);
void sort_segment_points(Segment1D &seg);
void print_vec3(glm::vec3 v);

bool get_lc_coeffs(std::vector<glm::vec3> mat, glm::vec3 b, float *res_coeff);
int find_max_abs_element_v3(glm::vec3 v3, int start_index);
void swap_rows_mat(std::vector<glm::vec3> &mat, int i, int j);
void swap_elem_v3(glm::vec3 &v3, int i, int j);
void simplify_rows_mat(std::vector<glm::vec3> &mat, glm::vec3 &b,
                       int main_row, int col);
void printf_mat(std::vector<glm::vec3> &mat, glm::vec3 b);

}
