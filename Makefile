INCLUDE = -I.

all: test main glmain executor

main: compile
	$(CC) $(INCLUDE) \
		out/bin/boaster/buffer.o \
		out/bin/boaster/executor.o \
		out/bin/boaster/image.o \
		out/bin/boaster/interpolator.o \
		out/bin/boaster/varray.o \
		out/bin/boaster/format.o \
		out/bin/boaster/property.o \
		out/bin/boaster/boaster.o \
		out/bin/millitime.o \
		-lm -lpthread \
		src/main.c -o out/main

glmain: compile
	$(CC) $(INCLUDE) \
		out/bin/boaster/buffer.o \
		out/bin/boaster/executor.o \
		out/bin/boaster/image.o \
		out/bin/boaster/interpolator.o \
		out/bin/boaster/varray.o \
		out/bin/boaster/format.o \
		out/bin/boaster/property.o \
		out/bin/boaster/boaster.o \
		out/bin/boastgl/boastgl.o \
		out/bin/millitime.o \
		-lm \
		-lglfw3 -lGL -pthread -ldl -lrt -lXrandr -lX11 \
		src/glmain.c -o out/glmain

executor: compile
	$(CC) $(INCLUDE) \
		out/bin/boaster/buffer.o \
		out/bin/boaster/executor.o \
		-lpthread \
		src/executor.c -o out/executor

test: compile test.buffer test.image test.vertex_shader test.varray test.format
	out/test/buffer_tests
	out/test/varray_tests
	out/test/image_tests
	out/test/vertex_shader_tests
	out/test/format_tests

compile: _ensure_out
	$(CC) $(INCLUDE) $(CFLAGS) -c src/boaster/boaster.c -o out/bin/boaster/boaster.o
	$(CC) $(INCLUDE) $(CFLAGS) -c src/boaster/buffer.c -o out/bin/boaster/buffer.o
	$(CC) $(INCLUDE) $(CFLAGS) -c src/boaster/executor.c -o out/bin/boaster/executor.o
	$(CC) $(INCLUDE) $(CFLAGS) -c src/boaster/format.c -o out/bin/boaster/format.o
	$(CC) $(INCLUDE) $(CFLAGS) -c src/boaster/image.c -o out/bin/boaster/image.o
	$(CC) $(INCLUDE) $(CFLAGS) -c src/boaster/interpolator.c -o out/bin/boaster/interpolator.o
	$(CC) $(INCLUDE) $(CFLAGS) -c src/boaster/property.c -o out/bin/boaster/property.o
	$(CC) $(INCLUDE) $(CFLAGS) -c src/boaster/varray.c -o out/bin/boaster/varray.o
	$(CC) $(INCLUDE) $(CFLAGS) -c src/millitime.c -o out/bin/millitime.o
	$(CC) $(INCLUDE) $(CFLAGS) -c src/test/test.c -o out/bin/test/test.o3

	$(CC) $(INCLUDE) $(CFLAGS) -c src/boastgl/boastgl.c -o out/bin/boastgl/boastgl.o

_ensure_out:
	mkdir -p out
	mkdir -p out/bin/test
	mkdir -p out/bin/boaster
	mkdir -p out/bin/boastgl
	mkdir -p out/test

clean:
	rm -rf out

run: main
	out/main

test.buffer:
	$(CC) $(INCLUDE) \
		out/bin/test/test.o \
		out/bin/boaster/buffer.o \
		test/buffer_tests.c -o out/test/buffer_tests

test.image:
	$(CC) $(INCLUDE) \
		out/bin/test/test.o \
		out/bin/boaster/format.o \
		out/bin/boaster/interpolator.o \
		out/bin/boaster/property.o \
		out/bin/boaster/varray.o \
		out/bin/boaster/image.o \
		test/image_tests.c -o out/test/image_tests

test.format:
	$(CC) $(INCLUDE) \
		out/bin/test/test.o \
		out/bin/boaster/interpolator.o \
		out/bin/boaster/varray.o \
		out/bin/boaster/format.o \
		out/bin/boaster/property.o \
		test/format_tests.c -o out/test/format_tests

test.vertex_shader:
	$(CC) $(INCLUDE) \
		out/bin/test/test.o \
		out/bin/boaster/buffer.o \
		out/bin/boaster/executor.o \
		out/bin/boaster/image.o \
		out/bin/boaster/interpolator.o \
		out/bin/boaster/boaster.o \
		out/bin/boaster/varray.o \
		out/bin/boaster/format.o \
		out/bin/boaster/property.o \
		-lm -lpthread \
		test/vertex_shader_tests.c -o out/test/vertex_shader_tests

test.varray:
	$(CC) $(INCLUDE) \
		out/bin/test/test.o \
		out/bin/boaster/varray.o \
		test/varray_tests.c -o out/test/varray_tests
