#include "malloc.h"
#include <assert.h>
#include <stdio.h>

void use_space(int* ptr, size_t size)
{
	int i;
	for(i = 0; i < size; ++i){
		ptr[i] = 1;
	}
}

void test_malloc(int test_nbr, size_t size)
{
	printf("Test malloc %d with size %zd\n", test_nbr, size);
	void* a = malloc1(size);
	assert(a != NULL);
	use_space(a, size/sizeof(int));
	// free1(a);
	a = NULL;
	printf("\n");	
}

int main()
{
	size_t size;
	int i = 1;	

	printf("Tests NULL:\n");
	assert(malloc1(0) == NULL);
	printf("\n");

	test_malloc(i++, 70);

	test_malloc(i++, 35);

	test_malloc(i++, 80);

	// print_list();

	// test_malloc(i++, 1 * sizeof(int));
	// print_list();
	// test_malloc(i++, 2 * sizeof(int));
	// print_list();
	// test_malloc(i++, 2 * sizeof(int));
	// print_list();
	// test_malloc(i++, 3 * sizeof(int));
	// print_list();
	// test_malloc(i++, 4 * sizeof(int));
	// print_list();

	// test_malloc(i++, 1024*1024);
	// print_list();
	// test_malloc(i++, 1024*1024*8);
	// print_list();
	// test_malloc(i++, 1024*1024*8-16);
	// print_list();

	// void* a;

	// printf("Test calloc\n");
	// size = 1024;
	// a = calloc1(size, sizeof(int));
	// use_space(a, size);
	// assert(a != NULL);
	// printf("\n");

	// print_list();

	// printf("Test realloc\n");
	// size = 2000;
	// a = realloc1(a, size*sizeof(int));
	// assert(a != NULL);
	// use_space(a, size);
	// free1(a);
	// check_all_freed();
	// a = NULL;
	// printf("\n");
	
	// print_list();

	return 0;
}