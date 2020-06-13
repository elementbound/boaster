#include <assert.h>
#include <stdlib.h>
#include <string.h>
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

    return image->data + ((y * image->width) + x) * image->__pixel_size;
}

void boaster_image_fill(boaster_image_t* image, void* pixel_bytes,
    size_t pixel_size) {

    assert(pixel_size == (image->__pixel_size));

    size_t buffer_size = image->width * image->height * image->__pixel_size;
    for(size_t i = 0; i < buffer_size; i += pixel_size) {
        memcpy(image->data + i, pixel_bytes, pixel_size);
    }
}

void boaster_image_destroy(boaster_image_t *image) {
    free(image->data);
    free(image);
}
