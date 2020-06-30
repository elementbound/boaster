#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <GLFW/glfw3.h>
#include "include/boaster/buffer.h"
#include "include/boaster/types/pixel.h"
#include "include/boastgl/boastgl.h"

struct boastgl_window_t {
    GLFWwindow *gl_window;
    int is_open;
    GLuint texture;
    boaster_buffer_t* buffer;
};

void boastgl_print_error(int error, const char* message) {
    printf("[boastgl] GLFW error#%d: %s\n", error, message);
}

void handle_window_close(GLFWwindow* gl_window) {
    boastgl_window_t *window = glfwGetWindowUserPointer(gl_window);

    window->is_open = 0;
}

int boastgl_init() {
    if (!glfwInit()) {
        return 0;
    }

    glfwSetErrorCallback(boastgl_print_error);
    return 1;
}

void boastgl_terminate() {
    glfwTerminate();
}

boastgl_window_t *boastgl_window_create() {
    GLFWwindow *gl_window = glfwCreateWindow(64, 64, "boastgl", NULL, NULL);
    if (!gl_window) {
        return NULL;
    }

    boastgl_window_t *window =
        (boastgl_window_t*) malloc(sizeof(boastgl_window_t));

    window->gl_window = gl_window;
    window->buffer = boaster_buffer_create();
    window->is_open = 1;

    glfwMakeContextCurrent(window->gl_window);
    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &(window->texture));

    glBindTexture(GL_TEXTURE_2D, window->texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glfwSetWindowUserPointer(gl_window, window);
    glfwSetWindowCloseCallback(gl_window, handle_window_close);

    return window;
}

void boastgl_window_destroy(boastgl_window_t *window) {
    if (!window) {
        return;
    }

    glfwDestroyWindow(window->gl_window);
    boaster_buffer_destroy(window->buffer);
    free(window);
}

void boastgl_window_push_image(
    boastgl_window_t *window,
    boaster_image_t *image,
    float scale
) {
    assert(window);
    assert(image);

    if (!boastgl_window_is_open(window)) {
        return;
    }

    boaster_buffer_t *buffer = window->buffer;
    boaster_buffer_ensure_capacity(buffer,
        image->width * image->height * 3 * sizeof(uint8_t));

    size_t out_width = image->width * scale;
    size_t out_height = image->height * scale;

    // TODO(elementbound): Codec support for custom format conversion
    for (int y = 0; y < image->height; ++y) {
        for (int x = 0; x < image->width; ++x) {
            uint8_t* out =
                buffer->data + ((y * image->width) + x) * 3 * sizeof(uint8_t);

            boaster_pixel_t *in =
                (boaster_pixel_t*) boaster_image_get_pixel(image, x, y);

            out[0] = in->color[0] * in->color[3] / 255;
            out[1] = in->color[1] * in->color[3] / 255;
            out[2] = in->color[2] * in->color[3] / 255;
        }
    }

    // Push texture to OpenGL
    glfwMakeContextCurrent(window->gl_window);

    glBindTexture(GL_TEXTURE_2D, window->texture);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, image->width, image->height, 0,
        GL_RGB, GL_UNSIGNED_BYTE, buffer->data);

    // Draw
    glfwSetWindowSize(window->gl_window, out_width, out_height);
    glViewport(0, 0, out_width, out_height);

    glClearColor(0.0, 1.0, 1.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT);

    glBegin(GL_QUADS);

    glVertex2f(1.0, 1.0);
    glTexCoord2f(0.0, 0.0);

    glVertex2f(-1.0, 1.0);
    glTexCoord2f(0.0, 1.0);

    glVertex2f(-1.0, -1.0);
    glTexCoord2f(1.0, 1.0);

    glVertex2f(1.0, -1.0);
    glTexCoord2f(1.0, 0.0);

    glEnd();

    glfwSwapBuffers(window->gl_window);
    glfwPollEvents();
}

int boastgl_window_is_open(boastgl_window_t *window) {
    if (!window)
        return 0;

    return window->is_open;
}
