CC = gcc
CFLAGS = -Wall -pedantic -std=c99 -lpthread
SRC = src/es2b.c
OBJ = obj/es2b.o
BIN = bin/es2b

all: $(BIN)

$(BIN): $(OBJ)
	@echo "Linking $@..."
	@$(CC) $(CFLAGS) -o $@ $^
	@echo "Done."

$(OBJ): $(SRC)
	@echo "Compiling $@..."
	@$(CC) $(CFLAGS) -c -o $@ $^

PHONY: clean

clean:
	@echo "Cleaning..."
	@rm -f $(OBJ) $(BIN)
	@echo "Done."
