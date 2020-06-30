#include <stddef.h>
#include "include/boaster/format.h"
#include "include/vertex.h"

void parrot_init_vertex_format(boaster_format_t *format) {
    boaster_format_init(format);

    boaster_format_add_property(
        format, "position", sizeof(float), 4,
        offsetof(parrot_vertex_t, position),
        boaster_floats_interpolator
    );

    boaster_format_add_property(
        format, "normal", sizeof(float), 3,
        offsetof(parrot_vertex_t, normal),
        boaster_floats_interpolator
    );

    boaster_format_add_property(
        format, "idx", sizeof(uint32_t), 1,
        offsetof(parrot_vertex_t, idx),
        boaster_noop_interpolator
    );
}

void parrot_init_transform_format(boaster_format_t *format) {
    boaster_format_init(format);

    boaster_format_add_property(
        format, "position", sizeof(float), 4,
        offsetof(parrot_vertex_t, position),
        boaster_floats_interpolator
    );

    boaster_format_add_property(
        format, "color", sizeof(float), 3,
        offsetof(parrot_vertex_t, normal),
        boaster_floats_interpolator
    );
}
