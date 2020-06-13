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

void boaster_fill_triangle(boaster_vertex_t *vertices, boaster_image_t *image) {
    // Thanks Black Pawn!
    // https://blackpawn.com/texts/pointinpoly/default.html
    float Ax = vertices[0].position[0];
    float Ay = vertices[0].position[1];
    float Az = vertices[0].position[2];

    float Bx = vertices[1].position[0];
    float By = vertices[1].position[1];
    float Bz = vertices[1].position[2];

    float Cx = vertices[2].position[0];
    float Cy = vertices[2].position[1];
    float Cz = vertices[2].position[2];

    float v0x = Cx - Ax;
    float v0y = Cy - Ay;
    float v0z = Cz - Az;

    float v1x = Bx - Ax;
    float v1y = By - Ay;
    float v1z = Bz - Az;

    float dot00 = v0x * v0x + v0y * v0y + v0z * v0z;
    float dot01 = v0x * v1x + v0y * v1y + v0z * v1z;
    float dot11 = v1x * v1x + v1y * v1y + v1z * v1z;

    for(size_t y = 0; y < image->height; y++) {
        for(size_t x = 0; x < image->width; x++) {
            float Px = x / (float)image->width;
            float Py = y / (float)image->height;
            float Pz = 0.5f;

            float v2x = Px - Ax;
            float v2y = Py - Ay;
            float v2z = Pz - Az;

            float dot02 = v0x * v2x + v0y * v2y + v0z * v2z;
            float dot12 = v1x * v2x + v1y * v2y + v1z * v2z;

            float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
            float u = (dot11 * dot02 - dot01 * dot12) * invDenom;
            float v = (dot00 * dot12 - dot01 * dot02) * invDenom;

            boaster_pixel_t* pixel =
                (boaster_pixel_t*) boaster_image_get_pixel(image, x, y);

            if(u >= 0.f && v >= 0.f && u + v < 1.f) {
                pixel->color[0] = 1.f;
                pixel->color[1] = 1.f;
                pixel->color[2] = 1.f;
                pixel->color[3] = 1.f;
            } else {
                pixel->color[0] = 0.f;
                pixel->color[1] = 0.f;
                pixel->color[2] = 0.f;
                pixel->color[3] = 1.f;
            }
        }
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
