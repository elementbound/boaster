#ifndef BOASTER_BOASTER_H_
#define BOASTER_BOASTER_H_

#include "include/boaster/types.h"

typedef struct boaster_context_t boaster_context_t;

typedef enum {
    /**
     * Simple raster mode, fill pixels on a single thread.
     */
    SIMPLE,

    /**
     * Multithreaded raster mode, fill pixels on multiple threads.
     */
    THREADED
} boaster_raster_mode_t;

typedef struct {
    boaster_raster_mode_t raster_mode;

    // [HINT] Thread count for rendering
    // Set to 0 or smaller to use all available cores
    int num_threads;
} boaster_config_t;

typedef void (*boaster_pixel_callback_t)(boaster_vertex_t* vertices,
    size_t x, size_t y,
    float barycentrics[3],
    boaster_pixel_t *pixel,
    void *custom_data);

typedef enum {
    BOASTER_OK,

    BOASTER_MISSING_VERTEX_SHADER,
    BOASTER_MISSING_PIXEL_SHADER,

    BOASTER_MISSING_VERTEX_BUFFER,
    BOASTER_MISSING_INPUT_FORMAT,
    BOASTER_BAD_INPUT_FORMAT,
    BOASTER_MISSING_TRANSFORM_FORMAT,

    BOASTER_MISSING_IMAGE,

    BOASTER_MISSING_CONTEXT
} boaster_error_t;

boaster_context_t *boaster_context_create(boaster_config_t config);
void boaster_context_destroy(boaster_context_t *context);

void boaster_run_vertex_shader(
    boaster_draw_call_t draw_call,
    size_t vertex_count,
    boaster_buffer_t *out
);

boaster_error_t boaster_validate_draw_call(boaster_draw_call_t draw_call);

boaster_error_t boaster_render(
    boaster_context_t *context,
    boaster_draw_call_t draw_call
);

#endif  // BOASTER_BOASTER_H_
