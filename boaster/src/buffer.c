#include "include/boaster/buffer.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DEFAULT_CAPACITY_MULTIPLIER 1.41421356

void* __buffer_push_data;
size_t __buffer_push_data_size;

size_t __default_capacity_strategy(size_t requested, size_t size, \
    size_t capacity) {

    size_t result = capacity;

    while (result < requested) {
        result = (size_t) (result * DEFAULT_CAPACITY_MULTIPLIER) + 1;
    }

    return result;
}

boaster_buffer_t *boaster_buffer_create() {
    boaster_buffer_t *result =
        (boaster_buffer_t*) malloc(sizeof(boaster_buffer_t));

    result->__capacity_strategy = __default_capacity_strategy;
    result->__capacity = 0;
    result->__size = 0;
    result->data = NULL;

    return result;
}

void boaster_buffer_destroy(boaster_buffer_t *buffer) {
    free(buffer->data);
    free(buffer);
}

void boaster_buffer_ensure_capacity(boaster_buffer_t *buffer, size_t capacity) {
    if (buffer->__capacity >= capacity)
        return;

    size_t new_capacity = buffer->__capacity_strategy(capacity, \
        buffer->__size, buffer->__capacity);

    buffer->data = realloc(buffer->data, new_capacity);
    buffer->__capacity = new_capacity;
}

void boaster_buffer_push_bytes(boaster_buffer_t* buffer, \
    const void* bytes, size_t bytes_count) {

    boaster_buffer_ensure_capacity(buffer, buffer->__size + bytes_count);

    memcpy(buffer->data + buffer->__size, bytes, bytes_count);
    buffer->__size += bytes_count;
}
