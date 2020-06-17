#ifndef BOASTER_VERTEX_FORMAT_H_
#define BOASTER_VERTEX_FORMAT_H_

#include <stddef.h>  // size_t

typedef struct {
    char* name;
    size_t size;

    size_t component_size;
    size_t component_count;

    int offset;
} boaster_vertex_property_t;

typedef struct {
    boaster_vertex_property_t *properties;
    size_t property_count;
} boaster_vertex_format_t;

void boaster_vertex_format_init(boaster_vertex_format_t *format);
size_t boaster_vertex_format_get_size(boaster_vertex_format_t *format);
void boaster_vertex_format_clear(boaster_vertex_format_t *format);

int boaster_vertex_format_add_property(boaster_vertex_format_t* format,
    const char *name,
    size_t component_size,
    size_t component_count,
    int offset);

int boaster_vertex_format_get_property_index(boaster_vertex_format_t* format,
    const char* name);

float boaster_vertex_format_get_float(boaster_vertex_format_t* format,
    void *vertex,
    int property_index,
    int component_index);

float* boaster_vertex_format_get_floats(boaster_vertex_format_t* format,
    void *vertex,
    int property_index);

#endif  // BOASTER_VERTEX_FORMAT_H_
