.PHONY: clean

build/queue.o: src/queue.c
	mkdir -p build
	gcc -I include/ -c src/queue.c -o build/queue.o
