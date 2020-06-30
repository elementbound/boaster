#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "include/boaster/boaster.h"
#include "include/boaster/millitime.h"
#include "include/boaster/varray.h"
#include "include/boastgl/boastgl.h"
#include "include/boastmath/matrix.h"
#include "include/boastmath/scalar.h"
#include "include/boastmath/vector.h"
#include "include/mesh_obj.h"
#include "include/shaders.h"
#include "include/vertex.h"

// Settings
const size_t scale = 6;
const size_t width = 720 / scale;
const size_t height = 720 / scale;

boaster_config_t boaster_config = {
    .raster_mode = SIMPLE,
    .num_threads = -1
};

objmesh_t **anim_frames;
size_t anim_numframes;
double anim_duration;
size_t anim_fps = 20;

void animate_color(float *result, int mesh_idx, float af) {
    if (mesh_idx == 1) {
        // Beak
        result[0] = 0.296;
        result[1] = 0.208;
        result[2] = 0.194;
    } else if (mesh_idx == 2) {
        // Eyes
        result[0] = 0.075;
        result[1] = 0.075;
        result[2] = 0.075;
    } else if (mesh_idx == 0) {
        // Body
        if (af > 0.75) {
            result[0] = 0.956;
            result[1] = 0.246;
            result[2] = 0.991;
        } else if (af > 0.5) {
            result[0] = 0.254;
            result[1] = 0.468;
            result[2] = 0.973;
        } else if (af > 0.25) {
            result[0] = 0.238;
            result[1] = 0.965;
            result[2] = 0.235;
        } else {
            result[0] = 0.965;
            result[1] = 0.254;
            result[2] = 0.246;
        }
    }
}

