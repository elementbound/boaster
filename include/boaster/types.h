#ifndef BOASTER_TYPES_H_
#define BOASTER_TYPES_H_

#include "./types/vertex.h"
#include "./types/pixel.h"
#include "./types/shaders.h"
#include "./image.h"
#include "include/buffer.h"

#ifndef boaster_byte_t
    #define byte_t unsigned char
#endif

typedef struct {
    boaster_vertex_shader_t vertex_shader;
    boaster_pixel_shader_t pixel_shader;

    void* uniform_data;
    buffer_t* vertex_buffer;
    boaster_image_t* target_image;
} boaster_draw_call_t;

#endif  // BOASTER_TYPES_H_
