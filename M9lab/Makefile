CC=gcc
CFLAGS=-Wall -Wextra
LDFLAGS=-lpthread	# LDFLAGS is for the linker
ALL=primes1

all:${ALL}

#####################################################################################################################
# to get dependencies, do gcc -MM source.c, and it will display the .c file's dependencies							#
# e.g. 																												#
# in test_mymalloc, we do gcc -MM test_mymalloc.c, and "test_mymalloc.o: test_mymalloc.c mymalloc.h" is displayed.	#
#####################################################################################################################
primes1: primes1.c
	${CC} ${CFLAGS} primes1.c -o primes1 ${LDFLAGS}
clean:
	rm -vf *.o ${ALL}