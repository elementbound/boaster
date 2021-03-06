#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <sys/sysinfo.h>
#include "include/boaster/boaster.h"
#include "include/boaster/buffer.h"
#include "include/boaster/executor.h"
#include "include/boaster/format.h"
#include "include/boaster/types.h"

typedef struct {
    boaster_draw_call_t *draw_call;
    void *vertex_data;
    void *pixel_data;
    boaster_property_t *depth_property;
    boaster_format_t* image_format;
} boaster_pixel_context_t;

typedef void(*boaster_rasterizer_t)(
    boaster_context_t *boaster_context,
    boaster_draw_call_t draw_call,
    boaster_buffer_t *vertex_buffer,
    boaster_format_t *vertex_format,
    boaster_image_t *image,
    boaster_pixel_callback_t callback);

struct boaster_context_t {
    boaster_rasterizer_t raster_function;
    int num_threads;
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
        void* input_vertex = vertex_buffer->data + input_format->size * i;
        void* output_vertex = out->data + transform_format->size * i;

        vertex_shader(input_vertex, output_vertex, uniform_data,
            input_format, transform_format);

        float *position = (float*) output_vertex;
        position[0] /= position[3];
        position[1] /= position[3];
        position[2] /= position[3];
        /* Leave W as is, for potential debug purposes */
        // position[3] = 1.0;

        #ifdef BOASTER_DUMP_VERTEX_SHADER_OUTPUT
        printf("[%f %f %f][%f]\n",
            position[0], position[1], position[2], position[3]);
        #endif
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

    float v1x = Bx - Ax;
    float v1y = By - Ay;

    float v2x = Px - Ax;
    float v2y = Py - Ay;

    float dot00 = v0x * v0x + v0y * v0y;
    float dot01 = v0x * v1x + v0y * v1y;
    float dot02 = v0x * v2x + v0y * v2y;
    float dot11 = v1x * v1x + v1y * v1y;
    float dot12 = v1x * v2x + v1y * v2y;

    float invDenom = 1 / (dot00 * dot11 - dot01 * dot01);
    barycentrics[0] = (dot11 * dot02 - dot01 * dot12) * invDenom;
    barycentrics[1] = (dot00 * dot12 - dot01 * dot02) * invDenom;
    barycentrics[2] = 1.0f - barycentrics[0] - barycentrics[1];

    return (isgreaterequal(barycentrics[0], 0.0)
        && isgreaterequal(barycentrics[1], 0.0)
        && isless(barycentrics[0] + barycentrics[1], 1.0 + threshold));
}

int boaster_point_in_vbuf_triangle(
    float **vertices,
    int x, int y, int i,
    float *barycentrics,
    boaster_image_t *image) {

    float Ax = vertices[0][0];
    float Ay = vertices[0][1];
    float Az = vertices[0][2];

    float Bx = vertices[1][0];
    float By = vertices[1][1];
    float Bz = vertices[1][2];

    float Cx = vertices[2][0];
    float Cy = vertices[2][1];
    float Cz = vertices[2][2];

    float Px = -1.0 + 2.0 * x / (float)image->width;
    float Py = -1.0 + 2.0 * y / (float)image->height;
    float Pz = 1.0;

    return boaster_check_point_in_triangle(
        Ax, Ay, Az, Bx, By, Bz, Cx, Cy, Cz,
        Px, Py, Pz,
        0.005, barycentrics
    );
}

void boaster_single_raster(
    boaster_context_t *boaster_context,
    boaster_draw_call_t draw_call,
    boaster_buffer_t *vertex_buffer,
    boaster_format_t *vertex_format,
    boaster_image_t *image,
    boaster_pixel_callback_t callback) {

    int vertex_size = vertex_format->size;
    int vertex_count = vertex_buffer->__size / vertex_size;
    int pixel_size = image->format->size;

    boaster_pixel_context_t pixel_context = {
        .draw_call = &draw_call,
        .vertex_data = malloc(vertex_size),
        .pixel_data = malloc(pixel_size),
        .depth_property = boaster_format_get_property(image->format, "depth"),
        .image_format = image->format
    };

    for (int y = 0; y < image->height; ++y) {
        for (int x = 0; x < image->width; ++x) {
            for (int i = 0; i < vertex_count; i += 3) {
                float barycentrics[] = { 0, 0, 0 };

                /*
                 * Position MUST be the first property, and it must consist of
                 * at least 3 floats, so casting this to (float**) is safe,
                 * unless validation is skipped with BOASTER_NO_VALIDATION
                 */
                void *vertices[] = {
                    vertex_buffer->data + vertex_format->size * i,
                    vertex_buffer->data + vertex_format->size * (i + 1),
                    vertex_buffer->data + vertex_format->size * (i + 2)
                };

                if(boaster_point_in_vbuf_triangle(
                    (float**) vertices,
                    x, y, i,
                    barycentrics,
                    image
                )) {
                    void* pixel = boaster_image_get_pixel(image, x, y);

                    callback(vertices, x, y, barycentrics, pixel,
                        &pixel_context);
                }
            }
        }
    }

    free(pixel_context.vertex_data);
    free(pixel_context.pixel_data);
}

