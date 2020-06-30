#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "include/boaster/buffer.h"
#include "include/boaster/varray.h"
#include "include/vertex.h"
#include "include/mesh_obj.h"

const int LINE_BUFFER_SIZE = 1024;

int strstart(const char* str, const char* prefix) {
    int src_len = strlen(str);
    int prefix_len = strlen(prefix);

    for (int i = 0; i < src_len && i < prefix_len; ++i)
        if (str[i] != prefix[i])
            return 0;

    return 1;
}

// NOTE: What a hack
extern char *strdup(const char* src);

objmesh_t *objmesh_create() {
    objmesh_t *result = (objmesh_t*) malloc(sizeof(objmesh_t));

    boaster_varray_initialize(
        (void**) &(result->objects),
        &(result->object_count)
    );

    return result;
}

void objmesh_destroy(objmesh_t *mesh) {
    for (size_t i = 0; i < mesh->object_count; ++i) {
        objmesh_object_t object = mesh->objects[i];

        free(object.name);
        boaster_buffer_destroy(object.vertex_buffer);
    }

    boaster_varray_clear(
        (void**) &(mesh->objects),
        &(mesh->object_count)
    );

    free(mesh);
}

objmesh_t *objmesh_load(FILE *file) {
    objmesh_t *result = objmesh_create();
    char *line_buffer = (char*) malloc(LINE_BUFFER_SIZE);
    char *line = line_buffer;

    boaster_buffer_t *position_buffer = boaster_buffer_create();
    boaster_buffer_t *normal_buffer = boaster_buffer_create();

    objmesh_object_t *current_object;
    uint32_t vertex_index;

    for(;;) {
        line = fgets(line_buffer, LINE_BUFFER_SIZE, file);

        if (ferror(file)) {
            objmesh_destroy(result);
            result = NULL;
            break;
        }

        if (feof(file))
            break;

        if (strstart(line, "v ")) {
            float vx, vy, vz;
            sscanf(line, "v %f %f %f", &vx, &vy, &vz);

            boaster_buffer_push_bytes(position_buffer, &vx, sizeof(float));
            boaster_buffer_push_bytes(position_buffer, &vy, sizeof(float));
            boaster_buffer_push_bytes(position_buffer, &vz, sizeof(float));
        } else if (strstart(line, "vn ")) {
            float nx, ny, nz;
            sscanf(line, "vn %f %f %f", &nx, &ny, &nz);

            boaster_buffer_push_data(normal_buffer, float, nx);
            boaster_buffer_push_data(normal_buffer, float, ny);
            boaster_buffer_push_data(normal_buffer, float, nz);
        } else if (strstart(line, "f ")) {
            int v0, v1, v2;
            int n0, n1, n2;
            sscanf(line, "f %d//%d %d//%d %d//%d",
                &v0, &n0, &v1, &n1, &v2, &n2);

            float* position = (float*) position_buffer->data;
            float* normal = (float*) normal_buffer->data;

            --v0;
            --v1;
            --v2;

            --n0;
            --n1;
            --n2;

            parrot_vertex_t vertices[3];
            vertices[0].position[0] = position[v0 * 3];
            vertices[0].position[1] = position[v0 * 3 + 1];
            vertices[0].position[2] = position[v0 * 3 + 2];
            vertices[0].position[3] = 1.0;

            vertices[1].position[0] = position[v1 * 3];
            vertices[1].position[1] = position[v1 * 3 + 1];
            vertices[1].position[2] = position[v1 * 3 + 2];
            vertices[1].position[3] = 1.0;

            vertices[2].position[0] = position[v2 * 3];
            vertices[2].position[1] = position[v2 * 3 + 1];
            vertices[2].position[2] = position[v2 * 3 + 2];
            vertices[2].position[3] = 1.0;

            vertices[0].normal[0] = normal[n0 * 3];
            vertices[0].normal[1] = normal[n0 * 3 + 1];
            vertices[0].normal[2] = normal[n0 * 3 + 2];

            vertices[1].normal[0] = normal[n1 * 3];
            vertices[1].normal[1] = normal[n1 * 3 + 1];
            vertices[1].normal[2] = normal[n1 * 3 + 2];

            vertices[2].normal[0] = normal[n2 * 3];
            vertices[2].normal[1] = normal[n2 * 3 + 1];
            vertices[2].normal[2] = normal[n2 * 3 + 2];

            vertices[0].idx = vertex_index++;
            vertices[1].idx = vertex_index++;
            vertices[2].idx = vertex_index++;

            boaster_buffer_push_bytes(
                current_object->vertex_buffer,
                vertices,
                sizeof(parrot_vertex_t) * 3
            );
        } else if (strstart(line, "o ")) {
            char *name = line + 2;
            int len = strlen(name);

            // Remove trailing newline
            name[len-1] = '\0';

            objmesh_object_t new_object;
            new_object.name = strdup(name);
            new_object.vertex_buffer = boaster_buffer_create();

            boaster_varray_add_struct(
                &(result->objects),
                &(result->object_count),
                new_object
            );

            current_object = &(result->objects[result->object_count - 1]);
            vertex_index = 0;
        }
    }

    boaster_buffer_destroy(position_buffer);
    boaster_buffer_destroy(normal_buffer);

    free(line_buffer);
    return result;
}

void objmesh_save(FILE *file, objmesh_t *mesh) {
    for (int i = 0; i < mesh->object_count; ++i) {
        objmesh_object_t object = mesh->objects[i];

        fprintf(file, "o %s\n", object.name);
        parrot_vertex_t *vertices =
            (parrot_vertex_t*) object.vertex_buffer->data;
        int vertex_count =
            object.vertex_buffer->__size / sizeof(parrot_vertex_t);

        for (int j = 0; j < vertex_count; ++j) {
            parrot_vertex_t vertex = vertices[j];

            fprintf(file, "v %f %f %f\n",
                vertex.position[0], vertex.position[1], vertex.position[2]);
            fprintf(file, "vn %f %f %f\n",
                vertex.normal[0], vertex.normal[1], vertex.normal[2]);
        }

        for (int j = 0; j < vertex_count; j += 3) {
            fprintf(file, "f %d//%d %d//%d %d//%d\n",
                j + 1, j + 1,
                j + 2, j + 2,
                j + 3, j + 3);
        }
    }
}
