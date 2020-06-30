INCLUDE = -I. -Itestboat -Iboaster -Iboastgl
LDFLAGS = -L./testboat/lib -L./boaster/lib -L./boastgl/lib

all: test main glmain executor

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

test: test.boastmath

test.boastmath: compile test.matrix
	out/test/boastmath/matrix_tests

compile: _ensure_out
	$(CC) $(INCLUDE) $(CFLAGS) -c src/millitime.c -o out/bin/millitime.o

	$(CC) $(INCLUDE) $(CFLAGS) -c src/boastmath/vector.c -o out/bin/boastmath/vector.o
	$(CC) $(INCLUDE) $(CFLAGS) -c src/boastmath/matrix.c -o out/bin/boastmath/matrix.o

_ensure_out:
	mkdir -p out
	mkdir -p out/bin/boastmath
	mkdir -p out/test
	mkdir -p out/test/boastmath
	mkdir -p out/boastmath

clean:
	rm -rf out

run: main
	out/main

test.matrix:
	$(CC) $(INCLUDE) $(LDFLAGS) \
		out/bin/boastmath/matrix.o \
		out/bin/boastmath/vector.o \
		test/boastmath/matrix_tests.c \
		-ltestboat -lm \
		-o out/test/boastmath/matrix_tests
