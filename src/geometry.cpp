#include <glm/ext.hpp>
#include <GLFW/glfw3.h>
#include <iostream>
#include <cmath>
#include "geometry.hpp"

int get_intersection_dim(struct triangle &tr1, struct triangle &tr2) {
    glm::vec3 a_tr1 = tr1.dots[2] - tr1.dots[0];
    glm::vec3 b_tr1 = tr1.dots[2] - tr1.dots[1];
    glm::vec3 a_tr2 = tr2.dots[2] - tr2.dots[0];
    glm::vec3 b_tr2 = tr2.dots[2] - tr2.dots[1];

    glm::vec3 n_tr1 = glm::cross(a_tr1, b_tr1);
    glm::vec3 n_tr2 = glm::cross(a_tr2, b_tr2);

    float cross_res = glm::length(glm::cross(n_tr1, n_tr2));

    if (fabsf(cross_res) > 0.001f)
        return 1;
// TODO: Проверить, не нужно ли делить на длины
    glm::vec3 pl1_offset = tr1.dots[0] - glm::dot(tr1.dots[0], a_tr1) * a_tr1
                                       - glm::dot(tr1.dots[0], b_tr1) * b_tr1;

    glm::vec3 pl2_offset = tr2.dots[0] - glm::dot(tr2.dots[0], a_tr2) * a_tr2
                                       - glm::dot(tr2.dots[0], b_tr2) * b_tr2;

    if (fabsf(glm::length(pl1_offset - pl2_offset)) > 0.001f)
        return 0;

    return 2;
}

void get_intersection_points(struct triangle &tr1, struct triangle &tr2,
                             glm::vec3 *pts) {
    int dim = get_intersection_dim(tr1, tr2);

    if (dim == 1) {
        glm::vec3 line_offset(0.0f), line_dir(0.0f);

        get_line(tr1, tr2, line_offset, line_dir);

        // переходим к новому базису
        // ищем пересечения
        // возвращаемся к старому базису
    } else if (dim == 2) {
        glm::vec3 plane_offset(0.0f), dir1(0.0f), dir2(0.0f);
        get_plane(tr1, plane_offset, dir1, dir2);
         
        // переходим к новому базису
        // ищем пересечения
        // возвращаемся к старому базису 
    }
}

// TODO: Эту функцию можно разделить на 2 части + сделать ее нормальнее
void get_line(struct triangle &tr1, struct triangle &tr2,
              glm::vec3 &line_offset, glm::vec3 &line_dir) {
    glm::vec4 a1 = glm::vec4(tr1.dots[1] - tr1.dots[0], 0.0f);
    glm::vec4 a2 = glm::vec4(tr1.dots[2] - tr1.dots[0], 0.0f);
    glm::vec4 b1 = glm::vec4(tr2.dots[1] - tr2.dots[0], 0.0f);
    glm::vec4 b2 = glm::vec4(tr2.dots[2] - tr2.dots[0], 0.0f);

    glm::mat4 sle(a1, a2, b1, b2);
    glm::vec4 c  = glm::vec4(tr2.dots[0] - tr2.dots[0], 0.0f);

    glm::vec4 sle_res = solve_sle4(sle, c);

    line_offset = tr1.dots[0] + sle_res[0] * (tr1.dots[1] - tr1.dots[0])
                              + sle_res[1] * (tr1.dots[2] - tr1.dots[0]);

    glm::vec3 n1 = glm::cross(tr1.dots[1] - tr1.dots[0],
                              tr1.dots[2] - tr1.dots[0]);
    glm::vec3 n2 = glm::cross(tr2.dots[1] - tr2.dots[0],
                              tr2.dots[2] - tr2.dots[0]);

    line_dir = glm::normalize(glm::cross(n1, n2));
}

void get_plane(struct triangle &tr, glm::vec3 plane_offset,
               glm::vec3 dir1, glm::vec3 dir2) {
    plane_offset = tr.dots[0];

    dir1 = glm::normalize(tr.dots[1] - tr.dots[0]);
    dir2 = tr.dots[1] - tr.dots[0];
    dir2 = glm::normalize(dir2 - glm::dot(dir2, dir1) * dir1);
}

glm::vec4 solve_sle4(glm::mat4 matrix, glm::vec4 b) {
    for (int col = 0, row = 0; col < 4; col++) {
        int max_row_index = find_max_abs_element_v4(matrix[col], row);

        if (fabsf(matrix[col][max_row_index]) < 0.001f) {
            matrix[col] = glm::vec4(0.0f);
        } else {
            swap_rows_m4(matrix, row, max_row_index);
            swap_elem_v4(b, row, max_row_index);
            simplify_rows_m4(matrix, b, row, col);

            row++;
        }
    }

    glm::vec4 res;

    for (int i = 0, j = 0; i < 4; i++) {
        if (fabsf(glm::length(matrix[i])) < 0.001f) {
            res[i] = 0.0f;
        } else {
            res[i] = b[j];
            j++;
        }
    }

    return res;
}

int find_max_abs_element_v4(glm::vec4 v4, int start_index) {
    int res = start_index;
    for (int i = start_index; i < 4; i++) {
        if (fabsf(v4[res]) < fabsf(v4[i]))
            res = i;
    }
    return res;
}

void swap_rows_m4(glm::mat4 &m4, int i, int j) {
    m4 = glm::transpose(m4);
    glm::vec4 tmp_v4 = m4[i];
    m4[i] = m4[j];
    m4[j] = tmp_v4;
    m4 = glm::transpose(m4);
}

void swap_elem_v4(glm::vec4 &v4, int i, int j) {
    float tmp = v4[i];
    v4[i] = v4[j];
    v4[j] = tmp;
}

void simplify_rows_m4(glm::mat4 &m4, glm::vec4 &b, int main_row, int col) {
    m4 = glm::transpose(m4);

    for (int i = 3; i > main_row; i--) {
        b[i]  -= (m4[i][col] / m4[main_row][col]) *  b[main_row];
        m4[i] -= (m4[i][col] / m4[main_row][col]) * m4[main_row];
    }

    for (int i = 0; i < main_row; i++) {
        b[i]  -= (m4[i][col] / m4[main_row][col]) *  b[main_row];
        m4[i] -= (m4[i][col] / m4[main_row][col]) * m4[main_row];
    }

    b[main_row]  = (1.0f / m4[main_row][col]) *  b[main_row];
    m4[main_row] = (1.0f / m4[main_row][col]) * m4[main_row];

    m4 = glm::transpose(m4);
}