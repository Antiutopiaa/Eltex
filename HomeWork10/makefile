CC = gcc
CFLAGS = -Wall -Werror

main: main.c 1/task1 2/task2 3/task3
	$(CC) $(CFLAGS) main.c -o main

1/task1: 1/task1.c
	$(CC) $(CFLAGS) 1/task1.c -o 1/task1

2/task2: 2/task2.c
	$(CC) $(CFLAGS) 2/task2.c -o 2/task2

3/task3: 3/task3.c
	$(CC) $(CFLAGS) 3/task3.c -o 3/task3

.PHONY: clean

clean:
	rm 1/task1 2/task2 3/task3 main
