#include <assert.h>
#include <math.h>
#include "include/boaster/boaster.h"
#include "include/boaster/buffer.h"

void boaster_run_vertex_shader(boaster_buffer_t *vertex_buffer,
    boaster_vertex_shader_t vertex_shader,
    void *uniform_data,
    boaster_buffer_t *out) {

    size_t vertex_count = vertex_buffer->__size / sizeof(boaster_vertex_t);

    for(size_t i = 0; i < vertex_count; i++) {
        void* input_vertex = boaster_buffer_see_index(vertex_buffer,
            boaster_vertex_t, i);

        void* output_vertex = boaster_buffer_see_index(out,
            boaster_vertex_t, i);

        vertex_shader(input_vertex, output_vertex, uniform_data);
    }
}

void boaster_fill_triangle(boaster_vertex_t *vertices, boaster_image_t *image,
    void *custom_data, boaster_pixel_callback_t callback) {

    float barycentrics[] = { 0, 0, 0 };

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
            barycentrics[0] = (dot11 * dot02 - dot01 * dot12) * invDenom;
            barycentrics[1] = (dot00 * dot12 - dot01 * dot02) * invDenom;
            barycentrics[2] = 1.0f - barycentrics[0] - barycentrics[1];

            if(isgreaterequal(barycentrics[0], 0.0)
                && isgreaterequal(barycentrics[1], 0.0)
                && isless(barycentrics[0] + barycentrics[1], 1.005)) {
                boaster_pixel_t* pixel =
                    (boaster_pixel_t*) boaster_image_get_pixel(image, x, y);

                callback(vertices, x, y, barycentrics, pixel, custom_data);
            }
        }
    }
}

void pixel_callback(boaster_vertex_t* vertices,
    size_t x, size_t y,
    float barycentrics[3],
    boaster_pixel_t* pixel,
    void *custom_data) {

    pixel->color[0] = 1;
    pixel->color[1] = 1;
    pixel->color[2] = 1;
    pixel->color[3] = 1;

    boaster_draw_call_t* draw_call =
        (boaster_draw_call_t*) custom_data;

    draw_call->pixel_shader(vertices, barycentrics, pixel,
        draw_call->uniform_data);
}

void boaster_render(boaster_draw_call_t draw_call) {
    assert(draw_call.vertex_buffer != NULL);
    assert(draw_call.vertex_shader != NULL);

    // Extract fields
    boaster_buffer_t* vertex_buffer = draw_call.vertex_buffer;
    boaster_vertex_shader_t vertex_shader = draw_call.vertex_shader;
    boaster_pixel_shader_t pixel_shader = draw_call.pixel_shader;
    void* uniform_data = draw_call.uniform_data;
    boaster_image_t* image = draw_call.target_image;

    // Vertices must come in multiples of three, forming triangles
    size_t vertex_count = vertex_buffer->__size / sizeof(boaster_vertex_t);
    assert(vertex_count % 3 == 0);

    // Allocate memory for vertex shader output
    boaster_buffer_t *vertex_out = boaster_buffer_create();
    boaster_buffer_ensure_capacity(vertex_out, vertex_buffer->__size);

    boaster_run_vertex_shader(vertex_buffer, vertex_shader, uniform_data,
        vertex_out);

    // Rasterize output triangles
    for (size_t i = 0; i < vertex_count; i += 3) {
        boaster_vertex_t *vertices =
            boaster_buffer_see_index(vertex_out, boaster_vertex_t, i);

        boaster_fill_triangle(vertices, image, (void*) &draw_call,
            pixel_callback);
    }

    // Cleanup
    boaster_buffer_destroy(vertex_out);
}
