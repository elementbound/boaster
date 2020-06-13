#ifndef BOASTER_TYPES_SHADERS_H_
#define BOASTER_TYPES_SHADERS_H_

typedef void (*boaster_vertex_shader_t)(void *vertex, void *out,
    void *uniforms);
typedef void (*boaster_pixel_shader_t)(void *vertices, float barycentrics[3],
    void *out, void *uniforms);

#endif  // BOASTER_TYPES_SHADERS_H_
