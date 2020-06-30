#ifndef PARROTDEMO_INCLUDE_MESH_OBJ_H_
#define PARROTDEMO_INCLUDE_MESH_OBJ_H_

#include <stddef.h>
#include <stdio.h>
#include "include/boaster/buffer.h"
#include "include/boaster/format.h"

typedef struct {
    char *name;
    boaster_buffer_t *vertex_buffer;
} objmesh_object_t;

typedef struct {
    objmesh_object_t *objects;
    size_t object_count;
    boaster_format_t *vertex_format;
} objmesh_t;

objmesh_t *objmesh_create();
void objmesh_destroy(objmesh_t *mesh);

objmesh_t *objmesh_load(FILE *file);
void objmesh_save(FILE *file, objmesh_t *mesh);

#endif  // PARROTDEMO_INCLUDE_MESH_OBJ_H_
