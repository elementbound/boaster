INCLUDE = -I. -I../boaster -I../boastgl -I../boastmath
LDFLAGS = -L../boaster/lib -L../boastgl/lib -L../boastmath/lib
CFLAGS = -ggdb

parrot: compile
	$(CC) $(CFLAGS) $(INCLUDE) $(LDFLAGS) \
		out/bin/mesh_obj.o \
		out/bin/shaders.o \
		out/bin/vertex.o \
		src/main.c \
		-lboaster -lboastgl -lboastmath -lpthread -lm \
		-lglfw3 -lGL -pthread -ldl -lrt -lXrandr -lX11 \
		-o out/parrot

compile: ensure_out
	$(CC) $(CFLAGS) $(INCLUDE) -c src/mesh_obj.c -o out/bin/mesh_obj.o
	$(CC) $(CFLAGS) $(INCLUDE) -c src/shaders.c -o out/bin/shaders.o
	$(CC) $(CFLAGS) $(INCLUDE) -c src/vertex.c -o out/bin/vertex.o

ensure_out:
	mkdir -p out
	mkdir -p out/bin

clean:
	rm -rf out
