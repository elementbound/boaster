#include <stddef.h>  // offsetof
#include <string.h>
#include "include/boaster/vertex_format.h"
#include "include/test/test.h"

typedef struct {
    float position[4];
    float color[4];
    float normal[3];
} test_vertex_format_t;

int main() {
    test("Initialize should empty format",
        // Given
        boaster_vertex_format_t format;

        // When
        boaster_vertex_format_init(&format);

        // Then
        test_assert(!format.properties,
            "Properties should be NULL")
        test_assert(format.property_count == 0,
            "Properties count should be zero")

        // Teardown
        boaster_vertex_format_clear(&format);
    )

    test("Add property should append property",
        // Given
        boaster_vertex_format_t format;
        boaster_vertex_format_init(&format);

        // When
        boaster_vertex_format_add_property(&format, "position", sizeof(float),
            4, 0);
        boaster_vertex_format_add_property(&format, "color", sizeof(char),
            4, 16);
        boaster_vertex_format_add_property(&format, "normals", sizeof(float),
            3, 20);

        // Then
        test_assert(format.property_count == 3,
            "Property count should be updated")

        test_assert(strcmp(format.properties[0].name, "position") == 0,
            "Property[0] name should be \"position\"")
        test_assert(strcmp(format.properties[1].name, "color") == 0,
            "Property[1] name should be \"color\"")
        test_assert(strcmp(format.properties[2].name, "normals") == 0,
            "Property[2] name should be \"normals\"")

        test_assert(format.properties[0].size == 4 * sizeof(float),
            "Property[0] size should be 4 * sizeof(float)")
        test_assert(format.properties[1].size == 4 * sizeof(char),
            "Property[1] size should be 4 * sizeof(char)")
        test_assert(format.properties[2].size == 3 * sizeof(float),
            "Property[1] size should be 3 * sizeof(float)")

        test_assert(format.properties[0].component_size == sizeof(float),
            "Property[0] component size should be sizeof(float)")
        test_assert(format.properties[1].component_size == sizeof(char),
            "Property[1] component size should be sizeof(float)")
        test_assert(format.properties[2].component_size == sizeof(float),
            "Property[2] component size should be sizeof(float)")

        test_assert(format.properties[0].component_count == 4,
            "Property[0] component count should be 4")
        test_assert(format.properties[1].component_count == 4,
            "Property[1] component count should be 4")
        test_assert(format.properties[2].component_count == 3,
            "Property[2] component count should be 3")

        test_assert(format.properties[0].offset == 0,
            "Property[0] offset should be 4")
        test_assert(format.properties[1].offset == 16,
            "Property[1] offset should be 16")
        test_assert(format.properties[2].offset == 20,
            "Property[2] offset should be 20")
    )

    test("Get property index should (silently) fail if not found",
        // Given
        boaster_vertex_format_t format;
        boaster_vertex_format_init(&format);

        boaster_vertex_format_add_property(&format, "position", sizeof(float),
            4, 0);

        // When
        int index = boaster_vertex_format_get_property_index(&format, "nope");

        // Then
        test_assert(index == -1, "Index should be -1")
    )

    test("Get property index should return index if found",
        // Given
        boaster_vertex_format_t format;
        boaster_vertex_format_init(&format);

        boaster_vertex_format_add_property(&format, "position", sizeof(float),
            4, 0);
        boaster_vertex_format_add_property(&format, "color", sizeof(char),
            4, 16);

        // When
        int index = boaster_vertex_format_get_property_index(&format, "color");

        // Then
        test_assert(index == 1, "Index should be 1")
    )

    test_vertex_format_t vertex = {
        .position = {1.0, 2.0, 0.5, 1.0},
        .color = {1.0, 0.5, 0.0, 1.0},
        .normal = {0.5, 0.5, 0.5}
    };

    test("Get property component should return value",
        // Given
        boaster_vertex_format_t format;
        boaster_vertex_format_init(&format);

        boaster_vertex_format_add_property(&format, "position", sizeof(float),
            4, offsetof(test_vertex_format_t, position));
        boaster_vertex_format_add_property(&format, "color", sizeof(float),
            4, offsetof(test_vertex_format_t, color));
        boaster_vertex_format_add_property(&format, "normal", sizeof(float),
            3, offsetof(test_vertex_format_t, normal));

        int pos_index =
            boaster_vertex_format_get_property_index(&format, "position");
        int color_index =
            boaster_vertex_format_get_property_index(&format, "color");
        int normal_index =
            boaster_vertex_format_get_property_index(&format, "normal");

        // When
        float value =
            boaster_vertex_format_get_float(&format, &vertex, pos_index, 1);

        // Then
        test_assert(value == 2.0, "Position component should be 2")
    )

    test("Get property should return array",
        // Given
        boaster_vertex_format_t format;
        boaster_vertex_format_init(&format);

        boaster_vertex_format_add_property(&format, "position", sizeof(float),
            4, offsetof(test_vertex_format_t, position));
        boaster_vertex_format_add_property(&format, "color", sizeof(float),
            4, offsetof(test_vertex_format_t, color));
        boaster_vertex_format_add_property(&format, "normal", sizeof(float),
            3, offsetof(test_vertex_format_t, normal));

        int pos_index =
            boaster_vertex_format_get_property_index(&format, "position");
        int color_index =
            boaster_vertex_format_get_property_index(&format, "color");
        int normal_index =
            boaster_vertex_format_get_property_index(&format, "normal");

        // When
        float* actual =
            boaster_vertex_format_get_floats(&format, &vertex, pos_index);

        // Then
        test_assert(actual == vertex.position,
            "Position should point to vertex")
    )

    return test_summarize();
}
