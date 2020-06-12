#ifndef BUFFER_H_
#define BUFFER_H_

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
} buffer_t;

buffer_t *buffer_create();
void buffer_destroy(buffer_t*);

void buffer_ensure_capacity(buffer_t *buffer, size_t capacity);
void buffer_push_bytes(buffer_t* buffer, const void* bytes, size_t bytes_count);

#define buffer_push_data(buffer, type, data) { \
    type push_data = data; \
    buffer_push_bytes(buffer, &push_data, sizeof(type)); \
}

#define buffer_get_at_index(buffer, type, index) \
    *(type*)(buffer->data + (index * sizeof(type)))

#define buffer_get_at_offset(buffer, type, offset) \
    *(type*)(buffer->data + offset)

#endif  // BUFFER_H_
