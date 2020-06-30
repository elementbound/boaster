#include <stdlib.h>
#include <string.h>
#include "include/boaster/varray.h"

void boaster_varray_initialize(void **data, size_t *count) {
    *data = 0;
    *count = 0;
}

void boaster_varray_add_bytes(void **data, size_t *count, void *bytes,
    size_t item_size) {
    *data = realloc(*data, item_size * ((*count) + 1));

    memcpy(*data + (item_size * (*count)), bytes, item_size);
    *count += 1;
}

void boaster_varray_remove_bytes(void **data, size_t *count, size_t index,
    size_t item_size) {
    
    void *item_pointer = *data + (index * item_size);
    void *last_pointer = *data + (((*count) - 1) * item_size);

    memcpy(item_pointer, last_pointer, item_size);
    *data = realloc(*data, ((*count) - 1) * item_size);
    *count -= 1;
}

void boaster_varray_clear(void **data, size_t *count) {
    free(*data);
    *data = 0;
    *count = 0;
}

void boaster_varray_add_char(void **data, size_t *count, char value) {
    *data = realloc(*data, sizeof(char) * ((*count) + 1));
    *count += 1;

    char *array = (char*) (*data);
    array[(*count) - 1] = value;
}

#define VARRAY_IMPLEMENT_ADD(type, suffix) \
void boaster_varray_add_ ## suffix (void **data, size_t *count, type value) { \
    *data = realloc(*data, sizeof(type) * ((*count) + 1)); \
    *count += 1; \
 \
    type *array = (type*) (*data); \
    array[(*count) - 1] = value; \
}

VARRAY_IMPLEMENT_ADD(unsigned char, uchar)
VARRAY_IMPLEMENT_ADD(short, short)
VARRAY_IMPLEMENT_ADD(unsigned short, ushort)
VARRAY_IMPLEMENT_ADD(int, int)
VARRAY_IMPLEMENT_ADD(unsigned int, uint)
VARRAY_IMPLEMENT_ADD(long, long)
VARRAY_IMPLEMENT_ADD(unsigned long, ulong)
VARRAY_IMPLEMENT_ADD(float, float)
VARRAY_IMPLEMENT_ADD(double, double)
VARRAY_IMPLEMENT_ADD(void *, pointer)

void boaster_varray_remove_char(void **data, size_t *count, size_t index) {
    char *array = (char*) (*data);
    array[index] = array[(*count) - 1];

    *data = realloc(*data, sizeof(char) * ((*count) - 1));
    *count -= 1;
}

#define VARRAY_IMPLEMENT_REMOVE(type, suffix) \
void boaster_varray_remove_ ## suffix(void **data, size_t *count, size_t index) { \
    type *array = (type*) (*data); \
    array[index] = array[(*count) - 1]; \
 \
    *data = realloc(*data, sizeof(type) * ((*count) - 1)); \
    *count -= 1; \
} \

VARRAY_IMPLEMENT_REMOVE(unsigned char, uchar)
VARRAY_IMPLEMENT_REMOVE(short, short)
VARRAY_IMPLEMENT_REMOVE(unsigned short, ushort)
VARRAY_IMPLEMENT_REMOVE(int, int)
VARRAY_IMPLEMENT_REMOVE(unsigned int, uint)
VARRAY_IMPLEMENT_REMOVE(long, long)
VARRAY_IMPLEMENT_REMOVE(unsigned long, ulong)
VARRAY_IMPLEMENT_REMOVE(float, float)
VARRAY_IMPLEMENT_REMOVE(double, double)
VARRAY_IMPLEMENT_REMOVE(void *, pointer)
