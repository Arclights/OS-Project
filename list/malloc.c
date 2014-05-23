#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define N (8 * 1024*1024 / sizeof(size_t))

typedef struct list_t list_t;

static size_t malloc_nbr = 0;

struct list_t {
	size_t size; /* size including list_t */
	list_t* next; /* next available block . */
	size_t index;
	char data[]; /* C99 flexible array. */
};

static size_t pool[N] = { N * sizeof pool[0] };
static list_t avail = { .next = (list_t*)pool };

// For testing
/*******************************/
void check_all_freed()
{
	list_t* p = avail.next;

	size_t tot_size = 0;

	while(p != NULL){
		if(p->next!=NULL && (char*)p + p->size != p->next){
			printf("Not everything freed\n");
			printf("p: %zu\tp->size: %zu\tp->next: %zu\n", p, p->size, p->next);
			return;
		}
		tot_size += p->size;
		p = p->next;
	}
	if(tot_size != N * sizeof pool[0]){
		printf("Not everything freed\n");
		printf("tot_size: %zu\tN: %zu\n", tot_size, N * sizeof pool[0]);
		return;
	}
	printf("Everything freed\n");
}

void print_list()
{
	list_t* p = (&avail)->next;
	printf("The list\n");
	int i = 0;
	while(p != NULL){
		printf("Node %d\tmem: %d\tsize: %d\n", i++, p, p->size);
		p = p->next;
	}
	printf("\n");
}
/*******************************/

// Alignment
void* malloc(size_t size){
	printf("malloc\n");
	print_list();

	// if(size == 0){
	// 	return NULL;
	// }

	size = size + sizeof(list_t);
	list_t* p = &avail;
	list_t* q = p->next;

	void* out;

	for(;;){
		if(q->size == size){
			printf("\tequal\n");
			// Take the entire chunk
			p->next = q->next;
			break;
		}else if(q->size - sizeof(list_t) > size){
			// Split the chunk
			printf("\tgreater\t%zd > %zd\n",q->size, size);
			p->next = (char*)q + size; // Sets p->next to the memory chunk after the picked chunk
			p->next->size = q->size - size; // The size of the p->next chunk is the remaining size of when a chunk of size size is removed
			p->next->next = q->next;
			 // The returned chunk has size size
			break;
		}

		p = q;
		q = p->next;

		if(q == NULL){
			printf("\tlast\n");
			q = (list_t*)sbrk(size);
			if(errno == ENOMEM){
				fprintf(stderr, "\t\terrno\n");
				abort();
			}
			break;
		}

	}
	q->index = malloc_nbr;
	q->size = size;
	q->next = NULL;
	out = (char*)q + sizeof(list_t);
	printf("Malloc nbr: %zu\n", malloc_nbr);
	malloc_nbr++;
	print_list();
	return out;
}

void* calloc(size_t nitems, size_t size){
	printf("calloc\n");
	void* ptr = malloc(nitems * size);
	if(ptr != NULL){
		memset(ptr, 0, nitems);
	}
	return ptr;
}

void free(void* ptr){
	printf("free\n");

	if(ptr == NULL){
		return;
	}
	print_list();
	list_t* r = (char*)ptr - sizeof(list_t);
	printf("Freeing nbr: %zu\n", r->index);
	printf("Freeing size: %zd\n", r->size);
	list_t* p = &avail;
	list_t* q = p->next;

	while(q != NULL){
		if(r < q){
			p->next = r;
			r->next = q;
			print_list();
			return;
		}

		p = q;
		q = q->next;
	}
	p->next = r;
	if(r->next==NULL){
		printf("r->next==NULL\n");
	}
	r->next = q;
	print_list();
	// printf("Freeing failed\n");
}

void* realloc(void *ptr, size_t size){
	printf("realloc\n");
	void* out = malloc(size);
	if(ptr != NULL){
		out = memcpy(out, ptr, size);
		free(ptr);
	}
	return out;
}