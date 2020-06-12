# Boaster

Boaster is a software rasterizer written in C, for the heck of it

## Concepts

### Vertex Buffers

For anything to be drawn, we need the actual 3D object described by triangles. Vertex buffers contain all the vertices of that object.

Vertices can have any amount and kind of data.

### Vertex Shaders

As in conventional pipelines, vertex shaders take a single vertex and other data bound to the draw call, and produce some output.

This output will determine the vertices' position on the screen, and may produce other data useful for the pixel shader.

### Pixel Shaders

Pixel shaders take the output of the vertex shaders and produce data to be written into the target buffer, with the buffer often being the screen.

### Draw call

A draw call contains all the necessary info for drawing a given primitive.