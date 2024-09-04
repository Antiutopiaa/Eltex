# Определяем компилятор и флаги
CC = gcc
CFLAGS = -fstack-protector-all -Wall -Wextra -g
LFLAGS = -lpthread -lncurses

# Определяем каталоги
CLIENT_DIR = client
SERVER_DIR = server
BUILD_DIR = build
CLIENT_OBJECT_DIR = $(CLIENT_DIR)/object
SERVER_OBJECT_DIR = $(SERVER_DIR)/object

# Определяем целевые файлы
CLIENT_TARGET = $(BUILD_DIR)/client
SERVER_TARGET = $(BUILD_DIR)/server

# Находим все .c файлы в каталогах
CLIENT_SOURCES = $(wildcard $(CLIENT_DIR)/*.c)
SERVER_SOURCES = $(wildcard $(SERVER_DIR)/*.c)

# Определяем объектные файлы
CLIENT_OBJECTS = $(CLIENT_SOURCES:$(CLIENT_DIR)/%.c=$(CLIENT_OBJECT_DIR)/%.o)
SERVER_OBJECTS = $(SERVER_SOURCES:$(SERVER_DIR)/%.c=$(SERVER_OBJECT_DIR)/%.o)

# Основные цели
all: $(CLIENT_TARGET) $(SERVER_TARGET)

$(CLIENT_TARGET): $(CLIENT_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

$(SERVER_TARGET): $(SERVER_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ $(LFLAGS)

# Правила для компиляции объектных файлов
$(CLIENT_OBJECT_DIR)/%.o: $(CLIENT_DIR)/%.c
	mkdir -p $(CLIENT_OBJECT_DIR)  # Создаем каталог client/object, если он не существует
	$(CC) $(CFLAGS) -c $< -o $@

$(SERVER_OBJECT_DIR)/%.o: $(SERVER_DIR)/%.c
	mkdir -p $(SERVER_OBJECT_DIR)  # Создаем каталог server/object, если он не существует
	$(CC) $(CFLAGS) -c $< -o $@

# Очистка
clean:
	rm -f $(CLIENT_OBJECT_DIR)/*.o $(SERVER_OBJECT_DIR)/*.o $(CLIENT_TARGET) $(SERVER_TARGET)

.PHONY: all clean

