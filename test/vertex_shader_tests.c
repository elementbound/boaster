#include <string.h>
#include "include/test/test.h"
#include "include/boaster/types.h"
#include "include/boaster/boaster.h"

typedef struct {
    float offset[4];
} test_uniforms_t;

void test_vertex_shader(void* v_vertex, void* v_out, void* v_uniforms,
    boaster_format_t *in_format,
    boaster_format_t *out_format) {
    boaster_vertex_t *in_vertex = (boaster_vertex_t*) v_vertex;
    boaster_vertex_t *out_vertex = (boaster_vertex_t*) v_out;
    test_uniforms_t *uniforms = (test_uniforms_t*) v_uniforms;

    out_vertex->position[0] = in_vertex->position[0] + uniforms->offset[0];
    out_vertex->position[1] = in_vertex->position[1] + uniforms->offset[1];
    out_vertex->position[2] = in_vertex->position[2] + uniforms->offset[2];
    out_vertex->position[3] = in_vertex->position[3] + uniforms->offset[3];

    out_vertex->color[0] = in_vertex->color[0];
    out_vertex->color[1] = in_vertex->color[1];
    out_vertex->color[2] = in_vertex->color[2];
    out_vertex->color[3] = in_vertex->color[3];
}

int main() {
    test_uniforms_t uniforms = {
        .offset = { 1.f, 2.f, 3.f, 1.f }
    };

    const size_t vertex_count = 3;

    boaster_vertex_t vertices[] = {
        { .position = { 0.f, 1.f, 2.f, 0.f }, .color = { 1.f, 0.f, 0.f, 1.f } },
        { .position = { 2.f, 3.f, 4.f, 0.f }, .color = { 0.f, 1.f, 0.f, 1.f } },
        { .position = { 8.f, 1.f, 7.f, 0.f }, .color = { 0.f, 1.f, 0.f, 1.f } }
    };

    boaster_vertex_t expected_vertices[] = {
        { .position = { 1.f, 3.f, 5.f, 1.f }, .color = { 1.f, 0.f, 0.f, 1.f } },
        { .position = { 3.f, 5.f, 7.f, 1.f }, .color = { 0.f, 1.f, 0.f, 1.f } },
        { .position = { 9.f, 3.f, 10.f, 1.f }, .color = { 0.f, 1.f, 0.f, 1.f } }
    };

    boaster_buffer_t *vertex_buffer = boaster_buffer_create();
    boaster_buffer_push_bytes(vertex_buffer, vertices, sizeof(vertices));

    boaster_format_t format;
    boaster_format_init(&format);
    boaster_format_add_property(&format, "position", sizeof(float), 4,
        offsetof(boaster_vertex_t, position), NULL);
    boaster_format_add_property(&format, "color", sizeof(float), 4,
        offsetof(boaster_vertex_t, color), NULL);

    boaster_draw_call_t draw_call = {
        .vertex_shader = test_vertex_shader,
        .uniform_data = &uniforms,
        .vertex_buffer = vertex_buffer,
        .input_format = &format,
        .transform_format = &format
    };

    test("Vertex shader should run for all vertices",
        // Given
        boaster_buffer_t *expected = boaster_buffer_create();
        boaster_buffer_push_bytes(expected, expected_vertices,
            sizeof(expected_vertices));

        boaster_buffer_t *actual = boaster_buffer_create();
        boaster_buffer_ensure_capacity(actual, vertex_buffer->__capacity);

        // When
        boaster_run_vertex_shader(draw_call, vertex_count, actual);

        // Then
        test_assert(memcmp(actual->data, expected->data, expected->__size) == 0,
            "Vertex shader output should equal expected");

        // Teardown
        boaster_buffer_destroy(vertex_buffer);
        boaster_buffer_destroy(expected);
        boaster_buffer_destroy(actual);
    );

    return test_summarize();
}
