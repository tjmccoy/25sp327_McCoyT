#include <stdio.h> 
#include <stdlib.h>

int main(int argc, char *argv[]) {
    if (argc != 1) {
        fprintf(stderr, "No arguments expected.\n");
        exit(EXIT_FAILURE);
    }
    
    char *ptr = NULL;   // initialize pointer
    int num_bytes = 1024;   // # of bytes to allocate

    ptr = malloc(num_bytes);

    if (!ptr) { // check to see if memory was correctly allocated
        perror("Failed to allocate bytes.");
        fprintf(stderr, "Failed to allocate bytes.\n");
        exit(EXIT_FAILURE);
    }

    printf("%s: Success!\n", argv[0]);
    free(ptr);  // free the memory 
    ptr = NULL; // set ptr to null

    return 0;
}