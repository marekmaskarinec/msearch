WARNINGS=-Wall -Wextra -Werror -Wno-unused-parameter -Wno-implicit-fallthrough -Wno-unused-result -Wno-old-style-declaration
LDFLAGS=-lm -lX11 -L /lib64 -lGL -ldl

FULL=$(WARNINGS) $(LDFLAGS) -g

OBJS=$(patsubst src/%.o, src/%.c, $(wildcard src/*.c))

src/%.o: src/%.c
	$(CC) $@ -c $< $(FULL)

build: $(OBJS)
	$(CC) $(FULL) $(OBJS) -o msearch

run: build
	dmenu_path | ./msearch

install: build
	sudo cp msearch_run /usr/bin
	sudo cp msearch /usr/bin
