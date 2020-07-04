#ifndef BOASTGL_BOASTGL_H_
#define BOASTGL_BOASTGL_H_

#include "include/boaster/image.h"

typedef struct boastgl_window_t boastgl_window_t;

typedef void (*boastgl_codec_t)(
    void *in_pixel,
    uint8_t *out_pixel,
    boaster_format_t *image_format
);

void boastgl_float3_codec(
    void *in_pixel,
    uint8_t *out_pixel,
    boaster_format_t *image_format
);

void boastgl_byte3_codec(
    void *in_pixel,
    uint8_t *out_pixel,
    boaster_format_t *image_format
);

int boastgl_init();
void boastgl_terminate();

boastgl_window_t *boastgl_window_create();
void boastgl_window_destroy(boastgl_window_t *window);

void boastgl_window_push_image(
    boastgl_window_t *window,
    boaster_image_t *image,
    boastgl_codec_t codec,
    float scale
);

int boastgl_window_is_open(boastgl_window_t *window);

#endif  // BOASTGL_BOASTGL_H_
