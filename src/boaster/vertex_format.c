#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "include/boaster/varray.h"
#include "include/boaster/vertex_format.h"

/**
 * Duplicate string and return its pointer.
 * 
 * The resulting buffer is owned by the caller.
 */
char *strdup(const char *src) {
    int length = strlen(src);
    char *result = (char*) malloc(length + 1);
    memcpy(result, src, length + 1);

    return result;
}

void boaster_vertex_format_init(boaster_vertex_format_t *format) {
    boaster_varray_initialize((void**) &(format->properties),
        &(format->property_count));
}

size_t boaster_vertex_format_get_size(boaster_vertex_format_t *format) {
    int result = 0;
    for (int i = 0; i < format->property_count; i++) {
        boaster_vertex_property_t property = format->properties[i];

        if (property.offset + property.size > result) {
            result = property.offset + property.size;
        }
    }

    return (size_t) result;
}

void boaster_vertex_format_clear(boaster_vertex_format_t *format) {
    boaster_varray_clear((void**) &(format->properties),
        &(format->property_count));
}

int boaster_vertex_format_add_property(boaster_vertex_format_t* format,
    const char *name,
    size_t component_size,
    size_t component_count,
    int offset) {
    boaster_vertex_property_t property;

    property.name = strdup(name);
    property.size = component_count * component_size;

    property.component_size = component_size;
    property.component_count = component_count;

    property.offset = offset;

    boaster_varray_add_struct(&(format->properties), &(format->property_count),
        property);

    return format->property_count - 1;
}

int boaster_vertex_format_get_property_index(boaster_vertex_format_t* format,
    const char* name) {
    for (int i = 0; i < format->property_count; ++i) {
        if (strcmp(format->properties[i].name, name) == 0) {
            return i;
        }
    }

    return -1;
}

float* boaster_vertex_format_get_floats(boaster_vertex_format_t* format,
    void *vertex,
    int property_index) {
    assert(property_index >= 0);
    assert(property_index < format->property_count);
    boaster_vertex_property_t property = format->properties[property_index];

    return (float*)(vertex + property.offset);
}

float boaster_vertex_format_get_float(boaster_vertex_format_t* format,
    void *vertex,
    int property_index,
    int component_index) {

    float* components =
        boaster_vertex_format_get_floats(format, vertex, property_index);

    // NOTE: Assert for component index?

    return components[component_index];
}
