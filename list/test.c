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
	void* a = malloc(size);
	assert(a != NULL);
	use_space(a, size/sizeof(int));
	free(a);
	check_all_freed();
	a = NULL;
	printf("\n");	
}

int main()
{
	size_t size;
	int i = 1;

	printf("Testing printf 1\n");

	printf("Testing %s\n", "printf 2");	

	printf("Test printf passed\n");

	check_all_freed();

	printf("Tests NULL:\n");
	assert(malloc(0) == NULL);
	printf("\n");

	print_list();

	test_malloc(i++, 1 * sizeof(int));
	print_list();
	test_malloc(i++, 2 * sizeof(int));
	print_list();
	test_malloc(i++, 2 * sizeof(int));
	print_list();
	test_malloc(i++, 3 * sizeof(int));
	print_list();
	test_malloc(i++, 4 * sizeof(int));
	print_list();

	test_malloc(i++, 1024*1024);
	print_list();
	test_malloc(i++, 1024*1024*8);
	print_list();
	test_malloc(i++, 1024*1024*8-16);
	print_list();

	void* a;

	printf("Test calloc\n");
	size = 1024;
	a = calloc(size, sizeof(int));
	use_space(a, size);
	assert(a != NULL);
	printf("\n");

	print_list();

	printf("Test realloc\n");
	size = 2000;
	a = realloc(a, size*sizeof(int));
	assert(a != NULL);
	use_space(a, size);
	free(a);
	check_all_freed();
	a = NULL;
	printf("\n");
	
	print_list();

	return 0;
}