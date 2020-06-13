#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include "include/boaster/boaster.h"

void clear_screen() {
    printf("\e[1;1H\e[2J");
}

int main() {
    const size_t width = 32;
    const size_t height = 16;
    boaster_image_t *image = boaster_image_create(width, height);

    boaster_vertex_t triangle_vertices[] = {
        { .position = { 0.f, 0.f, 0.f, 0.f }, .color = { 1.f, 0.f, 0.f, 1.f } },
        { .position = { 1.f, 0.f, 0.f, 0.f }, .color = { 0.f, 1.f, 0.f, 1.f } },
        { .position = { 0.5, 1.f, 0.f, 0.f }, .color = { 0.f, 1.f, 0.f, 1.f } }
    };

    for (int i = 0; ; i++) {
        if (i > image->width)
            i -= image->width;

        triangle_vertices[2].position[0] =
            (1.f + sin(i / (float)image->width * 2.f * M_PI)) / 2.f;

        boaster_fill_triangle(triangle_vertices, image);
        clear_screen();

        for (size_t y = 0; y < image->height; y++) {
            for (size_t x = 0; x < image->width; x++) {
                boaster_pixel_t* pixel =
                    (boaster_pixel_t*) boaster_image_get_pixel(image, x, y);

                if(pixel->color[0] > 0.5f)
                    printf("#");
                else
                    printf(".");
            }

            printf("\n");
        }

        usleep(100 * 1000);
    }

    boaster_image_destroy(image);

    return 0;
}
