#include <stddef.h>

typedef struct block_t block_t;

struct block_t {
	unsigned 	reserved:1; /* one if reserved. */
	char 		kval; 		/* current value of K. */
	block_t* 	succ; 		/* successor block in list. */
	block_t* 	pred; 		/* predecessor block in list. */
	size_t		index;
};

void* malloc(size_t size);
void* calloc(size_t nitems, size_t size);
void* realloc(void *ptr, size_t size);
void free(void* ptr);

void print_blocks();