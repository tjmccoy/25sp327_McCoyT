CC=gcc
LINKER=gcc
CFLAGS=-Wall -Wextra -O0
LDFLAGS=-g
LIBS=-lpthread

queue.o: queue.c common.h queue_internal.h queue.h 
	$(CC) $(CFLAGS) -c queue.c -o queue.o 

test_queue.o: test_queue.c queue_internal.h queue.h
	$(CC) $(CFLAGS) -c test_queue.c -o test_queue.o

test_queue: test_queue.c queue.o
	$(LINKER) $(LDFLAGS) test_queue.o queue.o -o test_queue -lcmocka

clean:
	rm -fr *.o test_queue

all: queue.o test_queue.o test_queue