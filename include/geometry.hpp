#pragma once

#include <glm/fwd.hpp>
#include <glm/detail/type_vec3.hpp>

struct Triangle {
  glm::vec3 dots[3];
};

struct Segment3D {
  glm::vec3 dots[2];
};
/*
struct Objects {
  Triangle triangles
};
*/
struct Line {
  glm::vec3 offset;
  glm::vec3 e;
};

struct Plane {
  glm::vec3 offset;
  glm::vec3 e[2];
};

struct Segment1D {
  float p[2];
};

void get_intersection_points(Triangle tr1, Triangle tr2, glm::vec3 *pts);
int get_intersection_dim(Triangle tr1, Triangle tr2);
void get_intersection_segment(Segment1D seg1,
                              Segment1D seg2,
                              Segment1D &seg_res);
void get_line(Triangle tr1, Triangle tr2,
              glm::vec3 &line_offset, glm::vec3 &line_dir);
bool get_segments(Triangle tr1, Triangle tr2,
                  Segment1D &seg1, Segment1D &seg2);
void get_plane(Triangle tr, glm::vec3 &plane_offset,
               glm::vec3 &dir1, glm::vec3 &dir2);
glm::vec4 solve_sle4(glm::mat4 matrix, glm::vec4 b);
int find_max_abs_element_v4(glm::vec4 v, int start_index);
void swap_rows_m4(glm::mat4 &m4, int i, int j);
void swap_elem_v4(glm::vec4 &v4, int i, int j);
void simplify_rows_m4(glm::mat4 &m4, glm::vec4 &b, int main_row, int col);
