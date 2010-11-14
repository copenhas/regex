CC = gcc 
CFLAGS = -Wall -c -g

BIN_DIR = ./bin
SRC_DIR = ./src


all: regex tags

regex: main.o engine.o stack.o
	$(CC) -o $(BIN_DIR)/regex \
		$(BIN_DIR)/main.o \
		$(BIN_DIR)/engine.o \
		$(BIN_DIR)/stack.o

main.o: $(SRC_DIR)/main.c engine.o
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $<

engine.o: $(SRC_DIR)/engine.c $(SRC_DIR)/engine.h
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $< 

stack.o: $(SRC_DIR)/stack.c $(SRC_DIR)/stack.h
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $<

tags:
	ctags -R

clean:
	rm -f $(BIN_DIR)/*
