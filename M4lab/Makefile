CC=gcc
CFLAGS=-g -ggdb -gdwarf-4 -O0 -Wall -Wextra -DUNIT_TEST
LIBS=-lcmocka

all: test_input

test_input: input.o test_input.o
	@echo "\nLink step:"
	gcc $(LDFLAGS) -o test_input test_input.o input.o $(LIBS) 

input.o: input.c
	gcc $(CFLAGS) -c input.c

test_list.o: test_input.c
	gcc $(CFLAGS) -c test_input.c

test: test_input
	@echo
	./test_input

clean:
	@echo
	rm -fv test_input *.o
	@echo

# make sure to mark phony targets in the case that you have a file that shares a name with one of these targets
.PHONY: all clean