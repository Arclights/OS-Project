#include <stddef.h>

void* malloc1(size_t size);
void* calloc1(size_t nitems, size_t size);
void* realloc1(void *ptr, size_t size);
void free1(void* ptr);

void print_blocks();