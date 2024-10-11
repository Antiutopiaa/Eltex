# Определение компилятора и флагов
CC = gcc
CFLAGS = -Wall -Wextra

# Директории
IP_DIR = ip
MAC_DIR = mac
RECEIVER_DIR = reciver
SERVER_DIR = server
UDP_DIR = udp

# Исполняемые файлы
IP_EXEC = x/ip
MAC_EXEC = x/mac
RECEIVER_EXEC = x/receiver
SERVER_EXEC = x/server
UDP_EXEC = x/udp

# Исходные файлы
IP_SRC = $(IP_DIR)/main.c
MAC_SRC = $(MAC_DIR)/main.c
RECEIVER_SRC = $(RECEIVER_DIR)/main.c
SERVER_SRC = $(SERVER_DIR)/main.c
UDP_SRC = $(UDP_DIR)/main.c

# Основная цель
.PHONY: all clean

all: $(IP_EXEC) $(MAC_EXEC) $(RECEIVER_EXEC) $(SERVER_EXEC) $(UDP_EXEC)

$(IP_EXEC): $(IP_SRC)
	@mkdir -p x
	$(CC) $(CFLAGS) -o $@ $<

$(MAC_EXEC): $(MAC_SRC)
	@mkdir -p x
	$(CC) $(CFLAGS) -o $@ $<

$(RECEIVER_EXEC): $(RECEIVER_SRC)
	@mkdir -p x
	$(CC) $(CFLAGS) -o $@ $<

$(SERVER_EXEC): $(SERVER_SRC)
	@mkdir -p x
	$(CC) $(CFLAGS) -o $@ $<

$(UDP_EXEC): $(UDP_SRC)
	@mkdir -p x
	$(CC) $(CFLAGS) -o $@ $<

clean:
	rm -f x/ip x/mac x/receiver x/server x/udp
	rm -rf x/

