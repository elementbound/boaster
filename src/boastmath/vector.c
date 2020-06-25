#include <math.h>
#include "include/boastmath/vector.h"

float bm_vecmul(bm_vec4 v, float f) {
    v[0] *= f;
    v[1] *= f;
    v[2] *= f;
    v[3] *= f;
}

float bm_veclen3(bm_vec4 v) {
    return sqrtf(
        v[0] * v[0] +
        v[1] * v[1] +
        v[2] * v[2]
    );
}

void bm_veccross(bm_vec4 result, bm_vec4 a, bm_vec4 b) {
    result[0] = a[1] * b[2] - a[2] * b[1];
    result[1] = a[2] * b[0] - a[0] * b[2];
    result[2] = a[0] * b[1] - a[1] * b[0];
}

float bm_vecdot3(bm_vec4 a, bm_vec4 b) {
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

void bm_vecnormalize(bm_vec4 v) {
    float l = bm_veclen3(v);

    if (isgreater(l, 0.005)) {
        v[0] /= l;
        v[1] /= l;
        v[2] /= l;
        v[3] /= l;
    }
}
