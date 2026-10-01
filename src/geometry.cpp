#include "geometry.hpp"

#include <glm/ext.hpp>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream> // TODO: del

void get_intersection_points(Triangle tr1, Triangle tr2, RenderObjects &objs) {
  int dim = get_intersection_dim(tr1, tr2);
  std::cout << "dim = " << dim << std::endl; // TODO: del

  if (dim == 1) {
    Segment3D segment = get_3d_segment(tr1, tr2);
    objs.segments.push_back(segment);

  } else if (dim == 2) {
    /* TODO: */
  }
}

int get_intersection_dim(Triangle tr1, Triangle tr2) {
  glm::vec3 a_tr1 = tr1.dots[2] - tr1.dots[0];
  glm::vec3 b_tr1 = tr1.dots[2] - tr1.dots[1];
  glm::vec3 a_tr2 = tr2.dots[2] - tr2.dots[0];
  glm::vec3 b_tr2 = tr2.dots[2] - tr2.dots[1];

  glm::vec3 n_tr1 = glm::cross(a_tr1, b_tr1);
  glm::vec3 n_tr2 = glm::cross(a_tr2, b_tr2);

  float cross_res = glm::length(glm::cross(n_tr1, n_tr2));

  if (fabsf(cross_res) > 0.001f)
    return 1;

  glm::vec3 pl1_offset = tr1.dots[0] - glm::dot(tr1.dots[0], a_tr1) * a_tr1
                                     - glm::dot(tr1.dots[0], b_tr1) * b_tr1;

  glm::vec3 pl2_offset = tr2.dots[0] - glm::dot(tr2.dots[0], a_tr2) * a_tr2
                                     - glm::dot(tr2.dots[0], b_tr2) * b_tr2;

  pl1_offset = glm::normalize(pl1_offset);
  pl2_offset = glm::normalize(pl2_offset);

  if (fabsf(glm::length(pl1_offset - pl2_offset)) > 0.001f)
    return 0;

  return 2;
}

Segment3D get_3d_segment(Triangle tr1, Triangle tr2) {
  Segment1D seg1{}, seg2{}, seg_1d_res{};
  Line line{};

  get_line(tr1, tr2, line);
  std::cout << "line: offset = ";
  print_vec3(line.offset);
  std::cout << ", direction = ";
  print_vec3(line.e);
  std::cout << std::endl; // TODO: del

  bool is_seg1_exist = get_1d_segment(tr1, line, seg1);
  bool is_seg2_exist = get_1d_segment(tr2, line, seg2);

  if (!is_seg1_exist)
    std::cout << "seg1 isn't exist" << std::endl;

  if (!is_seg2_exist)
    std::cout << "seg2 isn't exist" << std::endl;

  get_1d_intersection_segment(seg1, seg2, seg_1d_res);
  Segment3D seg_3d = segment_transform_1d_3d(line, seg_1d_res);

  return seg_3d;
}

void get_line(Triangle tr1, Triangle tr2, Line &line) {
  glm::vec4 a1 = glm::vec4(tr1.dots[1] - tr1.dots[0], 0.0f);
  glm::vec4 a2 = glm::vec4(tr1.dots[2] - tr1.dots[0], 0.0f);
  glm::vec4 b1 = glm::vec4(tr2.dots[1] - tr2.dots[0], 0.0f);
  glm::vec4 b2 = glm::vec4(tr2.dots[2] - tr2.dots[0], 0.0f);

  glm::mat4 sle(a1, a2, b1, b2);
  glm::vec4 c  = glm::vec4(tr2.dots[0] - tr2.dots[0], 0.0f);

  glm::vec4 sle_res = solve_sle4(sle, c);
  std::cout << "sle res = ";
  print_vec3(glm::vec3(sle_res));
  std::cout << std::endl;

  line.offset = tr1.dots[0] + sle_res[0] * (tr1.dots[1] - tr1.dots[0])
                            + sle_res[1] * (tr1.dots[2] - tr1.dots[0]);

  glm::vec3 n1 = glm::cross(tr1.dots[1] - tr1.dots[0],
                            tr1.dots[2] - tr1.dots[0]);
  glm::vec3 n2 = glm::cross(tr2.dots[1] - tr2.dots[0],
                            tr2.dots[2] - tr2.dots[0]);
  line.e = glm::normalize(glm::cross(n1, n2));
}

