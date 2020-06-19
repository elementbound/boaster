#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include "include/boaster/varray.h"
#include "include/boaster/format.h"
#include "include/boaster/interpolator.h"

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

void boaster_format_init(boaster_format_t *format) {
    boaster_varray_initialize((void**) &(format->properties),
        &(format->property_count));
    format->interpolators = NULL;
}

size_t boaster_format_get_size(boaster_format_t *format) {
    int result = 0;
    for (int i = 0; i < format->property_count; i++) {
        boaster_property_t property = format->properties[i];

        if (property.offset + property.size > result) {
            result = property.offset + property.size;
        }
    }

    return (size_t) result;
}

void boaster_format_clear(boaster_format_t *format) {
    boaster_varray_clear((void**) &(format->properties),
        &(format->property_count));
}

int boaster_format_add_property(boaster_format_t* format,
    const char *name,
    size_t component_size,
    size_t component_count,
    int offset,
    boaster_interpolator_t interpolator) {
    boaster_property_t property;

    property.name = strdup(name);
    property.size = component_count * component_size;

    property.component_size = component_size;
    property.component_count = component_count;

    property.offset = offset;

    boaster_varray_add_struct(&(format->properties), &(format->property_count),
        property);

    format->interpolators = (boaster_interpolator_t*) realloc(
        format->interpolators,
        (format->property_count + 1) * sizeof(boaster_interpolator_t)
    );

    format->interpolators[format->property_count - 1] = interpolator
        ? interpolator
        : boaster_noop_interpolator;

    return format->property_count - 1;
}

int boaster_format_get_property_index(boaster_format_t* format,
    const char* name) {
    for (int i = 0; i < format->property_count; ++i) {
        if (strcmp(format->properties[i].name, name) == 0) {
            return i;
        }
    }

    return -1;
}

boaster_property_t* boaster_format_get_property(
    boaster_format_t* format,
    const char* name) {

    int index = boaster_format_get_property_index(format, name);
    return index >= 0
        ? &(format->properties[index])
        : NULL;
}
