#ifndef PARROTDEMO_INCLUDE_SHADERS_H_
#define PARROTDEMO_INCLUDE_SHADERS_H_

#include "include/boaster/buffer.h"
#include "include/boaster/format.h"
#include "include/boastmath/matrix.h"
#include "include/vertex.h"

typedef struct {
    float color[3];
    bm_mat4 mat_VP;
    bm_vec4 view_axis;
    boaster_buffer_t *next_mesh;
    float transition_factor;
} parrot_uniforms_t;

void parrot_vertex_shader(
    void *v_vertex,
    void *v_out,
    void *v_uniforms,
    boaster_format_t *in_format,
    boaster_format_t *out_format
);

void parrot_pixel_shader(
    void *v_vertex,
    void *v_out,
    void *v_uniforms,
    boaster_format_t *format
);

#endif  // PARROTDEMO_INCLUDE_SHADERS_H_
