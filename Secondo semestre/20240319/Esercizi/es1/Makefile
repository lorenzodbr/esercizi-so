CC = gcc
CFLAGS = -Wall -pedantic -std=c99
SRC = src/es1.c
OBJ = obj/es1.o
BIN = bin/es1

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
