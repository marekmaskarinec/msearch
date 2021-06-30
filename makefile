WARNINGS=-Wall -Wextra -Werror -Wno-unused-parameter -Wno-implicit-fallthrough -Wno-unused-result -Wno-old-style-declaration
LDFLAGS=-lm -lX11 -L /lib64 -lGL -ldl

FULL=$(WARNINGS) $(LDFLAGS) -O3

OBJS=$(patsubst src/%.c, src/%.o, $(wildcard src/*.c))

src/%.o: src/%.c
	@echo "building $@"
	@$(CC) -o $@ -c $< $(FULL)

build: $(OBJS)
	@echo "building binary"
	@$(CC) $(FULL) $(OBJS) -o msearch

clean:
	rm -f msearch src/*.o

run: build
	dmenu_path | ./msearch

install: build
	sudo cp msearch_run /usr/bin
	sudo cp msearch /usr/bin
