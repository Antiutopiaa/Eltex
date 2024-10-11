# Определение компилятора и флагов
CC = gcc
CFLAGS = -Wall -Wextra

# Директории
MULTIPLEXING_DIR = multiplexing
PARALLEL_DIR = parallel
PARALLEL_POOL_DIR = parallel_pool
PARALLEL_QUEUE_DIR = parallel_queue

# Исполняемые файлы
TCP_CLIENT = $(MULTIPLEXING_DIR)/x/tcp_client
UDP_CLIENT = $(MULTIPLEXING_DIR)/x/udp_client
MULTIPLEXING_SERVER = $(MULTIPLEXING_DIR)/x/server

PARALLEL_CLIENT = $(PARALLEL_DIR)/x/client
PARALLEL_SERVER = $(PARALLEL_DIR)/x/server

PARALLEL_POOL_CLIENT = $(PARALLEL_POOL_DIR)/x/client
PARALLEL_POOL_SERVER = $(PARALLEL_POOL_DIR)/x/server

PARALLEL_QUEUE_CLIENT = $(PARALLEL_QUEUE_DIR)/x/client
PARALLEL_QUEUE_SERVER = $(PARALLEL_QUEUE_DIR)/x/server

# Цели по умолчанию
all: x $(TCP_CLIENT) $(UDP_CLIENT) $(MULTIPLEXING_SERVER) \
     $(PARALLEL_CLIENT) $(PARALLEL_SERVER) \
     $(PARALLEL_POOL_CLIENT) $(PARALLEL_POOL_SERVER) \
     $(PARALLEL_QUEUE_CLIENT) $(PARALLEL_QUEUE_SERVER)

# Создание директории x, если она не существует
x:
	mkdir -p multiplexing/x parallel/x parallel_pool/x parallel_queue/x

# Правила сборки для multiplexing
$(TCP_CLIENT): $(MULTIPLEXING_DIR)/client/tcp_client.c | x
	$(CC) $(CFLAGS) $< -o $@

$(UDP_CLIENT): $(MULTIPLEXING_DIR)/client/udp_client.c | x
	$(CC) $(CFLAGS) $< -o $@

$(MULTIPLEXING_SERVER): $(MULTIPLEXING_DIR)/server/main.c | x
	$(CC) $(CFLAGS) $< -o $@

# Правила сборки для parallel
$(PARALLEL_CLIENT): $(PARALLEL_DIR)/client/main.c | x
	$(CC) $(CFLAGS) $< -o $@

$(PARALLEL_SERVER): $(PARALLEL_DIR)/server/main.c | x
	$(CC) $(CFLAGS) $< -o $@

# Правила сборки для parallel_pool
$(PARALLEL_POOL_CLIENT): $(PARALLEL_POOL_DIR)/client/main.c | x
	$(CC) $(CFLAGS) $< -o $@

$(PARALLEL_POOL_SERVER): $(PARALLEL_POOL_DIR)/server/main.c | x
	$(CC) $(CFLAGS) $< -o $@

# Правила сборки для parallel_queue
$(PARALLEL_QUEUE_CLIENT): $(PARALLEL_QUEUE_DIR)/client/main.c | x
	$(CC) $(CFLAGS) $< -o $@

$(PARALLEL_QUEUE_SERVER): $(PARALLEL_QUEUE_DIR)/server/main.c | x
	$(CC) $(CFLAGS) $< -o $@

# Очистка скомпилированных файлов
clean:
	rm -f \
	$(TCP_CLIENT) \
	$(UDP_CLIENT) \
	$(MULTIPLEXING_SERVER) \
	$(PARALLEL_CLIENT) \
	$(PARALLEL_SERVER) \
	$(PARALLEL_POOL_CLIENT) \
	$(PARALLEL_POOL_SERVER) \
	$(PARALLEL_QUEUE_CLIENT) \
	$(PARALLEL_QUEUE_SERVER)

.PHONY: all clean x
