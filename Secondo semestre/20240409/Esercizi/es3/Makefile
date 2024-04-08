CC = gcc
CFLAGS = -Wall -pedantic
SRC = src/es3.c
OBJ = obj/es3.o
BIN = bin/es3

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
