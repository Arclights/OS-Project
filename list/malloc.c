#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define N (8 * 1024*1024 / sizeof(size_t))

typedef struct list_t list_t;

struct list_t {
	size_t size; /* size including list_t */
	list_t* next; /* next available block . */
	char data[]; /* C99 flexible array. */
};

static size_t pool[N] = { N * sizeof pool[0] };
static list_t avail = { .next = (list_t*)pool };

// For testing
/*******************************/
void check_all_freed()
{
	list_t* p = avail.next;

	while(p->next != NULL){
		if(p->next + p->next->size + sizeof(size_t) != p->next){
			printf("Not everything freed\n");
			break;
		}
	}
}

void print_list()
{
	list_t* p = &avail;
	printf("The list\n");
	int i = 0;
	while(p->next != NULL){
		printf("Node %d\tmem: %d\tsize: %d\n", i++, p->next, p->next->size);
		p = p->next;
	}
	printf("\n");
}
/*******************************/

int is_last(void* place){
	// return &avail + N > place;
	return place == NULL;
}

// Alignment
void* malloc1(size_t size){

	if(size == 0){
		return NULL;
	}

	size = size + sizeof(list_t);
	list_t* p = &avail;
	list_t* q = p->next;

	void* out;

	for(;;){
		if(q->size == size){
			printf("equal\n");
			// Take the entire chunk
			out = (char*)q + sizeof(list_t);
			p->next = q->next;
			break;
		}else if(q->size > size){
			printf("greater\t%zd > %zd\n",q->size, size);
			// Split the chunk
			out = (char*)q + sizeof(list_t);
			p->next = out + size; // Sets p->next to the memory chunk after the picked chunk
			p->next->size = q->size - size; // The size of the p->next chunk is the remaining size of when a chunk of size size is removed
			p->next->next = q->next;
			q->size = size; // The returned chunk has size size
			break;
		}

		p = q;
		q = p->next;

		if(is_last(q)){
			printf("last\n");
			list_t* pb = sbrk(size);
			if(errno == ENOMEM){
				printf("errno\n");
				return NULL;
			}
			pb->size = size;
			out = (char*)pb + sizeof(list_t);
			break;
		}

	}

	return out;
}

void* calloc1(size_t nitems, size_t size){
	void* ptr = malloc1(nitems * size);
	return memset(ptr, 0, nitems);
}

void* realloc1(void *ptr, size_t size){
	void* ptr_to = malloc1(size);
	if(ptr != NULL){
		return memcpy(ptr_to, ptr, size);
	}
	return NULL;
}

void free1(void *ptr){
	list_t* r = (char*)ptr - sizeof(list_t);
	printf("Freeing size: %zd\n", r->size);
	list_t* p = &avail;
	list_t* q = p->next;

	while(q != NULL){
		if(r < q){
			p->next = r;
			r->next = q;
			return;
		}

		p = q;
		q = q->next;
	}
	p->next = r;
	r->next = q;
	printf("Freeing failed\n");
}