CC = gcc
CFLAGS = -Wall -Werror 

# Цели по умолчанию
all: 1/client 1/server 2/client 2/server 3/build/client 3/build/server

1/client: 1/client.c
	$(CC) $(CFLAGS) 1/client.c -o 1/client

1/server: 1/server.c
	$(CC) $(CFLAGS) 1/server.c -o 1/server

2/client: 2/client.c
	$(CC) $(CFLAGS) 2/client.c -o 2/client -lrt

2/server: 2/server.c
	$(CC) $(CFLAGS) 2/server.c -o 2/server

3/build/server: 3/server/main.c
	make -C 3/

3/build/client: 3/client/main.c 3/client/pseudographics.c
	make -C 3/

.PHONY: clean

clean:
	rm -f 1/client 1/server 2/server 2/client 3/build/client 3/build/server 3/client/object/* 3/server/object/*

