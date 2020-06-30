INCLUDE = -I. -Itestboat -Iboaster -Iboastgl
LDFLAGS = -L./testboat/lib -L./boaster/lib -L./boastgl/lib -L./boastmath/lib

all: main glmain executor

main:
	$(CC) $(INCLUDE) $(LDFLAGS) \
		src/main.c \
		-lm -lpthread -lboaster \
		-o out/main

glmain:
	$(CC) $(INCLUDE) $(LDFLAGS) \
		src/glmain.c \
		-lm -lboaster -lboastgl \
		-lglfw3 -lGL -pthread -ldl -lrt -lXrandr -lX11 \
		-o out/glmain

executor:
	$(CC) $(INCLUDE) $(LDFLAGS) \
		src/executor.c \
		-lpthread -lboaster \
		-o out/executor

_ensure_out:
	mkdir -p out

clean:
	rm -rf out
