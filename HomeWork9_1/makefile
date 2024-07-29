CC = gcc
CFLAGS = -Wall
SRC_DIR = source
BLD_DIR = build

$(BLD_DIR)/main: $(SRC_DIR)/main.c 
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(BLD_DIR)/* file.txt
