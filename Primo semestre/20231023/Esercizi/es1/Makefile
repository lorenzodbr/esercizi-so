NAME=es1
CFLAGS=-Wall -Werror

all: obj/es1.o

obj/es1.o: src/es1.c
	$(CC) $(CFLAGS) -c src/es1.c -o obj/es1.o

.PHONY: clean
clean:
	rm -f obj/es1.o bin/es1
