#include "include/boaster/types.h"
#include "include/boaster/image.h"
#include "include/testboat.h"

int main() {
    boaster_format_t pixel_format;
    boaster_format_init(&pixel_format);
    boaster_format_add_property(&pixel_format, "color", sizeof(byte_t),
        4, offsetof(boaster_pixel_t, color), NULL);

    test("Created image should allocate memory",
        // Given
        const size_t width = 32;
        const size_t height = 32;

        // When
        boaster_image_t* image =
            boaster_image_create(&pixel_format, width, height);

        // Then
        test_assert(image->width == width,
            "Image should have correct width")
        test_assert(image->height == height,
            "Image should have correct height")
        test_assert(image->data != 0,
            "Image should have data allocated")
    );

    return test_summarize();
}
