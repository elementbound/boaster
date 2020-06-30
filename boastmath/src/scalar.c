#include <math.h>
#include "include/boastmath/scalar.h"

float bm_lerp(float a, float b, float f) {
    return a + f * (b - a);
}

float bm_unlerp(float a, float b, float x) {
    return (x - a) / (b - a);
}

float bm_clamp(float min, float max, float x) {
    return fminf(max, fmaxf(min, x));
}
