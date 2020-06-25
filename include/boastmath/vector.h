#ifndef BOASTMATH_VECTOR_H_
#define BOASTMATH_VECTOR_H_

#include "./types.h"

#define bm_v(v, i) v[i]
#define bm_spread_vec3(v) v[0], v[1], v[2]
#define bm_spread_vec4(v) v[0], v[1], v[2], v[3]

float bm_vecmul(bm_vec4 v, float f);
float bm_veclen3(bm_vec4 v);
void bm_veccross(bm_vec4 result, bm_vec4 a, bm_vec4 b);
float bm_vecdot3(bm_vec4 a, bm_vec4 b);
void bm_vecnormalize(bm_vec4 v);

#endif  // BOASTMATH_VECTOR_H_
