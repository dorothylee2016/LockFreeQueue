.PHONY: clean

build/queue.o: src/queue.c
	mkdir -p build
	gcc -I include/ -c src/queue.c -o build/queue.o

build/main.o: src/main.c
	mkdir -p build
	gcc -I include/ -c src/main.c -o build/main.o

exe/main: build/queue.o build/main.o
	mkdir -p exe
	gcc -I include -o exe/main build/queue.o build/main.o
