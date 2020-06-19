#include "include/boaster/property.h"

void* boaster_property_get_pointer(
    boaster_property_t* property,
    void *vertex) {

    return (vertex + property->offset);
}

float* boaster_property_get_floats(
    boaster_property_t* property,
    void *vertex) {

    return (float*) boaster_property_get_pointer(property, vertex);
}
