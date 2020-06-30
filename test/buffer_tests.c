#include <string.h>
#include "include/boaster/buffer.h"
#include "include/testboat.h"

int main() {
    test("Create empty buffer",
        boaster_buffer_t* buffer = boaster_buffer_create();

        test_assert(buffer->__size == 0, "Buffer should be empty");
        test_assert(buffer->__capacity == 0, "Buffer should have no capacity");
        test_assert(buffer->data == 0,
            "Buffer should have no memory allocated");

        boaster_buffer_destroy(buffer);
    );

    test("Buffer should fit pushed bytes",
        // Given
        boaster_buffer_t *buffer = boaster_buffer_create();
        const char* data = "Hello world!";

        // When
        boaster_buffer_push_bytes(buffer, data, strlen(data));

        // Then
        test_assert(buffer->__size == strlen(data),
            "Buffer size should equal data");
        test_assert(buffer->__capacity >= strlen(data),
            "Buffer capacity should hold data");
        test_assert(strcmp(buffer->data, data) == 0,
            "Buffer data should equal pushed data");

        // Teardown
        boaster_buffer_destroy(buffer);
    );

    test("Buffer should fit pushed data",
        // Given
        boaster_buffer_t *buffer = boaster_buffer_create();
        int data = 72;

        // When
        boaster_buffer_push_data(buffer, int, data);

        // Then
        test_assert(buffer->__size == sizeof(data),
            "Buffer size should equal data");
        test_assert(buffer->__capacity >= sizeof(data),
            "Buffer capacity should hold data");
        test_assert(boaster_buffer_get_at_index(buffer, int, 0) == data,
            "Buffer data should equal pushed data");

        // Teardown
        boaster_buffer_destroy(buffer);
    );

    return test_summarize();
}
