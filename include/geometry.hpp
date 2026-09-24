#pragma once

#include <glm/fwd.hpp>
#include <glm/detail/type_vec3.hpp>

struct Triangle {
  glm::vec3 dots[3];
};

struct Segment {
  float p1, p2;
};

int get_intersection_dim(Triangle &tr1, Triangle &tr2,
                         glm::vec3 *pts);
void get_intersection_points(Triangle &tr1, Triangle &tr2,
                             glm::vec3 *pts);
void get_line(Triangle &tr1, Triangle &tr2,
              glm::vec3 &line_offset, glm::vec3 &line_dir);
void get_segments(Triangle &tr1, Triangle &tr2,
                  Segment &seg_1, Segment &seg_2);
void get_plane(Triangle &tr, glm::vec3 plane_offset,
               glm::vec3 dir1, glm::vec3 dir2);
glm::vec4 solve_sle4(glm::mat4 matrix, glm::vec4 b);
int find_max_abs_element_v4(glm::vec4 v, int start_index);
void swap_rows_m4(glm::mat4 &m4, int i, int j);
void swap_elem_v4(glm::vec4 &v4, int i, int j);
void simplify_rows_m4(glm::mat4 &m4, glm::vec4 &b, int main_row, int col);
