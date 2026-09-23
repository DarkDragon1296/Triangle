#ifndef GEOMETRY_HPP
#define GEOMETRY_HPP

#include "glm/fwd.hpp"

struct triangle {
    glm::vec3 dots[3];
};

int get_intersection_dim(struct triangle &tr1, struct triangle &tr2,
                         glm::vec3 *pts);
void get_line(struct triangle &tr1, struct triangle &tr2,
              glm::vec3 &line_offset, glm::vec3 &line_dir);
glm::vec4 solve_sle4(glm::mat4 matrix, glm::vec4 b);
int find_max_abs_element_v4(glm::vec4 v, int start_index);
void swap_rows_m4(glm::mat4 &m4, int i, int j);
void swap_elem_v4(glm::vec4 &v4, int i, int j);
void simplify_rows_m4(glm::mat4 &m4, glm::vec4 &b, int main_row, int col);

#endif
