CC=gcc

make: main.c
	$(CC) -lncurses timer.c stopwatch.c main.c -o timer
