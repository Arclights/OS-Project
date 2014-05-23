#include <stddef.h>

typedef struct list_t list_t;

struct list_t {
	size_t size; /* size including list_t */
	list_t* next; /* next available block . */
	size_t index;
	char data[]; /* C99 flexible array. */
};

void* malloc(size_t size);
void* calloc(size_t nitems, size_t size);
void* realloc(void *ptr, size_t size);
void free(void* ptr);

void check_all_freed();
void print_list();