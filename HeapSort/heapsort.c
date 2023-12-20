#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "heapsort.h"

#ifndef NDEBUG
#include "../debug.c"
#endif

static void swap(void *a, void *b, void* keeper, const size_t size_elem);
static void sift_down(void *mem, const size_t cur_elem, const size_t first, void *keeper, const size_t size_elem, int (*cmp)(const void *, const void *));

void heapsort(void *mem, const size_t len, const size_t size_elem, int (*cmp)(const void *, const void *)) {
	void *keeper = malloc(size_elem);

	for (size_t i = len - 1; (i >= 0) && (i != -1); i--)
		sift_down(mem, i, len - 1, keeper, size_elem, cmp);

	for (size_t first_sorted = len - 1; first_sorted > 0; first_sorted--) {
		swap(mem, mem + first_sorted * size_elem, keeper, size_elem);
		sift_down(mem, 0, first_sorted - 1, keeper, size_elem, cmp);

		assert(check_data(mem, first_sorted, len - 1, size_elem, cmp));
	}

	free(keeper);

	assert(check_data(mem, 0, len - 1, size_elem, cmp));
}

static void swap(void *a, void *b, void *keeper, const size_t size_elem) {
	memcpy(keeper, a, size_elem);
	memcpy(a, b, size_elem);
	memcpy(b, keeper, size_elem);
}

static void sift_down(void *mem, const size_t cur_elem, const size_t first, void *keeper, const size_t size_elem, int (*cmp)(const void *, const void *)) {
	if (cur_elem * 2 >= first)
		return;

	//the following variables are used in order to reduce the number of multiplications
	size_t next_elem = 2 * cur_elem + 1;
	size_t next_elem_add = next_elem * size_elem; 
	size_t cur_add = cur_elem * size_elem;

	if ((next_elem + 1 <= first) && (cmp(mem + next_elem_add + size_elem, mem + next_elem_add) > 0)) {
		next_elem     += 1;
		next_elem_add += size_elem;
	}

	if (cmp(mem + next_elem_add, mem + cur_add) > 0) {
		swap(mem + next_elem_add, mem + cur_add, keeper, size_elem);
		sift_down(mem, next_elem, first, keeper, size_elem, cmp);
	}
}