#ifndef BOASTGL_BOASTGL_H_
#define BOASTGL_BOASTGL_H_

#include "include/boaster/image.h"

struct boastgl_window_t;

int boastgl_init();
void boastgl_terminate();

struct boastgl_window_t *boastgl_window_create();
void boastgl_window_destroy(struct boastgl_window_t *window);

void boastgl_window_push_image(
    struct boastgl_window_t *window,
    boaster_image_t *image,
    float scale
);

int boastgl_window_is_open(struct boastgl_window_t *window);

#endif  // BOASTGL_BOASTGL_H_
