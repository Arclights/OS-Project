#include <stddef.h>

void* malloc(size_t size);
void* calloc(size_t nitems, size_t size);
void* realloc(void *ptr, size_t size);
void free(void* ptr);

void print_blocks();