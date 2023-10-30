NAME=es4
CFLAGS=-Wall -Werror

all: obj/es4.o

obj/es4.o: src/es4.c
	$(CC) $(CFLAGS) -c src/es4.c -o obj/es4.o

.PHONY: clean
clean:
	rm -f obj/es4.o bin/es4
