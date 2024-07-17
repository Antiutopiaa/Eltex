CC = gcc
AR = ar
CFLAGS = -Wall
LIB_SRC_DIR = libsrc
LIB_OBJ_DIR = libobj
SRC_DIR = src
OBJ_DIR = obj

LIB_SOURCES = $(wildcard $(LIB_SRC_DIR)/*.c)
LIB_OBJECTS = $(patsubst $(LIB_SRC_DIR)/%.c, $(LIB_OBJ_DIR)/%.o, $(LIB_SOURCES))
LIB_NAME = libcalc.a

all: $(LIB_NAME) main

$(LIB_NAME): $(LIB_OBJECTS)
	$(AR) rcs $@ $^

$(LIB_OBJ_DIR)/%.o: $(LIB_SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

main: $(OBJ_DIR)/main.o $(LIB_NAME)
	$(CC) $(CFLAGS) -o $@ $< -L. -lcalc

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(LIB_OBJ_DIR)/*.o $(LIB_NAME) $(OBJ_DIR)/*.o main

