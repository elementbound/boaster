#ifndef PARROTDEMO_INCLUDE_VERTEX_H_
#define PARROTDEMO_INCLUDE_VERTEX_H_

#include <stdint.h>
#include "include/boaster/format.h"

typedef struct {
    float position[4];
    float normal[3];
    uint32_t idx;
} parrot_vertex_t;

typedef struct {
    float position[4];
    float color[3];
} parrot_color_vertex_t;

void parrot_init_vertex_format(boaster_format_t *format);
void parrot_init_transform_format(boaster_format_t *format);

#endif  // PARROTDEMO_INCLUDE_VERTEX_H_
