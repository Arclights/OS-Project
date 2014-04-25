#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <math.h>

#define N (9)

typedef struct block_t block_t;

struct block_t {
	unsigned 	reserved:1; /* one if reserved. */
	char 		kval; 		/* current value of K. */
	block_t* 	succ; 		/* successor block in list. */
	block_t* 	pred; 		/* predecessor block in list. */
};

static block_t freelists[N];


// For testing
/*******************************/
void print_blocks()
{

}
/*******************************/

size_t transform_to_closest_2_pow(size_t* size)
{
	size_t k = 1;
	while(pow(2.0, k) < *size){
		++k;
	}
	*size = (int)pow(2.0, k);
	return k;
}

// Alignment
void* malloc1(size_t size){

	if(size == 0){
		return NULL;
	}
	size_t k = transform_to_closest_2_pow(&size);
	printf("new size: %zd\n", size);
	printf("k: %zd\n", k);

	int j;
	for(j = k; j < N; ++j){
		if(&freelists[j] != NULL){
			break;
		}
	}

	if(j == N){
		return NULL;
	}

	block_t block = freelists[j];
	block_t* old_block_pred = block.pred;
	if(block.pred != NULL){
		block.pred->succ = block.succ;
	}
	if(block.succ != NULL){
		block.succ->pred = old_block_pred;
	}
	int i;
	for(i = k + 1; i < j; ++i){
		
	}

	return NULL;
}

void* calloc1(size_t nitems, size_t size){
	void* ptr = malloc1(nitems * size);
	if(ptr != NULL){
		return memset(ptr, 0, nitems);
	}
	return ptr;
}

void* realloc1(void *ptr, size_t size){
	void* ptr_to = malloc1(size);
	if(ptr != NULL){
		return memcpy(ptr_to, ptr, size);
	}
	return ptr;
}

void free1(void *ptr){

}