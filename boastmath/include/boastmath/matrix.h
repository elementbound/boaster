#ifndef BOASTMATH_MATRIX_H_
#define BOASTMATH_MATRIX_H_

#include "./types.h"

#define bm_m(m, i, j) m[(i) * 4 + (j)]

void bm_matmul (bm_mat4 result, bm_mat4 a, bm_mat4 b);
void bm_mattrans (bm_vec4 result, bm_mat4 mat, bm_vec4 v);

void bm_mat_lookat (
    bm_mat4 result,
    float fromX, float fromY, float fromZ,
    float atX, float atY, float atZ,
    float upX, float upY, float upZ
);

void bm_mat_perspective (
    bm_mat4 result,
    float left, float right,
    float top, float bottom,
    float near, float far
    // (float) wherever you are
);

void bm_mat_perspective_fov (
    bm_mat4 result,
    float aspect, float fov,
    float near, float far
);

#endif  // BOASTMATH_MATRIX_H_
