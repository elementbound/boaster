INCLUDE = -I. -I../boaster

boastgl: compile
	$(AR) rcs lib/libboastgl.a \
		out/bin/boastgl.o

compile: ensure_out
	$(CC) $(INCLUDE) $(CFLAGS) -c src/boastgl.c -o out/bin/boastgl.o

ensure_out:
	mkdir -p out/bin
	mkdir -p lib

clean:
	rm -rf out lib
