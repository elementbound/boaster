#ifndef BOASTMATH_SCALAR_H_
#define BOASTMATH_SCALAR_H_

#include <math.h>

float bm_lerp(float a, float b, float f);
float bm_unlerp(float a, float b, float x);
float bm_clamp(float min, float max, float x);

#define bm_rad(deg) ((deg) / 180.0 * M_PI)
#define bm_deg(rad) ((rad) * 180.0 / M_PI)

#endif  // BOASTMATH_SCALAR_H_
