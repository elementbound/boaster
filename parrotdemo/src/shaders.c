#include <assert.h>
#include <math.h>
#include <stdint.h>
#include "include/shaders.h"
#include "include/boastmath/matrix.h"
#include "include/boastmath/scalar.h"
#include "include/boastmath/vector.h"
#include "include/vertex.h"

float fclamp(float x, float min, float max) {
    return fmaxf(fminf(x, max), min);
}

float fsaturate(float x) {
    return fclamp(x, 0.0, 1.0);
}

void parrot_vertex_shader(
    void *v_vertex,
    void *v_out,
    void *v_uniforms,
    boaster_format_t *in_format,
    boaster_format_t *out_format
) {
    parrot_vertex_t *vertex = (parrot_vertex_t*) v_vertex;
    parrot_color_vertex_t *out = (parrot_color_vertex_t*) v_out;
    parrot_uniforms_t *uniforms = (parrot_uniforms_t*) v_uniforms;

    // Look up the same vertex in the next frame
    parrot_vertex_t *next_vertex =
        uniforms->next_mesh->data + vertex->idx * in_format->size;
    float f = uniforms->transition_factor;

    if (vertex->idx != next_vertex->idx) {
        // printf("[vsh] Vertex pair failed: %d %d\n",
        //    vertex->idx, next_vertex->idx);
        f = 0.0;
    }

    bm_vec4 pos = {
        bm_lerp(vertex->position[0], next_vertex->position[0], f),
        bm_lerp(vertex->position[1], next_vertex->position[1], f),
        bm_lerp(vertex->position[2], next_vertex->position[2], f),
        1.0
    };

    float dot = (1.0 + bm_vecdot3(vertex->normal, uniforms->view_axis)) / 2.0;

    bm_mattrans(out->position, uniforms->mat_VP, pos);

    out->color[0] = dot * uniforms->color[0];
    out->color[1] = dot * uniforms->color[1];
    out->color[2] = dot * uniforms->color[2];

    if (vertex->idx != next_vertex->idx) {
        out->color[0] = 1.0;
        out->color[1] = 0.0;
        out->color[2] = 0.0;
    }
}

void parrot_pixel_shader(
    void *v_vertex,
    void *v_out,
    void *v_uniforms,
    boaster_format_t *format
) {
    parrot_color_vertex_t *vertex = (parrot_color_vertex_t*) v_vertex;
    uint8_t *out = (uint8_t*) v_out;
    uint32_t *depth = (uint32_t*) (v_out + 4);

    out[0] = fsaturate(vertex->color[0]) * 255;
    out[1] = fsaturate(vertex->color[1]) * 255;
    out[2] = fsaturate(vertex->color[2]) * 255;
    out[3] = 255;
}
