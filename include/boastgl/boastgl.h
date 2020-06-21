#ifndef BOASTGL_BOASTGL_H_
#define BOASTGL_BOASTGL_H_

#include "include/boaster/image.h"

typedef struct boastgl_window_t boastgl_window_t;

int boastgl_init();
void boastgl_terminate();

boastgl_window_t *boastgl_window_create();
void boastgl_window_destroy(boastgl_window_t *window);

void boastgl_window_push_image(
    boastgl_window_t *window,
    boaster_image_t *image,
    float scale
);

int boastgl_window_is_open(boastgl_window_t *window);

#endif  // BOASTGL_BOASTGL_H_
