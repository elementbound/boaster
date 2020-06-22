#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "include/boaster/boaster.h"
#include "include/boaster/buffer.h"
#include "include/boaster/executor.h"

typedef struct {
    boaster_draw_call_t *draw_call;
    void *vertex_data;
} boaster_pixel_context_t;

typedef void(*boaster_rasterizer_t)(
    boaster_buffer_t *vertex_buffer,
    boaster_format_t *vertex_format,
    boaster_image_t *image,
    boaster_pixel_callback_t callback,
    void *callback_arg);

struct boaster_context_t {
    boaster_rasterizer_t raster_function;
};

void boaster_run_vertex_shader(boaster_draw_call_t draw_call,
    size_t vertex_count,
    boaster_buffer_t *out) {

    // Extract fields
    boaster_buffer_t* vertex_buffer = draw_call.vertex_buffer;
    boaster_format_t* input_format = draw_call.input_format;
    boaster_format_t* transform_format = draw_call.transform_format;
    boaster_vertex_shader_t vertex_shader = draw_call.vertex_shader;
    void* uniform_data = draw_call.uniform_data;
    boaster_image_t* image = draw_call.target_image;

    for(size_t i = 0; i < vertex_count; i++) {
        void* input_vertex = boaster_buffer_see_index(vertex_buffer,
            boaster_vertex_t, i);

        void* output_vertex = boaster_buffer_see_index(out,
            boaster_vertex_t, i);

        vertex_shader(input_vertex, output_vertex, uniform_data,
            input_format, transform_format);
    }
}

int boaster_check_point_in_triangle(
    float Ax, float Ay, float Az,
    float Bx, float By, float Bz,
    float Cx, float Cy, float Cz,
    float Px, float Py, float Pz,
    float threshold,
    float *barycentrics
) {
    // Thanks Black Pawn!
    // https://blackpawn.com/texts/pointinpoly/default.html

    float v0x = Cx - Ax;
    float v0y = Cy - Ay;
    float v0z = Cz - Az;

    float v1x = Bx - Ax;
    float v1y = By - Ay;
    float v1z = Bz - Az;

    float v2x = Px - Ax;
    float v2y = Py - Ay;
    float v2z = Pz - Az;

    float dot00 = v0x * v0x + v0y * v0y + v0z * v0z;
    float dot01 = v0x * v1x + v0y * v1y + v0z * v1z;
    float dot02 = v0x * v2x + v0y * v2y + v0z * v2z;
    float dot11 = v1x * v1x + v1y * v1y + v1z * v1z;
    float dot12 = v1x * v2x + v1y * v2y + v1z * v2z;

    float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
    barycentrics[0] = (dot11 * dot02 - dot01 * dot12) * invDenom;
    barycentrics[1] = (dot00 * dot12 - dot01 * dot02) * invDenom;
    barycentrics[2] = 1.0f - barycentrics[0] - barycentrics[1];

    return (isgreaterequal(barycentrics[0], 0.0)
        && isgreaterequal(barycentrics[1], 0.0)
        && isless(barycentrics[0] + barycentrics[1], 1.0 + threshold));
}

int boaster_point_in_vbuf_triangle(
    void *buffer, 
    int x, int y, int i,
    float *barycentrics,
    boaster_image_t *image) {

    boaster_vertex_t *vertices =
        &((boaster_vertex_t*) buffer)[i];

    float Ax = vertices[0].position[0];
    float Ay = vertices[0].position[1];
    float Az = vertices[0].position[2];

    float Bx = vertices[1].position[0];
    float By = vertices[1].position[1];
    float Bz = vertices[1].position[2];

    float Cx = vertices[2].position[0];
    float Cy = vertices[2].position[1];
    float Cz = vertices[2].position[2];

    float Px = x / (float)image->width;
    float Py = y / (float)image->height;
    float Pz = 0.5f;

    return boaster_check_point_in_triangle(
        Ax, Ay, Az, Bx, By, Bz, Cx, Cy, Cz,
        Px, Py, Pz,
        0.005, barycentrics
    );
}

void boaster_single_raster(
    boaster_buffer_t *vertex_buffer,
    boaster_format_t *vertex_format,
    boaster_image_t *image,
    boaster_pixel_callback_t callback,
    void *callback_arg) {

    int vertex_size = vertex_format->size;
    int vertex_count = vertex_buffer->__size / vertex_size;

    if (0) {
    printf("Vertex count: %d; size: %d; buffer: %d bytes\n",
        vertex_count, vertex_size, vertex_buffer->__size);
    }

    for (int y = 0; y < image->height; ++y) {
        for (int x = 0; x < image->width; ++x) {
            for (int i = 0; i < vertex_count; i += 3) {

                float barycentrics[] = { 0, 0, 0 };

                boaster_vertex_t *vertices =
                    &((boaster_vertex_t*) vertex_buffer->data)[i];

                if(boaster_point_in_vbuf_triangle(
                    vertex_buffer->data,
                    x, y, i,
                    barycentrics,
                    image
                )) {
                    boaster_pixel_t* pixel =
                        (boaster_pixel_t*) boaster_image_get_pixel(image, x, y);

                    callback(vertices, x, y, barycentrics, pixel, callback_arg);
                }
            }
        }
    }
}

typedef struct {
    int x;
    int y;
    int width;
    int height;

    boaster_buffer_t *vertex_buffer;
    boaster_format_t *vertex_format;
    boaster_image_t *image;
    boaster_pixel_callback_t callback;
    void *callback_arg;
} boaster_raster_task_t;

