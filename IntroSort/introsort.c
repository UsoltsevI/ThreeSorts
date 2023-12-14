#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "introsort.h"
#include "../InsSort/inssort.h"
#include "../HeapSort/heapsort.h"

static size_t THRESHOLD_TO_INSSORT = 32;

static size_t get_depth(const size_t len);
static void introsort_imp(void *first, void *last, const size_t maxdepth, void * const keeper, const size_t size_elem, int (*cmp)(const void *, const void *));
static void *partition(void *first, void *last, void * const keeper, const size_t size_elem, int (*cmp)(const void *, const void *));
static void swap(void *a, void *b, void * const keeper, const size_t size_elem);

void introsort(void *mem, const size_t len, const size_t size_elem, int (*cmp)(const void *, const void *)) {
    size_t maxdepth = get_depth(len);
    void *keeper = malloc(size_elem);
    introsort_imp(mem, mem + len * size_elem, maxdepth, keeper, size_elem, cmp);
    free(keeper);
}

static void introsort_imp(void *first, void *last, const size_t maxdepth, void * const keeper, const size_t size_elem, int (*cmp)(const void *, const void *)) {
    if (last - first < THRESHOLD_TO_INSSORT * size_elem) {
        inssort_vt(first, last, keeper, size_elem, cmp);
    
    } else if (maxdepth == 0) {
        heapsort(first, (last - first) / size_elem, size_elem, cmp);

    } else {
        void *p = partition(first, last, keeper, size_elem, cmp);
        introsort_imp(first, p + size_elem, maxdepth - 1, keeper, size_elem, cmp);
        introsort_imp(p, last, maxdepth - 1, keeper, size_elem, cmp);
    }
}

static size_t get_depth(const size_t len) {
    return (size_t) 2 * log2(len);
}

static void *partition(void *first, void *last, void * const keeper, const size_t size_elem, int (*cmp)(const void *, const void *)) {
    void *pivot = first;
    void *right = last - size_elem;
    void *left  = first + size_elem;

    while (left < right) {
        if (cmp(right, pivot) >= 0) {
            right -= size_elem;
            continue;
        }

        if (cmp(left, pivot) <= 0) {
            left += size_elem;
            continue;
        }

        swap(left, right, keeper, size_elem);

        left += size_elem;
        right -= size_elem;
    }

    swap(pivot, right, keeper, size_elem);

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
