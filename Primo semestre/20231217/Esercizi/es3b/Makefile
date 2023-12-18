CC = gcc
CFLAGS = -Wall -pedantic -std=c99
SRC = src/es3b.c
OBJ = obj/es3b.o
BIN = bin/es3b

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
