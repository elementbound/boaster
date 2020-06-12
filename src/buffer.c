#include "include/buffer.h"

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

buffer_t *buffer_create() {
    buffer_t *result = (buffer_t*) malloc(sizeof(buffer_t));

    result->__capacity_strategy = __default_capacity_strategy;
    result->__capacity = 0;
    result->__size = 0;
    result->data = NULL;

    return result;
}

void buffer_destroy(buffer_t *buffer) {
    free(buffer);
}

void buffer_ensure_capacity(buffer_t *buffer, size_t capacity) {
    if (buffer->__capacity >= capacity)
        return;

    size_t new_capacity = buffer->__capacity_strategy(capacity, \
        buffer->__size, buffer->__capacity);

    buffer->data = realloc(buffer->data, new_capacity);
    buffer->__capacity = new_capacity;
}

void buffer_push_bytes(buffer_t* buffer, \
    const void* bytes, size_t bytes_count) {

    buffer_ensure_capacity(buffer, buffer->__size + bytes_count);

    memcpy(buffer->data + buffer->__size, bytes, bytes_count);
    buffer->__size += bytes_count;
}
