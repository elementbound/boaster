#include <assert.h>
#include <stdlib.h>
#include "include/boaster/image.h"
#include "include/boaster/types/pixel.h"

boaster_image_t *boaster_image_create(size_t width, size_t height) {
    boaster_image_t *result =
        (boaster_image_t*) malloc(sizeof(boaster_image_t));

    result->__pixel_size = sizeof(boaster_pixel_t);
    result->width = width;
    result->height = height;
    result->data = malloc(width * height * result->__pixel_size);

    return result;
}

void* boaster_image_get_pixel(boaster_image_t* image, size_t x, size_t y) {
    assert(x < image->width);
    assert(y < image->height);

    return image->data + (y * image->height + x) * image->__pixel_size;
}

void boaster_image_destroy(boaster_image_t *image) {
    free(image->data);
    free(image);
}
