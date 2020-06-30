INCLUDE = -I. -Itestboat -Iboaster -Iboastgl
LDFLAGS = -L./testboat/lib -L./boaster/lib -L./boastgl/lib -L./boastmath/lib

all: main glmain executor

main: compile
	$(CC) $(INCLUDE) $(LDFLAGS) \
		out/bin/millitime.o \
		src/main.c \
		-lm -lpthread -lboaster \
		-o out/main

glmain: compile
	$(CC) $(INCLUDE) $(LDFLAGS) \
		out/bin/millitime.o \
		src/glmain.c \
		-lm -lboaster -lboastgl \
		-lglfw3 -lGL -pthread -ldl -lrt -lXrandr -lX11 \
		-o out/glmain

executor: compile
	$(CC) $(INCLUDE) $(LDFLAGS) \
		src/executor.c \
		-lpthread -lboaster \
		-o out/executor

compile: _ensure_out
	$(CC) $(INCLUDE) $(CFLAGS) -c src/millitime.c -o out/bin/millitime.o

_ensure_out:
	mkdir -p out
	mkdir -p out/bin

clean:
	rm -rf out
