#include <stdio.h>  // angled brackets are for libraries in the GCC library
#include "mymalloc.h" // quotes are for libraries on the local drive

void *alloc_bytes(size_t num_bytes) {
    char *ptr = NULL;
    ptr = malloc(num_bytes);

    if (!ptr) {
        perror("alloc_bytes");
        fprintf(stderr, "alloc_bytes(%ld) failed\n", num_bytes);
        exit(EXIT_FAILURE);
    }
    memset(ptr, 'a', num_bytes);
    ptr[num_bytes-1] =  '\0';
    
    return ptr;
}

void *realloc_bytes(void* ptr, size_t num_bytes) {
    /*
    ptr = realloc(ptr, num_bytes);

    if (!ptr) {
        perror("realloc_bytes");
        fprintf(stderr, "realloc_bytes(%ld) failed\n", num_bytes);
        exit(EXIT_FAILURE);
    }
    return;
    */ 
   return NULL;
}