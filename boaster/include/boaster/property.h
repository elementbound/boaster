#ifndef BOASTER_PROPERTY_H_
#define BOASTER_PROPERTY_H_

#include <stddef.h>  // size_t

typedef struct {
    char* name;
    size_t size;

    size_t component_size;
    size_t component_count;

    int offset;
} boaster_property_t;

void* boaster_property_get_pointer(
    boaster_property_t* property,
    void *vertex
);

float* boaster_property_get_floats(
    boaster_property_t* property,
    void *vertex
);

#endif  // BOASTER_PROPERTY_H_
