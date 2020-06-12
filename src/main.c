#include <stdio.h>
#include <string.h>
#include "include/buffer.h"

int main() {
    const char *data = "Hello world!";

    buffer_t *buffer = buffer_create();
    buffer_push_data(buffer, int, 4);
    buffer_push_data(buffer, int, 8);
    buffer_push_data(buffer, int, 2);

    printf("Buffer size: %d\n", buffer->__size);
    printf("Buffer number #0: %d\n", buffer_get_at_index(buffer, int, 0));
    printf("Buffer number #1: %d\n", buffer_get_at_index(buffer, int, 1));
    printf("Buffer number #2: %d\n", buffer_get_at_index(buffer, int, 2));
    printf("Buffer content at offset 4: %d\n",
        buffer_get_at_offset(buffer, int, 4));

    buffer_destroy(buffer);

    return 0;
}
