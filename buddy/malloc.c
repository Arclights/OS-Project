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
		freelists[i] = block->succ;
		if(freelists[i] != NULL){
			freelists[i]->pred = NULL;
		}

		block_t* new_block = ((char*)block)+s;		// Split in two
		block->succ = new_block;
		printf("block: %zd\n", block);
		printf("new_block: %zd\n", new_block);
		printf("block->kval: %zd\n", block->kval);
		printf("block->succ->succ: %zd\n", block->succ->succ);
		block->succ->succ = freelists[i-1];
		if(block->succ->succ != NULL){
			block->succ->succ->pred = block+s;
		}
		block->kval = i-1;
		new_block->kval = i-1;
		freelists[i-1] = block;
		print_blocks();
	}

	block_t* out = freelists[k];
	freelists[k] = out->succ;
	if(freelists[k] != NULL){
		freelists[k]->pred = NULL;
	}
	printf("out: %zd\n", out);
	printf("%zd\n", out->kval);
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

void add2freelist(int freelist, block_t* block){
	block_t* b = freelists[freelist];
	block_t* succ_block = b->succ;
	while(succ_block != NULL){
		b = succ_block;
		succ_block = b->succ;
	}
	b->succ = block;
	block->pred = b;
	block->succ = NULL;

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

	if(buddy->kval == block->kval && !buddy->reserved){
		if(buddy > block){
			block->kval = block->kval + 1;

			merge_blocks(block);
		}else{
			buddy->kval = buddy->kval + 1;
			merge_blocks(buddy);
		}
	}
}

void free(void* ptr){
	printf("Free\n");
	block_t* block = (block_t*) ptr;
	add2freelist(block->kval, block);
	block->reserved = 0;
	merge_blocks(block);
	printf("\n");
}