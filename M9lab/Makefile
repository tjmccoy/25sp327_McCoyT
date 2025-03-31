CC=gcc
CFLAGS=-Wall -Wextra
LDFLAGS=-lpthread	# LDFLAGS is for the linker
ALL=primes1 primes2

all:${ALL}

primes1: primes1.c
	${CC} ${CFLAGS} primes1.c -o primes1 ${LDFLAGS}

primes2: primes2.c
	${CC} ${CFLAGS} primes2.c -o primes2 ${LDFLAGS}

clean:
	rm -vf *.o ${ALL}