int main(int argc, const char** argv) {
    if (argc < 2) {
        printf("Usage: %s <animation>\n", argv[0]);
        printf("For animations, see data/anim\n");
        return 1;
    }

    // Load animation
    const char *animation = argv[1];
    boaster_varray_initialize((void**) &anim_frames, &anim_numframes);

    for (int i = 0; ; ++i) {
        char filename[1024];
        snprintf(filename, sizeof(filename), "data/anim/%s/%s_%06d.obj",
            animation, animation, i);

        FILE *file = fopen(filename, "r");

        if (!file)
            break;

        objmesh_t *frame = objmesh_load(file);
        boaster_varray_add_pointer(
            (void**) &anim_frames, &anim_numframes, frame);
        fclose(file);

        printf("Loaded file %s\n", filename);
    }

    anim_duration = 1000 * anim_numframes / (double) anim_fps;

    printf("Loaded %d frames ( %fms ) of %s\n",
        anim_numframes, anim_duration, animation);

    // Init Boaster
    printf("Setup Boaster\n");
    boaster_context_t *boaster_context = boaster_context_create(boaster_config);

    // Init BoastGL
    printf("Setup BoastGL\n");
    if (!boastgl_init()) {
        printf("Failed to init BoastGL!\n");
        return 3;
    }

    printf("Setup display\n");
    boastgl_window_t *window = boastgl_window_create();

    // Setup render
    printf("Setup draw call\n");
    parrot_uniforms_t uniforms;

    boaster_format_t vertex_format;
    boaster_format_t transform_format;
    boaster_format_t image_format;

    boaster_format_init(&image_format);
    boaster_format_add_property(
        &image_format, "color", sizeof(uint8_t), 4, 0, NULL);
    boaster_format_add_property(
        &image_format, "depth", sizeof(uint32_t), 1, 4, NULL);

    boaster_image_t *image = boaster_image_create(&image_format, width, height);

    parrot_init_vertex_format(&vertex_format);
    parrot_init_transform_format(&transform_format);

    printf("Input vertex format:\n");
    for (int i = 0; i < vertex_format.property_count; ++i) {
        boaster_property_t property = vertex_format.properties[i];

        printf("\t%s: %dx%d = %d bytes @ offset %d\n",
            property.name, property.component_count, property.component_size,
            property.size, property.offset);
    }

    printf("Transform vertex format:\n");
    for (int i = 0; i < transform_format.property_count; ++i) {
        boaster_property_t property = transform_format.properties[i];

        printf("\t%s: %dx%d = %d bytes @ offset %d\n",
            property.name, property.component_count, property.component_size,
            property.size, property.offset);
    }

    printf("Image format:\n");
    for (int i = 0; i < image_format.property_count; ++i) {
        boaster_property_t property = image_format.properties[i];

        printf("\t%s: %dx%d = %d bytes @ offset %d\n",
            property.name, property.component_count, property.component_size,
            property.size, property.offset);
    }

    boaster_draw_call_t draw_call = {
        .vertex_shader = parrot_vertex_shader,
        .pixel_shader = parrot_pixel_shader,
        .uniform_data = &uniforms,

        .input_format = &vertex_format,
        .transform_format = &transform_format,

        .enable_depth_test = 1,

        .target_image = image
    };

    bm_mat4 mat_view;
    bm_mat4 mat_proj;

    // 4 bytes for color, 4 bytes for depth
    uint8_t background_color[] = { 0, 0, 0, 255, 255, 255, 255, 255 };

    // Render
    printf("Enter render loop\n");
    double t_loop_start = boaster_get_millitime();
    uint64_t frames_rendered = 0;

    while (boastgl_window_is_open(window)) {
        double t_frame_start = boaster_get_millitime();
        double t_loop_at = t_frame_start - t_loop_start;

        double camera_duration = 8000.0;
        double f = fmod(t_loop_at, camera_duration) / camera_duration;
        double af = fmod(t_loop_at, anim_duration) / anim_duration;
        double sf = (1.0 + sin(f * M_PI * 2.0)) / 2.0;

        int frame_index = anim_numframes * af;
        double frame_length = 1000.0 / anim_fps;
        double frame_start = frame_index * frame_length;
        double frame_end = (frame_index + 1) * frame_length;
        double frame_transition = bm_unlerp(frame_start, frame_end,
            fmod(t_loop_at, anim_duration));

        double dist = 6.0;
        double yaw = bm_rad(bm_lerp(60.0, 120.0, sf));
        double fromX =  cos(yaw) * dist;
        double fromY = -sin(yaw) * dist;
        double fromZ = dist - 2.0 * sf;

        uniforms.view_axis[0] = fromX / dist;
        uniforms.view_axis[1] = fromY / dist;
        uniforms.view_axis[2] = fromZ / dist;
        uniforms.view_axis[3] = 1.0;

        bm_vecnormalize(uniforms.view_axis);

        bm_mat_lookat(
            mat_view,
            fromX, fromY, fromZ,
            0.0, 0.0, 1.0,
            0.0, 0.0, 1.0
        );

        bm_mat_perspective_fov(
            mat_proj,
            height / (float) width, bm_rad(60.0),
            0.2, 32.0
        );

        bm_matmul(uniforms.mat_VP, mat_proj, mat_view);

        boaster_image_fill(image, background_color, sizeof(background_color));

        objmesh_t *mesh = anim_frames[frame_index];
        objmesh_t *next_mesh =
            anim_frames[(frame_index + 1) % anim_numframes];

        for (int i = 0; i < mesh->object_count; ++i) {
            draw_call.vertex_buffer = mesh->objects[i].vertex_buffer;
            uniforms.next_mesh = next_mesh->objects[i].vertex_buffer;
            uniforms.transition_factor = frame_transition;

            animate_color(uniforms.color, i, af);

            boaster_error_t error = boaster_render(boaster_context, draw_call);

            if (error != BOASTER_OK) {
                printf("Error rendering %s: %d\n",
                    mesh->objects[i].name, error);
            }
        }

        boastgl_window_push_image(window, image, boastgl_byte3_codec, scale);

        double t_frame_end = boaster_get_millitime();

        printf("Frame time: %.2f ms   \r",
            t_frame_end - t_frame_start);
        fflush(stdout);

        #ifdef BOASTER_DUMP_VERTEX_SHADER_OUTPUT
            break;
        #endif

        ++frames_rendered;
    }

    double t_loop_end = boaster_get_millitime();
    double loop_duration = t_loop_end - t_loop_start;

    printf("\n");
    printf("Rendered %d frames in %f ms\n", frames_rendered, loop_duration);
    printf("Average fps: %f\n", (1000 * frames_rendered) / loop_duration);

    // Teardown
    boaster_image_destroy(image);
    boastgl_window_destroy(window);
    boastgl_terminate();
    boaster_context_destroy(boaster_context);

    for (int i = 0; i < anim_numframes; ++i) {
        objmesh_destroy(anim_frames[i]);
    }
    boaster_varray_clear((void**) &anim_frames, &anim_numframes);

    return 0;
}