void boaster_raster_runner(void *arg) {
    boaster_raster_task_t *task =
        (boaster_raster_task_t*) arg;

    boaster_format_t *vertex_format = task->vertex_format;
    boaster_buffer_t *vertex_buffer = task->vertex_buffer;
    boaster_image_t *image = task->image;
    boaster_pixel_callback_t callback = task->callback;
    void *callback_arg = task->callback_arg;

    int vertex_size = vertex_format->size;
    int vertex_count = vertex_buffer->__size / vertex_size;

    if (0) {
    printf("Vertex count: %d; size: %d; buffer: %d bytes\n",
        vertex_count, vertex_size, vertex_buffer->__size);
    }

    for (int y = task->y; y < task->y + task->height; ++y) {
        for (int x = task->x; x < task->x + task->width; ++x) {
            for (int i = 0; i < vertex_count; i += 3) {

                float barycentrics[] = { 0, 0, 0 };

                boaster_vertex_t *vertices =
                    &((boaster_vertex_t*) vertex_buffer->data)[i];

                if(boaster_point_in_vbuf_triangle(
                    vertex_buffer->data,
                    x, y, i,
                    barycentrics,
                    image
                )) {
                    boaster_pixel_t* pixel =
                        (boaster_pixel_t*) boaster_image_get_pixel(image, x, y);

                    callback(vertices, x, y, barycentrics, pixel, callback_arg);
                }
            }
        }
    }
}

void boaster_multi_raster(
    boaster_buffer_t *vertex_buffer,
    boaster_format_t *vertex_format,
    boaster_image_t *image,
    boaster_pixel_callback_t callback,
    void *callback_arg) {

    int num_threads = 8;

    int section_width = image->width / num_threads + 1;
    int section_height = image->height / num_threads + 1;
    int task_count =
        (image->width / section_width + 1) *
        (image->height / section_height + 1);

    boaster_executor_t *executor = boaster_executor_create(num_threads);
    boaster_raster_task_t *tasks = (boaster_raster_task_t*)
        malloc(task_count * sizeof(boaster_raster_task_t));

    int i = 0;
    for (int y = 0; y < image->height; y += section_height) {
        for (int x = 0; x < image->width; x += section_width) {
            tasks[i].x = x;
            tasks[i].y = y;
            tasks[i].width = fmin(image->width - x, section_width);
            tasks[i].height = fmin(image->height - y, section_height);

            tasks[i].vertex_buffer = vertex_buffer;
            tasks[i].vertex_format = vertex_format;
            tasks[i].image = image;
            tasks[i].callback = callback;
            tasks[i].callback_arg = callback_arg;

            boaster_executor_push_task(executor, boaster_raster_runner,
                &tasks[i]);

            ++i;
        }
    }

    boaster_executor_join(executor);
    boaster_executor_free(executor);
    free(tasks);
}

void pixel_callback(boaster_vertex_t* vertices,
    size_t x, size_t y,
    float barycentrics[3],
    boaster_pixel_t* pixel,
    void *custom_data) {

    boaster_pixel_context_t *pixel_context =
        (boaster_pixel_context_t*) custom_data;
    boaster_draw_call_t* draw_call =
        pixel_context->draw_call;
    boaster_format_t* format =
        draw_call->transform_format;

    for (int i = 0; i < format->property_count; ++i) {
        format->interpolators[i](
            vertices,
            barycentrics,
            pixel_context->vertex_data,
            format->size,
            &(format->properties[i])
        );
    }

    draw_call->pixel_shader(pixel_context->vertex_data, pixel,
        draw_call->uniform_data, format);
}

boaster_context_t *boaster_context_create(boaster_config_t config) {
    boaster_context_t *result =
        (boaster_context_t*) malloc(sizeof(boaster_context_t));

    switch (config.raster_mode) {
        case THREADED:
            result->raster_function = boaster_multi_raster;
            break;

        case SIMPLE:
        default:
            result->raster_function = boaster_single_raster;
    }

    return result;
}

void boaster_context_destroy(boaster_context_t *context) {
    free(context);
}

void boaster_render(boaster_context_t *context, boaster_draw_call_t draw_call) {
    assert(context != NULL);
    assert(draw_call.vertex_buffer != NULL);
    assert(draw_call.vertex_shader != NULL);

    // Extract fields
    boaster_buffer_t* vertex_buffer = draw_call.vertex_buffer;
    boaster_format_t* input_format = draw_call.input_format;
    boaster_format_t* transform_format = draw_call.transform_format;
    boaster_vertex_shader_t vertex_shader = draw_call.vertex_shader;
    boaster_pixel_shader_t pixel_shader = draw_call.pixel_shader;
    void* uniform_data = draw_call.uniform_data;
    boaster_image_t* image = draw_call.target_image;

    // Vertices must come in multiples of three, forming triangles
    size_t in_vertex_size = input_format->size;
    size_t vertex_count = vertex_buffer->__size / in_vertex_size;
    assert(vertex_count % 3 == 0);

    // Allocate memory for vertex shader output
    size_t out_vertex_size = transform_format->size;
    boaster_buffer_t *vertex_out = boaster_buffer_create();
    boaster_buffer_ensure_capacity(vertex_out, out_vertex_size * vertex_count);
    vertex_out->__size = out_vertex_size * vertex_count;

    boaster_run_vertex_shader(draw_call, vertex_count, vertex_out);

    // Rasterize output triangles
    boaster_pixel_context_t pixel_context;
    pixel_context.draw_call = &draw_call;
    pixel_context.vertex_data = malloc(out_vertex_size);

    context->raster_function(
        vertex_out,
        transform_format,
        image,
        pixel_callback,
        &pixel_context
    );

    // Cleanup
    free(pixel_context.vertex_data);
    boaster_buffer_destroy(vertex_out);
}