typedef struct {
    int x;
    int y;
    int width;
    int height;

    boaster_draw_call_t *draw_call;
    boaster_buffer_t *vertex_buffer;
    boaster_format_t *vertex_format;
    boaster_image_t *image;
    boaster_pixel_callback_t callback;
} boaster_raster_task_t;

void boaster_raster_runner(void *arg) {
    boaster_raster_task_t *task =
        (boaster_raster_task_t*) arg;

    boaster_format_t *vertex_format = task->vertex_format;
    boaster_buffer_t *vertex_buffer = task->vertex_buffer;
    boaster_image_t *image = task->image;
    boaster_pixel_callback_t callback = task->callback;

    int vertex_size = vertex_format->size;
    int vertex_count = vertex_buffer->__size / vertex_size;
    int pixel_size = image->format->size;

    boaster_pixel_context_t pixel_context = {
        .draw_call = task->draw_call,
        .vertex_data = malloc(vertex_size),
        .pixel_data = malloc(pixel_size),
        .depth_property =
            boaster_format_get_property(image->format, "depth"),
        .image_format = image->format
    };

    for (int y = task->y; y < task->y + task->height; ++y) {
        for (int x = task->x; x < task->x + task->width; ++x) {
            for (int i = 0; i < vertex_count; i += 3) {
                float barycentrics[] = { 0, 0, 0 };

                /*
                 * Position MUST be the first property, and it must consist of
                 * at least 3 floats, so casting this to (float**) is safe,
                 * unless validation is skipped with BOASTER_NO_VALIDATION
                 */
                void *vertices[] = {
                    vertex_buffer->data + vertex_format->size * i,
                    vertex_buffer->data + vertex_format->size * (i + 1),
                    vertex_buffer->data + vertex_format->size * (i + 2)
                };

                if(boaster_point_in_vbuf_triangle(
                    (float**) vertices,
                    x, y, i,
                    barycentrics,
                    image
                )) {
                    void* pixel = boaster_image_get_pixel(image, x, y);

                    callback(vertices, x, y, barycentrics, pixel,
                        &pixel_context);
                }
            }
        }
    }

    free(pixel_context.vertex_data);
    free(pixel_context.pixel_data);
}

void boaster_multi_raster(
    boaster_context_t *boaster_context,
    boaster_draw_call_t draw_call,
    boaster_buffer_t *vertex_buffer,
    boaster_format_t *vertex_format,
    boaster_image_t *image,
    boaster_pixel_callback_t callback) {

    int num_threads = boaster_context->num_threads;

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

            tasks[i].draw_call = &draw_call;
            tasks[i].vertex_buffer = vertex_buffer;
            tasks[i].vertex_format = vertex_format;
            tasks[i].image = image;
            tasks[i].callback = callback;

            boaster_executor_push_task(executor, boaster_raster_runner,
                &tasks[i]);

            ++i;
        }
    }

    boaster_executor_join(executor);
    boaster_executor_free(executor);
    free(tasks);
}

