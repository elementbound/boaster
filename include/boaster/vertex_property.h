#ifndef BOASTER_VERTEX_PROPERTY_H_
#define BOASTER_VERTEX_PROPERTY_H_

#include <stddef.h>  // size_t

typedef struct {
    char* name;
    size_t size;

    size_t component_size;
    size_t component_count;

    int offset;
} boaster_vertex_property_t;

void* boaster_vertex_property_get_pointer(
    boaster_vertex_property_t* property,
    void *vertex
);

float* boaster_vertex_property_get_floats(
    boaster_vertex_property_t* property,
    void *vertex
);

#endif  // BOASTER_VERTEX_PROPERTY_H_
