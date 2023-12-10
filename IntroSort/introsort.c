#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "introsort.h"
#include "../InsSort/inssort.h"
#include "../HeapSort/heapsort.h"

static size_t THRESHOLD_TO_INSSORT = 32;

static size_t get_depth(const size_t len);
static void introsort_imp(void *mem, const size_t len, const size_t maxdepth, void * const keeper, const size_t size_elem, int (*cmp)(const void *, const void *));
static size_t partition(void *mem, const size_t len, void * const keeper, const size_t size_elem, int (*cmp)(const void *, const void *));
static void swap(void *a, void *b, void * const keeper, const size_t size_elem);

void introsort(void *mem, const size_t len, const size_t size_elem, int (*cmp)(const void *, const void *)) {
    size_t maxdepth = get_depth(len);
    void *keeper = malloc(size_elem);
    introsort_imp(mem, len, maxdepth, keeper, size_elem, cmp);
    free(keeper);
}

static void introsort_imp(void *mem, const size_t len, const size_t maxdepth, void * const keeper, const size_t size_elem, int (*cmp)(const void *, const void *)) {
    if (len < THRESHOLD_TO_INSSORT) {
        inssort_k(mem, len, keeper, size_elem, cmp);
    
    } else if (maxdepth == 0) {
        heapsort(mem, len, size_elem, cmp);

    } else {
        size_t p = partition(mem, len, keeper, size_elem, cmp);
        introsort_imp(mem, p + 1, maxdepth - 1, keeper, size_elem, cmp);
        introsort_imp(mem + p * size_elem, len - p, maxdepth - 1, keeper, size_elem, cmp);
    }
}

static size_t get_depth(const size_t len) {
    return (size_t) 2 * log2(len);
}

static size_t partition(void *mem, const size_t len, void * const keeper, const size_t size_elem, int (*cmp)(const void *, const void *)) {
    void *pivot = mem;
    size_t right = len - 1;
    size_t left  = 1;

    while (left < right) {
        if (cmp(mem + right * size_elem, pivot) >= 0) {
            right--;
            continue;
        }

        if (cmp(mem + left * size_elem, pivot) <= 0) {
            left++;
            continue;
        }

        swap(mem + left * size_elem, mem + right * size_elem, keeper, size_elem);

        left++;
        right--;
    }

    swap(pivot, mem + right * size_elem, keeper, size_elem);

    return right;
}

static void swap(void *a, void *b, void * const keeper, const size_t size_elem) {
    memcpy(keeper, a, size_elem);
    memcpy(a, b, size_elem);
    memcpy(b, keeper, size_elem);
}

size_t set_threshold_to_inssort_introsort(const size_t len) {
    if (len == 0)
        return THRESHOLD_TO_INSSORT;

    size_t prev = THRESHOLD_TO_INSSORT;
    THRESHOLD_TO_INSSORT = len;
    return prev;
}
