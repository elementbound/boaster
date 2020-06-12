#ifndef BOASTER_TYPES_PIXEL_H_
#define BOASTER_TYPES_PIXEL_H_

#ifndef boaster_byte_t
    #define byte_t unsigned char
#endif

typedef struct {
    byte_t color[4];
} boaster_pixel_t;

#endif  // BOASTER_TYPES_PIXEL_H_
