INCLUDE = -I.

build: compile
	$(CC) $(INCLUDE) \
		out/bin/buffer.o \
		src/main.c -o out/main

test: compile

compile: _ensure_out
	$(CC) $(INCLUDE) $(CFLAGS) -c src/buffer.c -o out/bin/buffer.o

_ensure_out:
	mkdir -p out
	mkdir -p out/bin

clean:
	rm -rf out

run: build
	out/main