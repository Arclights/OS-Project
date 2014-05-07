#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define N (9)

typedef struct block_t block_t;

struct block_t {
	unsigned 	reserved:1; /* one if reserved. */
	char 		kval; 		/* current value of K. */
	block_t* 	succ; 		/* successor block in list. */
	block_t* 	pred; 		/* predecessor block in list. */
};

static char memory[1 << N] = { [1] = N };

static block_t* freelists[N + 1] = { [9] = (block_t*)&memory };


// For testing
/*******************************/
void print_blocks()
{	
	int i;
	printf("Blocks:\n");
	for(i = 1; i < N + 1; i++){
		printf("%d\t", i);
	}
	printf("\n");
	for(i = 1; i < N + 1; i++){
		if(freelists[i] == NULL){
			printf("0\t");
		}else{
			int j = 0;
			block_t* block = freelists[i];
			while(block != NULL){
				++j;
				block = block->succ;
			}
			printf("%d\t", j);
		}
	}
	printf("\n");
}
/*******************************/

size_t transform_to_closest_2_pow(size_t* size)
{
	size_t k = 1;
	while(1 << k < *size){
		++k;
	}
	*size = 1 << k;
	return k;
}

void remove_from_freelist(int freelist, block_t* block){
	printf("remove %zd\n", block);
	if(block->succ != NULL){
		printf("block->succ != NULL\n");
		block->succ->pred = block->pred;
	}
	if(block->pred != NULL){
		printf("block->pred != NULL\n");
		block->pred->succ = block->succ;
	}else{
		freelists[freelist] = block->succ;
	}
	block->pred = NULL;
	block->succ = NULL;
}

void add2freelist(int freelist, block_t* block){
	block->pred = NULL;
	block->succ = freelists[freelist];
	freelists[freelist] = block;

}

// Alignment
void* malloc(size_t size){
	printf("Malloc\n");

	printf("Size %zd\n", size);

	size_t k = transform_to_closest_2_pow(&size);
	printf("new size: %zd\n", size);
	printf("k: %zd\n", k);

	block_t* block;

	int j;
	for(j = k; j <= N; ++j){
		if(freelists[j]!=NULL){
			block = freelists[j];
			break;
		}
	}

	if(block == NULL){
		return NULL;
	}

	printf("J: %d\n", j);
	// print_blocks();

	int i;
	for(i = j; i > k; --i){
		size_t s = 1 << (i-1);
		printf("s: %zd\n", s);
		block = freelists[i];
		remove_from_freelist(i, block);

		block_t* new_block = ((char*)block)+s;		// Split in two
		block->kval = i-1;
		new_block->kval = i-1;
		add2freelist(i-1, new_block);
		add2freelist(i-1, block);
		print_blocks();
	}

	block_t* out = freelists[k];
	remove_from_freelist(k, out);
	printf("out: %zd\n", out);
	printf("%zd\n", out->kval);
	out->reserved = 1;
	return out;
}

void* calloc(size_t nitems, size_t size){
	printf("Calloc\n");
	void* ptr = malloc(nitems * size);
	if(ptr != NULL){
		return memset(ptr, 0, nitems);
	}
	return ptr;
}

void* realloc(void *ptr, size_t size){
	printf("Realloc\n");
	void* ptr_to = malloc(size);
	if(ptr != NULL){
		return memcpy(ptr_to, ptr, size);
	}
	return ptr;
}

void merge_blocks(block_t* block){
	size_t start = (size_t)&memory;
	size_t pointer = (size_t)block;
	unsigned K = block->kval;
	block_t* buddy = (block_t*)(start + ((pointer - start) ^ (1 << K)));
	printf("block: %zd\n", block);
	printf("buddy: %zd\n", buddy);
	printf("block->kval: %zd\n", block->kval);
	printf("buddy->kval: %zd\n", buddy->kval);
	printf("buddy->kval == block->kval: %zd\n", buddy->kval == block->kval);
	printf("!buddy->reserved: %zd\n", !buddy->reserved);

	if(buddy->kval == block->kval && !buddy->reserved){
		remove_from_freelist(K, block);
		remove_from_freelist(K ,buddy);
		printf("After remove:\n");
		print_blocks();
		if(buddy > block){
			block->kval = block->kval + 1;
			add2freelist(block->kval, block);
			print_blocks();
			merge_blocks(block);
		}else{
			buddy->kval = buddy->kval + 1;
			add2freelist(buddy->kval, buddy);
			print_blocks();
			merge_blocks(buddy);
		}
	}
}

void free(void* ptr){
	printf("Free\n");
	block_t* block = (block_t*) ptr;
	printf("Block size: %zd\n", block->kval);
	add2freelist(block->kval, block);
	block->reserved = 0;
	merge_blocks(block);
	printf("\n");
}