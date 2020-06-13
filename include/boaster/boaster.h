#ifndef BOASTER_BOASTER_H_
#define BOASTER_BOASTER_H_

#include "include/boaster/types.h"

typedef void (*boaster_pixel_callback_t)(boaster_vertex_t* vertices,
    size_t x, size_t y,
    float barycentrics[3],
    boaster_pixel_t *pixel,
    void *custom_data);

void boaster_run_vertex_shader(boaster_buffer_t *vertex_buffer,
    boaster_vertex_shader_t vertex_shader,
    void* uniform_data,
    boaster_buffer_t *out);

void boaster_fill_triangle(boaster_vertex_t *vertices, boaster_image_t *image,
    void *custom_data, boaster_pixel_callback_t callback);

void boaster_render(boaster_draw_call_t draw_call);

#endif  // BOASTER_BOASTER_H_
