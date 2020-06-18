#ifndef BOASTER_TYPES_SHADERS_H_
#define BOASTER_TYPES_SHADERS_H_

#include "include/boaster/vertex_format.h"

typedef void (*boaster_vertex_shader_t)(
    void *vertex,
    void *out,
    void *uniforms,
    boaster_vertex_format_t *in_format,
    boaster_vertex_format_t *out_format);

typedef void (*boaster_pixel_shader_t)(
    void *vertex,
    void *out,
    void *uniforms,
    boaster_vertex_format_t *format);

#endif  // BOASTER_TYPES_SHADERS_H_
