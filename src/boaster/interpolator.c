#include <string.h>
#include "include/boaster/interpolator.h"

void boaster_noop_interpolator(
    void* input_vertices,
    float barycentrics[3],
    void* output,
    size_t vertex_size,
    boaster_property_t* property
) {
    void *src =
        boaster_property_get_pointer(property, input_vertices);
    void *dst =
        boaster_property_get_pointer(property, output);

    memcpy(dst, src, property->size);
}

void boaster_floats_interpolator(
    void* input_vertices,
    float barycentrics[3],
    void* output,
    size_t vertex_size,
    boaster_property_t* property
) {
    float *data[] = {
        boaster_property_get_floats(property,
            input_vertices + vertex_size * 0),
        boaster_property_get_floats(property,
            input_vertices + vertex_size * 1),
        boaster_property_get_floats(property,
            input_vertices + vertex_size * 2),
    };

    float* out = boaster_property_get_floats(property, output);

    for (int i = 0; i < property->component_count; i++) {
        float v0 = data[2][i] - data[0][i];
        float v1 = data[1][i] - data[0][i];

        out[i] = data[0][i] + barycentrics[0] * v0 + barycentrics[1] * v1;
    }
}
