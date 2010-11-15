CC = gcc 
CFLAGS = -Wall -c -g 

MEMPROF = valgrind --leak-check=full --show-reachable=yes --track-origins=yes

TAGS = ctags

BIN_DIR = ./bin
SRC_DIR = ./src

EXE = $(BIN_DIR)/regex


all: $(EXE) tags

memory: $(EXE) 
	$(MEMPROF) $(EXE) anchorcllassallstail ^anchor[class]*tail$ 

$(EXE): $(BIN_DIR)/main.o $(BIN_DIR)/engine.o $(BIN_DIR)/stack.o
	$(CC) -o $(EXE) \
		$(BIN_DIR)/main.o \
		$(BIN_DIR)/engine.o \
		$(BIN_DIR)/stack.o

$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -o $@ $<

tags:
	$(TAGS) -R

clobber: 
	rm -f $(BIN_DIR)/*

clean:
	rm -f $(BIN_DIR)/*.o
