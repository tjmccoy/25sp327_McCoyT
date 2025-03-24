# Makefile
CC = gcc
CFLAGS = -Wall -Wextra

all: parent child

parent: parent.c
	$(CC) $(CFLAGS) -o parent parent.c

child: child.c
	$(CC) $(CFLAGS) -o child child.c

run: all
	./parent

clean:
	rm -f parent child
