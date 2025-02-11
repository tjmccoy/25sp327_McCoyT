CC=gcc
COMPARE_CFLAGS=-g -Wall -Wextra
STATIC_CFLAGS=-static
ALL=compare_static compare_dynamic

all: $(ALL)

compare.o: compare.c
	$(CC) $(COMPARE_CFLAGS) -c compare.c

compare_static: compare.o
	$(CC) $(STATIC_CFLAGS) -o compare_static compare.o

compare_dynamic: compare.o
	$(CC) -o compare_dynamic compare.o

clean:
	$(RM) -v *.o compare_static compare_dynamic
	
compare:
	@echo "compare static:"
	@ls -l compare_static
	@echo "compare dynamic:"
	@ls -l compare_dynamic
	@./compare_static
	@./compare_dynamic

.PHONY: all clean compare
