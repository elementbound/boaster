#ifndef BOASTER_INTERPOLATOR_H_
#define BOASTER_INTERPOLATOR_H_

#include "./vertex_property.h"

typedef void (*boaster_interpolator_t)(
    void* input_vertices,
    float barycentrics[3],
    void* output,
    size_t vertex_size,
    boaster_vertex_property_t* property
);

void boaster_noop_interpolator(
    void* input_vertices,
    float barycentrics[3],
    void* output,
    size_t vertex_size,
    boaster_vertex_property_t* property
);

void boaster_floats_interpolator(
    void* input_vertices,
    float barycentrics[3],
    void* output,
    size_t vertex_size,
    boaster_vertex_property_t* property
);

#endif  // BOASTER_INTERPOLATOR_H_
