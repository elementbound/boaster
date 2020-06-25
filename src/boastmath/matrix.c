/*
 * This file contains code parts based on the Mesa library's implementation of
 * matrix operations. These parts are marked as such by a comment.
 * 
 * Included the Mesa license:
 * Mesa 3-D graphics library
 *
 * Copyright (C) 1999-2005  Brian Paul   All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a
 * copy of this software and associated documentation files (the "Software"),
 * to deal in the Software without restriction, including without limitation
 * the rights to use, copy, modify, merge, publish, distribute, sublicense,
 * and/or sell copies of the Software, and to permit persons to whom the
 * Software is furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included
 * in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
 * OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR
 * OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE,
 * ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
 * OTHER DEALINGS IN THE SOFTWARE.
 */

#include <math.h>
#include "include/boastmath/matrix.h"
#include "include/boastmath/vector.h"

#define R(row, col) result[(row) * 4 + (col)]
#define A(row, col) a[(row) * 4 + (col)]
#define B(row, col) b[(row) * 4 + (col)]

#define RV(i) result[i]
#define V(i) v[i]

// Based on the Mesa implementation
// See: https://gitlab.freedesktop.org/mesa/mesa/-/blob/master/src/mesa/math/m_matrix.c
void bm_matmul (bm_mat4 result, bm_mat4 a, bm_mat4 b) {
    #pragma GCC unroll 4
    #pragma GCC ivdep
    for (int i = 0; i < 4; i++) {
        const float ai0 = A(i, 0);
        const float ai1 = A(i, 1);
        const float ai2 = A(i, 2);
        const float ai3 = A(i, 3);

        R(i, 0) = ai0 * B(0, 0) + ai1 * B(1, 0) + ai2 * B(2, 0) + ai3 * B(3, 0);
        R(i, 1) = ai0 * B(0, 1) + ai1 * B(1, 1) + ai2 * B(2, 1) + ai3 * B(3, 1);
        R(i, 2) = ai0 * B(0, 2) + ai1 * B(1, 2) + ai2 * B(2, 2) + ai3 * B(3, 2);
        R(i, 3) = ai0 * B(0, 3) + ai1 * B(1, 3) + ai2 * B(2, 3) + ai3 * B(3, 3);
    }
}

void bm_mattrans (bm_vec4 result, bm_mat4 a, bm_vec4 v) {
    RV(0) = V(0) * A(0, 0) + V(1) * A(0, 1) + V(2) * A(0, 2) + V(3) * A(0, 3);
    RV(1) = V(0) * A(1, 0) + V(1) * A(1, 1) + V(2) * A(1, 2) + V(3) * A(1, 3);
    RV(2) = V(0) * A(2, 0) + V(1) * A(2, 1) + V(2) * A(2, 2) + V(3) * A(2, 3);
    RV(3) = V(0) * A(3, 0) + V(1) * A(3, 1) + V(2) * A(3, 2) + V(3) * A(3, 3);
}

// See: https://www.geertarien.com/blog/2017/07/30/breakdown-of-the-lookAt-function-in-OpenGL/
void bm_mat_lookat (
    bm_mat4 result,
    float fromX, float fromY, float fromZ,
    float atX, float atY, float atZ,
    float upX, float upY, float upZ
) {
    bm_vec4 at = {atX, atY, atZ, 0.0};
    bm_vec4 up = {upX, upY, upZ, 0.0};

    bm_vec4 zaxis = {fromX - atX, fromY - atY, fromZ - atZ, 0.0};
    bm_vec4 xaxis;
    bm_vec4 yaxis;

    bm_vecnormalize(zaxis);

    bm_veccross(xaxis, zaxis, up);
    bm_vecnormalize(xaxis);

    bm_veccross(yaxis, xaxis, zaxis);

    bm_vecmul(zaxis, -1.0);

    R(0, 0) = xaxis[0];
    R(0, 1) = xaxis[1];
    R(0, 2) = xaxis[2];
    R(0, 3) = -bm_vecdot3(xaxis, at);

    R(1, 0) = yaxis[0];
    R(1, 1) = yaxis[1];
    R(1, 2) = yaxis[2];
    R(1, 3) = -bm_vecdot3(yaxis, at);

    R(2, 0) = zaxis[0];
    R(2, 1) = zaxis[1];
    R(2, 2) = zaxis[2];
    R(2, 3) = -bm_vecdot3(zaxis, at);

    R(3, 0) = 0.0;
    R(3, 1) = 0.0;
    R(3, 2) = 0.0;
    R(3, 3) = 1.0;
}

// See: http://www.songho.ca/opengl/gl_projectionmatrix.html#perspective
void bm_mat_perspective (
    bm_mat4 result,
    float left, float right,
    float top, float bottom,
    float near, float far
    // (float) wherever you are
) {
    R(0, 0) = (2 * near) / (right - left);
    R(0, 1) = 0.0;
    R(0, 2) = (right + left) / (right - left);
    R(0, 3) = 0.0;

    R(1, 0) = 0.0;
    R(1, 1) = (2 * near) / (top - bottom);
    R(1, 2) = (top + bottom) / (top - bottom);
    R(1, 3) = 0.0;

    R(2, 0) = 0.0;
    R(2, 1) = 0.0;
    R(2, 2) = -(far + near) / (far - near);
    R(2, 3) = -(2 * near * far) / (far - near);

    R(3, 0) = 0.0;
    R(3, 1) = 0.0;
    R(3, 2) = -1.0;
    R(3, 3) = 0.0;
}

void bm_mat_perspective_fov (
    bm_mat4 result,
    float aspect, float fov,
    float near, float far
) {
    const float width = 2.0 * near * tanf(fov / 2.0);
    const float height = width * aspect;

    bm_mat_perspective(result,
        -width / 2.0, width / 2.0,
        -height / 2.0, height / 2.0,
        near, far
    );
}
