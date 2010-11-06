CC = gcc 
CFLAGS = -c -g

BIN_DIR = ./bin
SRC_DIR = ./src


regex: clean main.o engine.o
	$(CC) -o $(BIN_DIR)/regex $(BIN_DIR)/main.o $(BIN_DIR)/engine.o

main.o: $(SRC_DIR)/main.c engine.o
	$(CC) $(CFLAGS) -o $(BIN_DIR)/main.o $(SRC_DIR)/main.c

engine.o: $(SRC_DIR)/engine.c $(SRC_DIR)/engine.h
	$(CC) $(CFLAGS) -o $(BIN_DIR)/engine.o $(SRC_DIR)/engine.c

clean:
	rm $(BIN_DIR)/*
