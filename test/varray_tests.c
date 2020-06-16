#include "include/boaster/varray.h"
#include "include/test/test.h"

int main() {
    test("Initialize should reset data",
        // Given
        int placeholder = 8;
        void *data = &placeholder;
        size_t count = 8;

        // When
        boaster_varray_initialize(&data, &count);

        // Then
        test_assert(!data, "Data should be reset")
        test_assert(!count, "Size should be reset")
    );

    test("Clear should free data",
        // Given
        int placeholder = 8;
        int *data = &placeholder;
        size_t count = 8;

        boaster_varray_initialize((void**)&data, &count);
        boaster_varray_add_int((void**)&data, &count, 8);
        boaster_varray_add_int((void**)&data, &count, 9);
        boaster_varray_add_int((void**)&data, &count, 7);

        // When
        boaster_varray_clear((void**)&data, &count);

        // Then
        test_assert(!data, "Data should be reset")
        test_assert(!count, "Size should be reset")
    );

    test("Add bytes should copy content",
        // Given
        int expected = 16;
        int *data = 0;
        size_t count = 0;

        boaster_varray_initialize((void**)&data, &count);

        // When
        boaster_varray_add_bytes((void**)(&data), &count, &expected, sizeof(expected));

        // Then
        test_assert(count == 1, "Count should be incremented")
        test_assert(data[0] == expected, "Data should be copied")
    )
    
    test("Add should copy primitive",
        // Given
        int expected = 16;
        int *data = 0;
        size_t count = 0;

        boaster_varray_initialize((void**)&data, &count);

        // When
        boaster_varray_add_int((void**)(&data), &count, 7);
        boaster_varray_add_int((void**)(&data), &count, 12);
        boaster_varray_add_int((void**)(&data), &count, expected);

        // Then
        test_assert(count == 3, "Count should be incremented")
        test_assert(data[2] == expected, "Data should be copied")
    )
    
    test("Remove bytes should strip",
        // Given
        int *data = 0;
        size_t count = 0;

        boaster_varray_initialize((void**)&data, &count);
        boaster_varray_add_int((void**)(&data), &count, 1);
        boaster_varray_add_int((void**)(&data), &count, 2);
        boaster_varray_add_int((void**)(&data), &count, 3);

        // When
        boaster_varray_remove_bytes((void**)&data, &count, 1, sizeof(int));

        // Then
        test_assert(count == 2, "Count should be decremented")
        test_assert(data[0] == 1, "First element should be retained")
        test_assert(data[1] == 3, "Last element should be retained")
    )
    
    test("Remove primitive should strip",
        // Given
        int *data = 0;
        size_t count = 0;

        boaster_varray_initialize((void**)&data, &count);
        boaster_varray_add_int((void**)(&data), &count, 1);
        boaster_varray_add_int((void**)(&data), &count, 2);
        boaster_varray_add_int((void**)(&data), &count, 3);

        // When
        boaster_varray_remove_int((void**)&data, &count, 1);

        // Then
        test_assert(count == 2, "Count should be decremented")
        test_assert(data[0] == 1, "First element should be retained")
        test_assert(data[1] == 3, "Last element should be retained")
    )

    return test_summarize();
}
