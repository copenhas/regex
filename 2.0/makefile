CC = gcc
CFLAGS = -c -g

BIN_DIR = ./bin
SRC_DIR = ./src
INCLUDES = ./include

regex: clean main.o regex.o
	$(CC) -o $(BIN_DIR)/regex $(BIN_DIR)/*.o

main.o: $(SRC_DIR)/main.c 
	$(CC) $(CFLAGS) -o $(BIN_DIR)/main.o $(SRC_DIR)/main.c

regex.o: $(SRC_DIR)/regex.c
	$(CC) $(CFLAGS) -o $(BIN_DIR)/regex.o $(SRC_DIR)/regex.c

clean:
	rm $(BIN_DIR)/*
