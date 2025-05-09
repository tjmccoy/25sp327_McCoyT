CC = gcc
LINKER = gcc
CFLAGS = -Wall -Wextra -O0
LDFLAGS = -g
LIBS = -lpthread
OBJS = main.o thread_pool.o worker_thread.o queue.o tasks.o
TEST_OBJS = test_queue.o queue.o
TARGETS = request_server test_queue

all: $(TARGETS)

request_server: $(OBJS)
	$(LINKER) $(LDFLAGS) $(OBJS) $(LIBS) -o request_server

test_queue: test_queue.o queue.o
	$(LINKER) $(LDFLAGS) test_queue.o queue.o -o test_queue -lcmocka -Wl,--wrap=pthread_mutex_lock,--wrap=pthread_mutex_unlock

main.o: main.c common.h queue.h tasks.h thread_pool.h worker_thread.h
	$(CC) $(CFLAGS) -c main.c -o main.o

thread_pool.o: thread_pool.c thread_pool.h queue.h worker_thread.h common.h
	$(CC) $(CFLAGS) -c thread_pool.c -o thread_pool.o

worker_thread.o: worker_thread.c worker_thread.h queue.h common.h
	$(CC) $(CFLAGS) -c worker_thread.c -o worker_thread.o

queue.o: queue.c queue_internal.h queue.h common.h
	$(CC) $(CFLAGS) -c queue.c -o queue.o

tasks.o: tasks.c tasks.h common.h
	$(CC) $(CFLAGS) -c tasks.c -o tasks.o

test_queue.o: test_queue.c queue_internal.h queue.h
	$(CC) $(CFLAGS) -c test_queue.c -o test_queue.o

clean:
	rm -fr *.o $(OBJS) $(TARGETS)
