#include <math.h>
#include <signal.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <time.h>
#include <unistd.h>
#include "include/boaster/boaster.h"
#include "include/millitime.h"

double time_spent = 0;
uint64_t vertices_rendered = 0;

void exit_handler(int _) {
    printf("\n");
    printf("Rendered %d vertices in %f ms\n", vertices_rendered, time_spent);
    printf("Average rendering speed: %f vertices / sec\n",
        vertices_rendered * 1000.0 / time_spent);

    exit(0);
}

void clear_screen() {
    printf("\e[1;1H\e[2J");
}

void pass_through_vertex_shader(void* v_vertex, void* v_out, void* v_uniforms,
    boaster_format_t *in_format,
    boaster_format_t *out_format) {
    boaster_property_t* in_position_prop =
        boaster_format_get_property(in_format, "position");
    boaster_property_t* in_color_prop =
        boaster_format_get_property(in_format, "color");
    boaster_property_t* out_position_prop =
        boaster_format_get_property(out_format, "position");
    boaster_property_t* out_color_prop =
        boaster_format_get_property(out_format, "color");

    float* in_position =
        boaster_property_get_floats(in_position_prop, v_vertex);
    float* in_color =
        boaster_property_get_floats(in_color_prop, v_vertex);

    float* out_position =
        boaster_property_get_floats(out_position_prop, v_out);
    float* out_color =
        boaster_property_get_floats(out_color_prop, v_out);

    boaster_vertex_t *in_vertex = (boaster_vertex_t*) v_vertex;
    boaster_vertex_t *out_vertex = (boaster_vertex_t*) v_out;
    float step = *(float*)v_uniforms;
    float fx =  cos((step + in_position[1]) * M_PI * 2.0);
    float fy = -sin((step + in_position[1]) * M_PI * 2.0);

    out_position[0] = in_position[0] + fx * 0.1;
    out_position[1] = in_position[1] + fx * 0.1;
    out_position[2] = in_position[2];
    out_position[3] = in_position[3];

    out_color[0] = in_color[0];
    out_color[1] = in_color[1];
    out_color[2] = in_color[2];
    out_color[3] = in_color[3];
}

float lerp(float a, float b, float f) {
    return (1 - f) * a + f * b;
}

char brightness_to_ascii(float brightness) {
    const char* gradient = " .-:=+*#%@";
    const int length = strlen(gradient);

    const int index = brightness * (float)(length - 1);
    return gradient[index];
}

void pixel_shader(
    void *vertex,
    void *v_out,
    void *v_uniforms,
    boaster_format_t *format) {

    boaster_pixel_t *out = (boaster_pixel_t*) v_out;
    boaster_property_t *in_color_p =
        boaster_format_get_property(format, "color");
    float *in_color = boaster_property_get_floats(in_color_p, vertex);
    float step = *(float*)v_uniforms;

    float f =  (1.0 + sin(step * M_PI * 2.0)) / 2.0;

    float value = 0.1 + 0.9 * in_color[0];
    // value = 0.2 + 0.8 * f;

    out->color[0] = value * 255;
    out->color[1] = value * 255;
    out->color[2] = value * 255;
    out->color[3] = 255;
}

void dump_image(boaster_image_t* image) {
    for (size_t y = 0; y < image->height; y++) {
        for (size_t x = 0; x < image->width; x++) {
            boaster_pixel_t* pixel =
                (boaster_pixel_t*) boaster_image_get_pixel(image, x, y);

            float value = fmaxf(pixel->color[0], pixel->color[1]);
            value = fmaxf(value, pixel->color[2]);
            value /= 255.0;
            value *= pixel->color[3] / 255.0;

            printf("%c", brightness_to_ascii(value));
        }

        printf("\n");
    }
}

int main() {
    const size_t width = 32;
    const size_t height = 16;

    signal(SIGINT, exit_handler);

    boaster_format_t pixel_format;
    boaster_format_init(&pixel_format);
    boaster_format_add_property(&pixel_format, "color", sizeof(unsigned char),
        4, offsetof(boaster_pixel_t, color), NULL);

    boaster_image_t *image = boaster_image_create(&pixel_format, width, height);

    boaster_vertex_t triangle_vertices[] = {
        { .position = { 0.2, 0.2, 0.0, 0.f }, .color = { 0.f, 0.f, 0.f, 1.f } },
        { .position = { 0.8, 0.2, 0.0, 0.f }, .color = { 0.5, 1.f, 0.f, 1.f } },
        { .position = { 0.2, 0.8, 0.0, 0.f }, .color = { 1.0, 1.f, 0.f, 1.f } },

        { .position = { 0.2, 0.8, 0.0, 0.f }, .color = { 1.0, 1.f, 0.f, 1.f } },
        { .position = { 0.8, 0.2, 0.0, 0.f }, .color = { 0.5, 1.f, 0.f, 1.f } },
        { .position = { 0.8, 0.8, 0.0, 0.f }, .color = { 1.f, 1.f, 0.f, 1.f } },
    };

    boaster_format_t format;
    boaster_format_init(&format);
    boaster_format_add_property(&format, "position", sizeof(float), 4,
        offsetof(boaster_vertex_t, position), boaster_floats_interpolator);
    boaster_format_add_property(&format, "color", sizeof(float), 4,
        offsetof(boaster_vertex_t, color), boaster_floats_interpolator);

    boaster_buffer_t* vertex_buffer = boaster_buffer_create();
    boaster_buffer_push_bytes(vertex_buffer, triangle_vertices,
        sizeof(triangle_vertices));

    boaster_pixel_t background = {
        .color = { 0, 0, 0, 255 }
    };

    boaster_draw_call_t draw_call = {
        .vertex_shader = pass_through_vertex_shader,
        .input_format = &format,
        .transform_format = &format,
        .pixel_shader = pixel_shader,
        .vertex_buffer = vertex_buffer,
        .target_image = image
    };

    uint64_t vertex_count = vertex_buffer->__size / format.size;

    for (int i = 0; ; i++) {
        if (i > image->width)
            i -= image->width;

        float f = i / (float)image->width;

        clear_screen();
        boaster_image_fill(image, &background, sizeof(background));
        draw_call.uniform_data = &f;

        double t_start = get_millitime();
        boaster_render(draw_call);
        double t_end = get_millitime();

        dump_image(image);

        time_spent += t_end - t_start;
        vertices_rendered += vertex_count;

        printf("%f ms\n", t_end - t_start);

        usleep(50 * 1000);
    }

    boaster_image_destroy(image);

    return 0;
}
