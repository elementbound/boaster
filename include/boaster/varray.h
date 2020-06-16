#ifndef BOASTER_VARRAY_H_
#define BOASTER_VARRAY_H_

#include <stddef.h>

void boaster_varray_initialize(void **data, size_t *count);
void boaster_varray_add_bytes(void **data, size_t *count, void *bytes,
    size_t item_size);
void boaster_varray_remove_bytes(void **data, size_t *count, size_t index,
    size_t item_size);
void boaster_varray_clear(void **data, size_t *count);

#define boaster_varray_add_struct(data, count, item) \
    boaster_varray_add_bytes(data, count, &item, sizeof(item))

#define boaster_varray_remove_struct(data, count, index, type) \
    boaster_varray_remove_bytes(data, count, index, sizeof(type))

void boaster_varray_add_char(void **data, size_t *count, char value);
void boaster_varray_add_uchar(void **data, size_t *count, unsigned char value);
void boaster_varray_add_short(void **data, size_t *count, short value);
void boaster_varray_add_ushort(void **data, size_t *count, unsigned short value);
void boaster_varray_add_int(void **data, size_t *count, int value);
void boaster_varray_add_uint(void **data, size_t *count, unsigned int value);
void boaster_varray_add_long(void **data, size_t *count, long value);
void boaster_varray_add_ulong(void **data, size_t *count, unsigned long value);
void boaster_varray_add_float(void **data, size_t *count, float value);
void boaster_varray_add_double(void **data, size_t *count, double value);
void boaster_varray_add_pointer(void **data, size_t *count, void *value);

void boaster_varray_remove_char(void **data, size_t *count, size_t index);
void boaster_varray_remove_uchar(void **data, size_t *count, size_t index);
void boaster_varray_remove_short(void **data, size_t *count, size_t index);
void boaster_varray_remove_ushort(void **data, size_t *count, size_t index);
void boaster_varray_remove_int(void **data, size_t *count, size_t index);
void boaster_varray_remove_uint(void **data, size_t *count, size_t index);
void boaster_varray_remove_long(void **data, size_t *count, size_t index);
void boaster_varray_remove_ulong(void **data, size_t *count, size_t index);
void boaster_varray_remove_float(void **data, size_t *count, size_t index);
void boaster_varray_remove_double(void **data, size_t *count, size_t index);
void boaster_varray_remove_pointer(void **data, size_t *count, size_t index);

#endif  // BOASTER_VARRAY_H_
