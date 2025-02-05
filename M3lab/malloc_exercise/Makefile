CC=gcc
CFLAGS=-Wall -Wextra
ALL=test_mymalloc

all:${ALL}

#####################################################################################################################
# to get dependencies, do gcc -MM source.c, and it will display the .c file's dependencies							#
# e.g. 																												#
# in test_mymalloc, we do gcc -MM test_mymalloc.c, and "test_mymalloc.o: test_mymalloc.c mymalloc.h" is displayed.	#
#####################################################################################################################
test_mymalloc: mymalloc.o test_mymalloc.o
	${CC} ${CFLAGS} -o test_mymalloc mymalloc.o test_mymalloc.o

test_mymalloc.o: test_mymalloc.c mymalloc.h
	${CC} ${CFLAGS} -c test_mymalloc.c -o test_mymalloc.o

mymalloc.o: mymalloc.c mymalloc.h
	${CC} ${CFLAGS} -c mymalloc.c -o mymalloc.o

clean:	# removes object files & executable file <- phony target
	rm -vf *.o ${ALL}