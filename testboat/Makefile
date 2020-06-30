INCLUDE = -I.

testboat: compile
	$(AR) rcs lib/libtestboat.a out/bin/testboat.o

compile: ensure_out
	$(CC) $(INCLUDE) -c src/testboat.c -o out/bin/testboat.o

ensure_out:
	mkdir -p out
	mkdir -p out/bin
	mkdir -p lib

clean:
	rm -rf out
