INCLUDE = -I.

build: compile
	$(CC) $(INCLUDE) \
		out/bin/buffer.o \
		src/main.c -o out/main

test: compile test.buffer
	out/test/buffer_tests

compile: _ensure_out
	$(CC) $(INCLUDE) $(CFLAGS) -c src/buffer.c -o out/bin/buffer.o

_ensure_out:
	mkdir -p out
	mkdir -p out/bin
	mkdir -p out/test

clean:
	rm -rf out

run: build
	out/main

test.buffer:
	$(CC) $(INCLUDE) \
		out/bin/buffer.o \
		test/buffer_tests.c -o out/test/buffer_tests
