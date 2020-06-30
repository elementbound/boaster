INCLUDE = -I. -I../testboat
LDFLAGS = -L../testboat/lib -L./lib

all: boastmath test

boastmath: compile
	$(AR) rcs lib/libboastmath.a \
		out/bin/matrix.o \
		out/bin/scalar.o \
		out/bin/vector.o

clean:
	rm -rf out lib

compile: ensure_out
	$(CC) $(INCLUDE) $(CFLAGS) -c src/matrix.c -o out/bin/matrix.o
	$(CC) $(INCLUDE) $(CFLAGS) -c src/scalar.c -o out/bin/scalar.o
	$(CC) $(INCLUDE) $(CFLAGS) -c src/vector.c -o out/bin/vector.o

test: test.build test.run

test.run:
	out/test/matrix_tests

test.build: ensure_out
	$(CC) $(INCLUDE) $(LDFLAGS) \
		test/matrix_tests.c \
		-ltestboat -lboastmath -lm \
		-o out/test/matrix_tests

ensure_out:
	mkdir -p out
	mkdir -p out/bin
	mkdir -p out/test
	mkdir -p lib
