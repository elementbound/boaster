INCLUDE = -I. -Itestboat -Iboaster -Iboastgl
LDFLAGS = -L./testboat/lib -L./boaster/lib -L./boastgl/lib -L./boastmath/lib

all: modules samples

samples: main glmain executor parrotdemo

modules:
	$(MAKE) -C testboat
	$(MAKE) -C boaster
	$(MAKE) -C boastgl
	$(MAKE) -C boastmath

main: ensure_out
	$(CC) $(INCLUDE) $(LDFLAGS) \
		samples/main.c \
		-lm -lpthread -lboaster \
		-o out/main

glmain: ensure_out
	$(CC) $(INCLUDE) $(LDFLAGS) \
		samples/glmain.c \
		-lm -lboaster -lboastgl \
		-lglfw3 -lGL -pthread -ldl -lrt -lXrandr -lX11 \
		-o out/glmain

parrotdemo:
	$(MAKE) -C parrotdemo

executor: ensure_out
	$(CC) $(INCLUDE) $(LDFLAGS) \
		samples/executor.c \
		-lpthread -lboaster \
		-o out/executor

ensure_out:
	mkdir -p out

clean:
	rm -rf out

cleanall:
	rm -rf **/out
