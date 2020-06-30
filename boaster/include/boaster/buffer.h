#ifndef BOASTER_BUFFER_H_
#define BOASTER_BUFFER_H_

#include <stdlib.h>  // malloc
#include <stddef.h>

extern void* __buffer_push_data;
extern size_t __buffer_push_data_size;

typedef size_t (*buffer_capacity_strategy)(size_t requested, size_t size, \
    size_t capacity);

typedef struct {
    buffer_capacity_strategy __capacity_strategy;
    size_t __capacity;
    size_t __size;
    void* data;
} boaster_buffer_t;

boaster_buffer_t *boaster_buffer_create();
void boaster_buffer_clear(boaster_buffer_t *buffer);
void boaster_buffer_destroy(boaster_buffer_t *buffer);

void boaster_buffer_ensure_capacity(boaster_buffer_t *buffer, size_t capacity);
void boaster_buffer_push_bytes(boaster_buffer_t* buffer, const void* bytes,
    size_t bytes_count);

#define boaster_buffer_push_data(buffer, type, data) { \
    type push_data = data; \
    boaster_buffer_push_bytes(buffer, &push_data, sizeof(type)); \
}

#define boaster_buffer_get_at_index(buffer, type, index) \
    *(type*)(buffer->data + (index * sizeof(type)))

#define boaster_buffer_get_at_offset(buffer, type, offset) \
    *(type*)(buffer->data + offset)

#define boaster_buffer_see_index(buffer, type, index) \
    (type*)(buffer->data + (index * sizeof(type)))

#endif  // BOASTER_BUFFER_H_
