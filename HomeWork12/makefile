CC = gcc
CFLAGS = -Wall -Werror

main: main.c 1/task1 2/server 2/client 3/task3
	$(CC) $(CFLAGS) main.c -o main

1/task1: 1/task1.c
	$(CC) $(CFLAGS) 1/task1.c -o 1/task1

2/client: 2/client.c
	$(CC) $(CFLAGS) 2/client.c -o 2/client

2/server: 2/server.c
	$(CC) $(CFLAGS) 2/server.c -o 2/server

3/task3: 3/task3.c
	$(CC) $(CFLAGS) 3/task3.c -o 3/task3

.PHONY: clean

clean:
	-rm 1/task1 2/server 2/client 3/task3 main
