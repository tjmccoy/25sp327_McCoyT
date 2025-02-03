CC = gcc
CFLAGS = -Wall -Wextra
ALL = future_value

all: $(ALL)

###################################################################################################
# To get dependencies, you can run "gcc -MM future_value.c" which will display the dependencies.
# For example, it should output something like:
#    future_value.o: future_value.c future_value.h
###################################################################################################

future_value: future_value.o
	$(CC) $(CFLAGS) -o future_value future_value.o

future_value.o: future_value.c future_value.h
	$(CC) $(CFLAGS) -c future_value.c -o future_value.o

clean:	# removes object files & executable file
	rm -vf *.o $(ALL)
