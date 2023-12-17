CC = gcc
CFLAGS = -Wall -pedantic -std=c99
SRC = src/es3.c
OBJ = obj/es3.o
BIN = bin/es3

all: $(BIN)

$(BIN): $(OBJ)
  @echo "Linking 20231217 3..."
	@$(CC) $(CFLAGS) -o $@ $^
  @echo "Done."

$(OBJ): $(SRC)
  @echo "Compiling 20231217 3..."
	@$(CC) $(CFLAGS) -c -o $@ $^

PHONY: clean

clean:
	@echo "Cleaning..."
	@rm -f $(OBJ) $(BIN)
	@echo "Done."
