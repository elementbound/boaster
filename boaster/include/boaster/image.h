#ifndef BOASTER_IMAGE_H_
#define BOASTER_IMAGE_H_

#include <stddef.h>  // size_t
#include "./format.h"

typedef struct {
    boaster_format_t *format;
    size_t width;
    size_t height;
    void* data;
} boaster_image_t;

boaster_image_t *boaster_image_create (
    boaster_format_t *format,
    size_t width,
    size_t height
);

void* boaster_image_get_pixel(boaster_image_t *image, size_t x, size_t y);
void boaster_image_fill(boaster_image_t *image, void *pixel_bytes,
    size_t pixel_size);
void boaster_image_destroy(boaster_image_t *image);

#endif  // BOASTER_IMAGE_H_
