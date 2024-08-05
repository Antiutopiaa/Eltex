CC = gcc
CFLAGS = -Wall
SRC_DIR = source
OBJ_DIR = obj
BUILD_DIR = build

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SOURCES))


$(BUILD_DIR)/hw9: $(OBJECTS)
	-mkdir $(BUILD_DIR)
	$(CC) $(CFLAGS) $^ -o $@ -lncurses

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	-mkdir $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@ 

.PHONY: clean

clean:
	rm -rf $(OBJ_DIR)/* $(BUILD_DIR)/*
