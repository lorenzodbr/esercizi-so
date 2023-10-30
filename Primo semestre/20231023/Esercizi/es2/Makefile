NAME=es2
CFLAGS=-Wall -Werror

all: obj/es2.o

obj/es2.o: src/es2.c
	$(CC) $(CFLAGS) -c src/es2.c -o obj/es2.o

.PHONY: clean
clean:
	rm -f obj/es2.o bin/es2
