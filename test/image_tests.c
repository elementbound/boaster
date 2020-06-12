#include "include/boaster/types.h"
#include "include/boaster/image.h"
#include "include/test/test.h"

int main() {
    test("Created image should have correct pixel size",
        // Given
        const size_t width = 32;
        const size_t height = 32;

        // When
        boaster_image_t* image = boaster_image_create(width, height);

        // Then
        test_assert(image->__pixel_size == sizeof(byte_t) * 4,
            "Pixel size should equal four bytes")
    );

    test("Created image should allocate memory",
        // Given
        const size_t width = 32;
        const size_t height = 32;

        // When
        boaster_image_t* image = boaster_image_create(width, height);

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
