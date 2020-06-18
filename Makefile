INCLUDE = -I.

build: compile
	$(CC) $(INCLUDE) \
		out/bin/boaster/buffer.o \
		out/bin/boaster/image.o \
		out/bin/boaster/interpolator.o \
		out/bin/boaster/varray.o \
		out/bin/boaster/vertex_format.o \
		out/bin/boaster/vertex_property.o \
		out/bin/boaster/boaster.o \
		-lm \
		src/main.c -o out/main

test: compile test.buffer test.image test.vertex_shader test.varray test.vertex_format
	out/test/buffer_tests
	out/test/varray_tests
	out/test/image_tests
	out/test/vertex_shader_tests
	out/test/vertex_format_tests

compile: _ensure_out
	$(CC) $(INCLUDE) $(CFLAGS) -c src/test/test.c -o out/bin/test/test.o
	$(CC) $(INCLUDE) $(CFLAGS) -c src/boaster/buffer.c -o out/bin/boaster/buffer.o
	$(CC) $(INCLUDE) $(CFLAGS) -c src/boaster/image.c -o out/bin/boaster/image.o
	$(CC) $(INCLUDE) $(CFLAGS) -c src/boaster/interpolator.c -o out/bin/boaster/interpolator.o
	$(CC) $(INCLUDE) $(CFLAGS) -c src/boaster/varray.c -o out/bin/boaster/varray.o
	$(CC) $(INCLUDE) $(CFLAGS) -c src/boaster/vertex_format.c -o out/bin/boaster/vertex_format.o
	$(CC) $(INCLUDE) $(CFLAGS) -c src/boaster/vertex_property.c -o out/bin/boaster/vertex_property.o
	$(CC) $(INCLUDE) $(CFLAGS) -c src/boaster/boaster.c -o out/bin/boaster/boaster.o

_ensure_out:
	mkdir -p out
	mkdir -p out/bin/test
	mkdir -p out/bin/boaster
	mkdir -p out/test

clean:
	rm -rf out

run: build
	out/main

test.buffer:
	$(CC) $(INCLUDE) \
		out/bin/test/test.o \
		out/bin/boaster/buffer.o \
		test/buffer_tests.c -o out/test/buffer_tests

test.image:
	$(CC) $(INCLUDE) \
		out/bin/test/test.o \
		out/bin/boaster/image.o \
		test/image_tests.c -o out/test/image_tests

test.vertex_format:
	$(CC) $(INCLUDE) \
		out/bin/test/test.o \
		out/bin/boaster/interpolator.o \
		out/bin/boaster/varray.o \
		out/bin/boaster/vertex_format.o \
		out/bin/boaster/vertex_property.o \
		test/vertex_format_tests.c -o out/test/vertex_format_tests

test.vertex_shader:
	$(CC) $(INCLUDE) \
		out/bin/test/test.o \
		out/bin/boaster/buffer.o \
		out/bin/boaster/image.o \
		out/bin/boaster/interpolator.o \
		out/bin/boaster/boaster.o \
		out/bin/boaster/varray.o \
		out/bin/boaster/vertex_format.o \
		out/bin/boaster/vertex_property.o \
		test/vertex_shader_tests.c -o out/test/vertex_shader_tests

test.varray:
	$(CC) $(INCLUDE) \
		out/bin/test/test.o \
		out/bin/boaster/varray.o \
		test/varray_tests.c -o out/test/varray_tests
