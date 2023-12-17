CC = gcc
CFLAGS = -Wall -pedantic -std=c99 -lpthread
SRC = src/es1b.c
OBJ = obj/es1b.o
BIN = bin/es1b

all: $(BIN)

$(BIN): $(OBJ)
	@echo "Linking $@..."
	@$(CC) $(CFLAGS) -o $@ $^
	@echo "Done."

$(OBJ): $(SRC)
	@echo "Compiling $@ (with flags: $(CFLAGS))"
	@$(CC) $(CFLAGS) -c -o $@ $^

PHONY: clean

clean:
	@echo "Cleaning..."
	@rm -f $(OBJ) $(BIN)
	@echo "Done."
