INCLUDE = -I. -I../testboat
LDFLAGS = -L../testboat/lib -L./lib

all: boaster test

boaster: compile
	$(AR) rcs lib/libboaster.a \
		out/bin/boaster.o \
		out/bin/buffer.o \
		out/bin/executor.o \
		out/bin/format.o \
		out/bin/image.o \
		out/bin/interpolator.o \
		out/bin/property.o \
		out/bin/varray.o

clean:
	rm -rf out

compile: ensure_out
	$(CC) $(INCLUDE) $(CFLAGS) -c src/boaster.c -o out/bin/boaster.o
	$(CC) $(INCLUDE) $(CFLAGS) -c src/buffer.c -o out/bin/buffer.o
	$(CC) $(INCLUDE) $(CFLAGS) -c src/executor.c -o out/bin/executor.o
	$(CC) $(INCLUDE) $(CFLAGS) -c src/format.c -o out/bin/format.o
	$(CC) $(INCLUDE) $(CFLAGS) -c src/image.c -o out/bin/image.o
	$(CC) $(INCLUDE) $(CFLAGS) -c src/interpolator.c -o out/bin/interpolator.o
	$(CC) $(INCLUDE) $(CFLAGS) -c src/property.c -o out/bin/property.o
	$(CC) $(INCLUDE) $(CFLAGS) -c src/varray.c -o out/bin/varray.o

test: test.build test.run

test.run:
	out/test/buffer_tests
	out/test/varray_tests
	out/test/image_tests
	out/test/vertex_shader_tests
	out/test/format_tests

test.build: ensure_out
	$(CC) $(INCLUDE) $(LDFLAGS) \
		test/buffer_tests.c \
		-lboaster -ltestboat -lpthread \
		-o out/test/buffer_tests
	
	$(CC) $(INCLUDE) $(LDFLAGS) \
		test/image_tests.c \
		-lboaster -ltestboat -lpthread \
		-o out/test/image_tests
		
	$(CC) $(INCLUDE) $(LDFLAGS) \
		test/format_tests.c \
		-lboaster -ltestboat -lpthread \
		-o out/test/format_tests
		
	$(CC) $(INCLUDE) $(LDFLAGS) \
		test/vertex_shader_tests.c \
		-lboaster -ltestboat -lpthread -lm \
		-o out/test/vertex_shader_tests
		
	$(CC) $(INCLUDE) $(LDFLAGS) \
		test/varray_tests.c \
		-lboaster -ltestboat -lpthread \
		-o out/test/varray_tests

ensure_out:
	mkdir -p out
	mkdir -p out/bin
	mkdir -p out/test
	mkdir -p lib
