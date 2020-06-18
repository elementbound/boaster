#ifndef BOASTER_VERTEX_FORMAT_H_
#define BOASTER_VERTEX_FORMAT_H_

#include <stddef.h>  // size_t
#include "./vertex_property.h"
#include "./interpolator.h"

typedef struct {
    boaster_vertex_property_t *properties;
    boaster_interpolator_t *interpolators;
    size_t property_count;
} boaster_vertex_format_t;

void boaster_vertex_format_init(boaster_vertex_format_t *format);
size_t boaster_vertex_format_get_size(boaster_vertex_format_t *format);
void boaster_vertex_format_clear(boaster_vertex_format_t *format);

int boaster_vertex_format_add_property(boaster_vertex_format_t* format,
    const char *name,
    size_t component_size,
    size_t component_count,
    int offset,
    boaster_interpolator_t interpolator);

int boaster_vertex_format_get_property_index(boaster_vertex_format_t* format,
    const char* name);
boaster_vertex_property_t* boaster_vertex_format_get_property(
    boaster_vertex_format_t* format,
    const char* name);

#endif  // BOASTER_VERTEX_FORMAT_H_
