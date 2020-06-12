#include <assert.h>
#include "include/boaster/boaster.h"
#include "include/boaster/buffer.h"

void boaster_run_vertex_shader(boaster_buffer_t *vertex_buffer,
    boaster_vertex_shader_t vertex_shader,
    void* uniform_data,
    boaster_buffer_t* out) {

    size_t vertex_count = vertex_buffer->__size / sizeof(boaster_vertex_t);

    for(size_t i = 0; i < vertex_count; i++) {
        void* input_vertex = boaster_buffer_see_index(vertex_buffer,
            boaster_vertex_t, i);

        void* output_vertex = boaster_buffer_see_index(out,
            boaster_vertex_t, i);

        vertex_shader(input_vertex, output_vertex, uniform_data);
    }
}

void boaster_render(boaster_draw_call_t draw_call) {
    assert(draw_call.vertex_buffer != 0);
    assert(draw_call.vertex_shader != 0);

    // Extract fields
    boaster_buffer_t* vertex_buffer = draw_call.vertex_buffer;
    boaster_vertex_shader_t vertex_shader = draw_call.vertex_shader;
    void* uniform_data = draw_call.uniform_data;

    // Allocate memory for vertex shader output
    boaster_buffer_t *vertex_out = boaster_buffer_create();
    boaster_buffer_ensure_capacity(vertex_out, vertex_buffer->__size);

    boaster_run_vertex_shader(vertex_buffer, vertex_shader, uniform_data,
        vertex_out);

    // Cleanup
    boaster_buffer_destroy(vertex_out);
}
