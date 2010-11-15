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

%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$@ $<

tags:
	ctags -R

clean:
	rm -f $(BIN_DIR)/*