bool get_1d_segment(Triangle tr, Line line, Segment1D &seg) {
  glm::vec4 z(0.0f);
  glm::mat4 sle(glm::vec4(line.e, 0.0f), z, z, z);
  int segment_counter = 0;

  for (int i = 0; i < 3; i++) {
    sle[1] = glm::vec4(tr.dots[(i + 1) % 3] - tr.dots[i % 3], 0.0f);

    glm::vec4 c = glm::vec4(line.offset - tr.dots[i % 3], 0.0f);
    glm::vec4 sle_res = solve_sle4(sle, c);

    // TODO: del
    std::cout << "[" << sle_res[0] << ", " << sle_res[1] << "]" << std::endl;

    if (fabsf(sle_res[1]) <= 1.0f && segment_counter < 2) {
      seg.dots[segment_counter] = sle_res[0];
      segment_counter++;
    }
  }

  if (segment_counter < 2)
    return false;

  return true;
}

bool get_1d_intersection_segment(Segment1D seg1,
                                Segment1D seg2,
                                Segment1D &seg_res) {
  sort_segment_points(seg1);
  sort_segment_points(seg2);

  if (seg1.dots[1] < seg2.dots[0] || seg2.dots[1] < seg1.dots[1])
    return false;

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

void get_plane(Triangle tr, glm::vec3 &plane_offset,
               glm::vec3 &dir1, glm::vec3 &dir2) {
  plane_offset = tr.dots[0];

  dir1 = glm::normalize(tr.dots[1] - tr.dots[0]);
  dir2 = tr.dots[1] - tr.dots[0];
  dir2 = glm::normalize(dir2 - glm::dot(dir2, dir1) * dir1);
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

bool get_lc_coeffs(std::vector<glm::vec3> mat, glm::vec3 b, float *res_coeff) {
  for (int col = 0, row = 0; col < (int)mat.size() && row < 3; col++) {
    int max_row_index = find_max_abs_element_v3(mat[col], row);

    if (fabsf(mat[col][max_row_index]) < 0.001f) {
      mat[col] = glm::vec3(0.0f);
    } else {
      swap_rows_mat(mat, row, max_row_index);
      swap_elem_v3(b, row, max_row_index);
      simplify_rows_mat(mat, b, row, col);

      row++;
    }
  }

  for (int i = 0, j = 0; i < mat.size(); i++) {
    if (fabsf(glm::length(matrix[i])) < 0.001f) {
      res_coeff[i] = 0.0f;
    } else {
      res_coeff[i] = b[j];
      j++;
    }
    std::cout << res_coeff[i] << " ";
  }

  std::cout << std::endl;
  return true;
}

int find_max_abs_element_v3(glm::vec3 v3, int start_index) {
  int res = start_index;
  for (int i = start_index; i < 3; i++) {
    if (fabsf(v3[res]) < fabsf(v3[i]))
      res = i;
  }
  return res;
}

void swap_rows_mat(std::vector<glm::vec3> &mat, int i, int j) {
  for (int l = 0; l < mat.size(); l++)
    swap_elem_v3(mat[i], i, j);
}

void swap_elem_v3(glm::vec3 &v3, int i, int j) {
  float tmp = v3[i];
  v3[i] = v3[j];
  v3[j] = tmp;
}

void simplify_rows_mat(std::vector<glm::vec3> &mat, glm::vec3 &b,
                       int main_row, int col) {
  for (int i = 0; i < main_row; i++) {
    


  }

  /*  mat = glm::transpose(m3);

  for (int i = 2; i > main_row; i--) {
    b[i]  -= (m3[i][col] / m3[main_row][col]) *  b[main_row];
    m3[i] -= (m3[i][col] / m3[main_row][col]) * m3[main_row];
  }

  for (int i = 0; i < main_row; i++) {
    b[i]  -= (m3[i][col] / m3[main_row][col]) *  b[main_row];
    m3[i] -= (m3[i][col] / m3[main_row][col]) * m3[main_row];
  }

  b[main_row]  = (1.0f / m3[main_row][col]) *  b[main_row];
  m3[main_row] = (1.0f / m3[main_row][col]) * m3[main_row];

  m3 = glm::transpose(m3);
  */
}