INCLUDE = -I.

build: compile
	$(CC) $(INCLUDE) \
		out/bin/boaster/buffer.o \
		src/main.c -o out/main

test: compile test.buffer
	out/test/buffer_tests

compile: _ensure_out
	$(CC) $(INCLUDE) $(CFLAGS) -c src/test/test.c -o out/bin/test/test.o
	$(CC) $(INCLUDE) $(CFLAGS) -c src/boaster/buffer.c -o out/bin/boaster/buffer.o

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
