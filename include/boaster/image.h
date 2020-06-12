#ifndef BOASTER_IMAGE_H_
#define BOASTER_IMAGE_H_

#include <stddef.h>  // size_t

typedef struct {
    size_t __pixel_size;
    size_t width;
    size_t height;
    void* data;
} boaster_image_t;

boaster_image_t *boaster_image_create(size_t width, size_t height);
void* boaster_image_get_pixel(boaster_image_t* image, size_t x, size_t y);
void boaster_image_destroy(boaster_image_t *image);

#endif  // BOASTER_IMAGE_H_