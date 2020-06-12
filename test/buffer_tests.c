#include "include/buffer.h"
#include <stdio.h>
#include <string.h>

const char* __current_test;
int __current_pass;
int __tests_finished = 0;
int __tests_passed = 0;

void test_start(const char* name) {
    printf("[TEST][START] %s\n", name);
    __current_test = name;
    __current_pass = 1;
}

#define test_assert(condition, message) {\
    if (!(condition)) { \
        printf("[TEST][FAIL] %s\n", __current_test); \
        printf("    Assertion failed: " #condition "\n"); \
        printf("    Message: %s\n", message); \
        __current_pass = 0; \
    } \
}

void test_finish() {
    __tests_finished++;

    if (__current_pass) {
        __tests_passed++;

        printf("[TEST][PASS] %s\n", __current_test);
    } else {
        printf("[TEST][FAIL] %s\n", __current_test);
    }

    printf("----\n");
}

#define test(name, test) { \
    test_start(name);\
    test \
    test_finish(); \
}

int test_summarize() {
    if (__tests_finished == __tests_passed) {
        printf("[SUITE][PASS] %d / %d passed\n", \
            __tests_passed, __tests_finished);

        return 0;
    } else {
        printf("[SUITE][FAIL] %d / %d passed\n", \
            __tests_passed, __tests_finished);

        return 1;
    }
}

int main() {
    test("Create empty buffer",
        buffer_t* buffer = buffer_create();

        test_assert(buffer->__size == 0, "Buffer should be empty");
        test_assert(buffer->__capacity == 0, "Buffer should have no capacity");
        test_assert(buffer->data == 0,
            "Buffer should have no memory allocated");

        buffer_destroy(buffer);
    );

    test("Buffer should fit pushed bytes",
        // Given
        buffer_t *buffer = buffer_create();
        const char* data = "Hello world!";

        // When
        buffer_push_bytes(buffer, data, strlen(data));

        // Then
        test_assert(buffer->__size == strlen(data),
            "Buffer size should equal data");
        test_assert(buffer->__capacity >= strlen(data),
            "Buffer capacity should hold data");
        test_assert(strcmp(buffer->data, data) == 0,
            "Buffer data should equal pushed data");

        // Teardown
        buffer_destroy(buffer);
    );

    test("Buffer should fit pushed data",
        // Given
        buffer_t *buffer = buffer_create();
        int data = 72;

        // When
        buffer_push_data(buffer, int, data);

        // Then
        test_assert(buffer->__size == sizeof(data),
            "Buffer size should equal data");
        test_assert(buffer->__capacity >= sizeof(data),
            "Buffer capacity should hold data");
        test_assert(buffer_get_at_index(buffer, int, 0) == data,
            "Buffer data should equal pushed data");

        // Teardown
        buffer_destroy(buffer);
    );

    return test_summarize();
}
