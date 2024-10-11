# Компилятор и флаги компиляции
CC = gcc
CFLAGS = -Wall -Wextra

# Директории для всех протоколов
TCP_INET_DIR = tcp_inet
TCP_LOCAL_DIR = tcp_local
UDP_INET_DIR = udp_inet
UDP_LOCAL_DIR = udp_local

# Исполняемые файлы в директории x для каждого протокола 
TCP_INET_X_CLIENT = $(TCP_INET_DIR)/x/client 
TCP_INET_X_SERVER = $(TCP_INET_DIR)/x/server 

TCP_LOCAL_X_CLIENT = $(TCP_LOCAL_DIR)/x/client 
TCP_LOCAL_X_SERVER = $(TCP_LOCAL_DIR)/x/server 

UDP_INET_X_CLIENT = $(UDP_INET_DIR)/x/client 
UDP_INET_X_SERVER = $(UDP_INET_DIR)/x/server 

UDP_LOCAL_X_CLIENT = $(UDP_LOCAL_DIR)/x/client 
UDP_LOCAL_X_SERVER = $(UDP_LOCAL_DIR)/x/server 

# Исходные файлы для каждого клиента и сервера 
TCP_INET_CLIENT_SRC = $(TCP_INET_DIR)/client/main.c 
TCP_INET_SERVER_SRC = $(TCP_INET_DIR)/server/main.c 

TCP_LOCAL_CLIENT_SRC = $(TCP_LOCAL_DIR)/client/main.c 
TCP_LOCAL_SERVER_SRC = $(TCP_LOCAL_DIR)/server/main.c 

UDP_INET_CLIENT_SRC = $(UDP_INET_DIR)/client/main.c 
UDP_INET_SERVER_SRC = $(UDP_INET_DIR)/server/main.c 

UDP_LOCAL_CLIENT_SRC = $(UDP_LOCAL_DIR)/client/main.c 
UDP_LOCAL_SERVER_SRC = $(UDP_LOCAL_DIR)/server/main.c 

# Основное правило сборки 
all: directories \
     $(TCP_INET_X_CLIENT) $(TCP_INET_X_SERVER) \
     $(TCP_LOCAL_X_CLIENT) $(TCP_LOCAL_X_SERVER) \
     $(UDP_INET_X_CLIENT) $(UDP_INET_X_SERVER) \
     $(UDP_LOCAL_X_CLIENT) $(UDP_LOCAL_X_SERVER)

# Создание необходимых директорий 
directories:
	mkdir -p $(TCP_INET_DIR)/x \
             $(TCP_LOCAL_DIR)/x \
             $(UDP_INET_DIR)/x \
             $(UDP_LOCAL_DIR)/x

# Правила компиляции для TCP Inet клиент и сервер 
$(TCP_INET_X_CLIENT): $(TCP_INET_CLIENT_SRC)
	$(CC) $(CFLAGS) $< -o $@

$(TCP_INET_X_SERVER): $(TCP_INET_SERVER_SRC)
	$(CC) $(CFLAGS) $< -o $@

# Правила компиляции для TCP Local клиент и сервер 
$(TCP_LOCAL_X_CLIENT): $(TCP_LOCAL_CLIENT_SRC)
	$(CC) $(CFLAGS) $< -o $@

$(TCP_LOCAL_X_SERVER): $(TCP_LOCAL_SERVER_SRC)
	$(CC) $(CFLAGS) $< -o $@

# Правила компиляции для UDP Inet клиент и сервер 
$(UDP_INET_X_CLIENT): $(UDP_INET_CLIENT_SRC)
	$(CC) $(CFLAGS) $< -o $@

$(UDP_INET_X_SERVER): $(UDP_INET_SERVER_SRC)
	$(CC) $(CFLAGS) $< -o $@

# Правила компиляции для UDP Local клиент и сервер 
$(UDP_LOCAL_X_CLIENT): $(UDP_LOCAL_CLIENT_SRC)
	$(CC) $(CFLAGS) $< -o $@

$(UDP_LOCAL_X_SERVER): $(UDP_LOCAL_SERVER_SRC)
	$(CC) $(CFLAGS) $< -o $@

# Очистка скомпилированных файлов (опционально)
clean:
	rm -f \
		$(TCP_INET_X_CLIENT) \
		$(TCP_INET_X_SERVER) \
		$(TCP_LOCAL_X_CLIENT) \
		$(TCP_LOCAL_X_SERVER) \
		$(UDP_INET_X_CLIENT) \
		$(UDP_INET_X_SERVER) \
		$(UDP_LOCAL_X_CLIENT) \
		$(UDP_LOCAL_X_SERVER)

.PHONY: all directories clean
