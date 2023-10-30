NAME=es3
CFLAGS=-Wall -Werror

all: obj/es3.o

obj/es3.o: src/es3.c
	$(CC) $(CFLAGS) -c src/es3.c -o obj/es3.o

.PHONY: clean
clean:
	rm -f obj/es3.o bin/es3
