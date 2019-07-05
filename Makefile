CC=gcc

make: timer.c
	$(CC) -lncurses timer.c -o timer
