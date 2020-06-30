INCLUDE = -I. -Itestboat
LDFLAGS = -L./testboat/lib

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

test: test.boastmath compile test.buffer test.image test.vertex_shader test.varray test.format
	out/test/buffer_tests
	out/test/varray_tests
	out/test/image_tests
	out/test/vertex_shader_tests
	out/test/format_tests

test.boastmath: compile test.matrix
	out/test/boastmath/matrix_tests

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

	$(CC) $(INCLUDE) $(CFLAGS) -c src/boastgl/boastgl.c -o out/bin/boastgl/boastgl.o

	$(CC) $(INCLUDE) $(CFLAGS) -c src/boastmath/vector.c -o out/bin/boastmath/vector.o
	$(CC) $(INCLUDE) $(CFLAGS) -c src/boastmath/matrix.c -o out/bin/boastmath/matrix.o

_ensure_out:
	mkdir -p out
	mkdir -p out/bin/test
	mkdir -p out/bin/boaster
	mkdir -p out/bin/boastgl
	mkdir -p out/bin/boastmath
	mkdir -p out/test
	mkdir -p out/test/boastmath
	mkdir -p out/boastmath

clean:
	rm -rf out

run: main
	out/main

test.buffer:
	$(CC) $(INCLUDE) $(LDFLAGS) \
		out/bin/boaster/buffer.o \
		test/buffer_tests.c \
		-ltestboat \
		-o out/test/buffer_tests

test.image:
	$(CC) $(INCLUDE) $(LDFLAGS) \
		out/bin/boaster/format.o \
		out/bin/boaster/interpolator.o \
		out/bin/boaster/property.o \
		out/bin/boaster/varray.o \
		out/bin/boaster/image.o \
		test/image_tests.c \
		-ltestboat \
		-o out/test/image_tests

test.format:
	$(CC) $(INCLUDE) $(LDFLAGS) \
		out/bin/boaster/interpolator.o \
		out/bin/boaster/varray.o \
		out/bin/boaster/format.o \
		out/bin/boaster/property.o \
		test/format_tests.c \
		-ltestboat \
		-o out/test/format_tests

test.vertex_shader:
	$(CC) $(INCLUDE) $(LDFLAGS) \
		out/bin/boaster/buffer.o \
		out/bin/boaster/executor.o \
		out/bin/boaster/image.o \
		out/bin/boaster/interpolator.o \
		out/bin/boaster/boaster.o \
		out/bin/boaster/varray.o \
		out/bin/boaster/format.o \
		out/bin/boaster/property.o \
		test/vertex_shader_tests.c \
		-lm -lpthread -ltestboat \
		-o out/test/vertex_shader_tests

test.varray:
	$(CC) $(INCLUDE) $(LDFLAGS) \
		out/bin/boaster/varray.o \
		test/varray_tests.c \
		-ltestboat -lm \
		-o out/test/varray_tests

test.matrix:
	$(CC) $(INCLUDE) $(LDFLAGS) \
		out/bin/boastmath/matrix.o \
		out/bin/boastmath/vector.o \
		test/boastmath/matrix_tests.c \
		-ltestboat -lm \
		-o out/test/boastmath/matrix_tests