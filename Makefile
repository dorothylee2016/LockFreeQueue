.PHONY: clean

DEFAULT: exe/main exe/mian_lock

build/queue.o: src/queue.c
	mkdir -p build
	gcc -I include/ -c src/queue.c -o build/queue.o

build/main.o: src/main.c
	mkdir -p build
	gcc -pthread -I include/ -c src/main.c -o build/main.o

exe/main: build/queue.o build/main.o
	mkdir -p exe
	gcc -pthread -I include -o exe/main build/queue.o build/main.o

build/debug_queue.o: src/queue.c
	mkdir -p build
	gcc -g -I include/ -c src/queue.c -o build/debug_queue.o

build/debug_main.o: src/main.c
	mkdir -p build
	gcc -g -pthread -I include/ -c src/main.c -o build/debug_main.o

exe/debug_main: build/debug_queue.o build/debug_main.o
	mkdir -p exe
	gcc -g -pthread -I include -o exe/debug_main build/debug_queue.o build/debug_main.o

build/spinlock.o: src/spinlock.c
	mkdir -p build
	gcc -I include/ -c src/spinlock.c -o build/spinlock.o

build/queue_lock.o: src/queue_lock.c
	mkdir -p build
	gcc -I include/ -c src/queue_lock.c -o build/queue_lock.o

exe/main_lock: build/queue_lock.o build/spinlock.o build/main.o
	mkdir -p exe
	gcc -pthread -I include/ -o exe/main_lock build/queue_lock.o build/spinlock.o build/main.o
