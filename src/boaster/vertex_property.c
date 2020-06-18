#include "include/boaster/vertex_property.h"

void* boaster_vertex_property_get_pointer(
    boaster_vertex_property_t* property,
    void *vertex) {

    return (vertex + property->offset);
}

float* boaster_vertex_property_get_floats(
    boaster_vertex_property_t* property,
    void *vertex) {

    return (float*) boaster_vertex_property_get_pointer(property, vertex);
}