void pixel_callback(void** vertices,
    size_t x, size_t y,
    float barycentrics[3],
    void* pixel,
    void *custom_data) {

    boaster_pixel_context_t *pixel_context =
        (boaster_pixel_context_t*) custom_data;
    boaster_draw_call_t* draw_call =
        pixel_context->draw_call;
    boaster_format_t* format =
        draw_call->transform_format;
    boaster_format_t* image_format =
        pixel_context->image_format;
    boaster_property_t *depth_property =
        pixel_context->depth_property;

    for (int i = 0; i < format->property_count; ++i) {
        format->interpolators[i](
            *vertices,
            barycentrics,
            pixel_context->vertex_data,
            format->size,
            &(format->properties[i])
        );
    }

    float *position = (float*) pixel_context->vertex_data;

    if (fabs(position[0]) < 1.0
        && fabs(position[1]) < 1.0
        && fabs(position[2]) < 1.0
    ) {
        int copy_pixel = 1;

        draw_call->pixel_shader(
            pixel_context->vertex_data, pixel_context->pixel_data,
            draw_call->uniform_data, format);

        if (draw_call->enable_depth_test) {
            assert(depth_property->size == sizeof(uint32_t));

            const uint32_t max_depth = (1u << 31) - 1;
            uint32_t current_z =
                *(uint32_t*) boaster_property_get_pointer(depth_property,
                    pixel);
            uint32_t new_z = (1.0 + position[2]) / 2.0 * max_depth;

            if (new_z < current_z) {
                uint32_t *out_z =
                    (uint32_t*) boaster_property_get_pointer(depth_property,
                        pixel_context->pixel_data);

                *out_z = new_z;
            } else {
                copy_pixel = 0;
            }
        }

        if (copy_pixel)
            memcpy(pixel, pixel_context->pixel_data, image_format->size);
    }
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

    if (config.num_threads <= 0) {
        result->num_threads = get_nprocs_conf();
    } else {
        result->num_threads = config.num_threads;
    }

    return result;
}

void boaster_context_destroy(boaster_context_t *context) {
    free(context);
}

int boaster_format_check_position(boaster_format_t *format) {
    assert(format);

    boaster_property_t *position =
        boaster_format_get_property(format, "position");

    int position_index =
        boaster_format_get_property_index(format, "position");

    if (!position)
        return 0;

    if (position_index != 0)
        return 0;

    if (position->offset != 0)
        return 0;

    if (position->component_size != sizeof(float))
        return 0;

    if (position->component_count < 4)
        return 0;

    return 1;
}

boaster_error_t boaster_validate_draw_call(boaster_draw_call_t draw_call) {
    if (!draw_call.vertex_shader)
        return BOASTER_MISSING_VERTEX_SHADER;

    if (!draw_call.pixel_shader)
        return BOASTER_MISSING_PIXEL_SHADER;

    if (!draw_call.vertex_buffer)
        return BOASTER_MISSING_VERTEX_BUFFER;

    if (!draw_call.input_format)
        return BOASTER_MISSING_INPUT_FORMAT;

    if (!boaster_format_check_position(draw_call.input_format))
        return BOASTER_BAD_INPUT_FORMAT;

    if (!draw_call.transform_format)
        return BOASTER_MISSING_TRANSFORM_FORMAT;

    if (!draw_call.target_image)
        return BOASTER_MISSING_IMAGE;

    return BOASTER_OK;
}

boaster_error_t boaster_render(
    boaster_context_t *context,
    boaster_draw_call_t draw_call
    ) {
    #ifndef BOASTER_NO_VALIDATE
    if (!context)
        return BOASTER_MISSING_CONTEXT;

    boaster_error_t error = boaster_validate_draw_call(draw_call);
    if (error != BOASTER_OK)
        return error;
    #endif

    // Extract fields
    boaster_buffer_t* vertex_buffer = draw_call.vertex_buffer;
    boaster_format_t* input_format = draw_call.input_format;
    boaster_format_t* transform_format = draw_call.transform_format;
    boaster_vertex_shader_t vertex_shader = draw_call.vertex_shader;
    boaster_pixel_shader_t pixel_shader = draw_call.pixel_shader;
    void* uniform_data = draw_call.uniform_data;
    boaster_image_t* image = draw_call.target_image;

    boaster_format_t *image_format = image->format;
    boaster_property_t *depth_property =
        boaster_format_get_property(image_format, "depth");

    if (draw_call.enable_depth_test && !depth_property)
        draw_call.enable_depth_test = 0;

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
    context->raster_function(
        context,
        draw_call,
        vertex_out,
        transform_format,
        image,
        pixel_callback
    );

    // Cleanup
    boaster_buffer_destroy(vertex_out);

    return BOASTER_OK;
}
