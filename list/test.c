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
	// check_all_freed();
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
	assert(malloc(0) != NULL);
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

	printf("Testing multiple mallocs\n");
	a = malloc(10);
	assert(a != NULL);
	use_space(a, 10/sizeof(int));
	print_list();
	void* b = malloc(20);
	assert(b != NULL);
	use_space(b, 20/sizeof(int));
	print_list();
	void* c = malloc(30);
	assert(c != NULL);
	use_space(c, 30/sizeof(int));
	print_list();
	void* d = malloc(40);
	assert(d != NULL);
	use_space(d, 40/sizeof(int));
	print_list();
	void* e = malloc(50);
	assert(e != NULL);
	use_space(e, 50/sizeof(int));
	print_list();
	void* f = malloc(60);
	assert(f != NULL);
	use_space(f, 60/sizeof(int));
	print_list();
	void* g = malloc(70);
	assert(g != NULL);
	use_space(g, 70/sizeof(int));
	print_list();
	free(a);
	free(b);
	free(c);
	free(d);
	free(e);
	free(f);
	free(g);
	printf("\n");

	printf("Test calloc\n");
	size = 1024;
	a = calloc(size, sizeof(int));
	use_space(a, size);
	assert(a != NULL);
	printf("\n");

	print_list();

	printf("Test realloc1\n");
	size = 2000;
	a = realloc(a, size*sizeof(int));
	assert(a != NULL);
	use_space(a, size);
	printf("\n");

	print_list();

	printf("Test realloc2\n");
	size = 1000;
	a = realloc(a, size*sizeof(int));
	assert(a != NULL);
	use_space(a, size);
	free(a);
	check_all_freed();
	a = NULL;
	printf("\n");

	print_list();

	printf("Test realloc3\n");
	a = realloc(NULL, 0);
	assert(a != NULL);
	printf("\n");
	
	print_list();

	printf("Freeing an allready freed pointer\n");
	a = malloc(10);
	free(a);
	print_list();
	free(a);
	print_list();

	// printf("Test really large amount of mallocs\n");
	// int mallocs = 1000;
	// void* ptrs[mallocs];
	// for(i = 0; i < mallocs; ++i){
	// 	ptrs[i] = malloc(100000);
	// }
	// ptrs[mallocs/2] = realloc(ptrs[mallocs/2], 500);
	// for(i = 0; i < mallocs; ++i){
	// 	free(ptrs[i]);
	// }
	// printf("\n");
	
	print_list();	

	printf("sizeof(size_t): %zd\n", sizeof(size_t));
	printf("sizeof(char): %zd\n", sizeof(char));

	return 0;
}