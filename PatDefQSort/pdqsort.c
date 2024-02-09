#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "pdqsort.h"
#include "../InsSort/inssort.h"
#include "../HeapSort/heapsort.h"

#include <assert.h>

#ifndef NDEBUG
#include "../debug.c"
#endif

static size_t THRESHOLD_TO_INSSORT = 32;

static size_t get_depth(const size_t len) {
    return (size_t) 2 * log2(len);
}

static void swap(void *a, void *b, void * const keeper, const size_t size_elem) {
    memcpy(keeper, a, size_elem);
    memcpy(a,      b, size_elem);
    memcpy(b, keeper, size_elem);
}

static void* partition_right(void *begin, void *end, void *keeper, const size_t size_elem, int (*cmp)(const void *, const void *)) {
    void *pivot = begin;
    void *first = begin;
    void *last  = end  ;

    do { first += size_elem; } while (first < last && cmp(first, pivot) <= 0);

    if (first - size_elem == begin) {
        do { last -= size_elem; } while (first < last && cmp(pivot, last) < 0);

    } else {
        do { last -= size_elem; } while (                cmp(pivot, last) < 0);
    }

    while (first < last) {
        swap(first, last, keeper, size_elem);
        do { first += size_elem; } while (cmp(first, pivot) <= 0);
        do { last  -= size_elem; } while (cmp(pivot, last ) <  0);
    }

    first -= size_elem;
    swap(pivot, first, keeper, size_elem);

    return first;
}

static void *partition_left(void *begin, void *end, void *keeper, const size_t size_elem, int (*cmp)(const void *, const void *), int *already_partitioned) {
    //in partition_left pivot is the first one
    //before calling partition left we swap the first element with the
    //average value of the three of the first, median and last
    void *pivot = begin;
    void *first = begin;
    void *last  = end  ;

    do {last -= size_elem; } while (first < last && cmp(pivot, last) <= 0);

    if (last + size_elem == end) {
        do { first += size_elem; } while (first < last && cmp(first, pivot) < 0);

    } else {
        do { first += size_elem; } while (                cmp(first, pivot) < 0);
    }

    *already_partitioned = (first >= last);

    while (first < last) {
        swap(first, last, keeper, size_elem);
        do { last  -= size_elem; } while (cmp(pivot, last ) <= 0);
        do { first += size_elem; } while (cmp(first, pivot) <  0);
    }

    swap(begin, last, keeper, size_elem);

    return last;
}

static void pdqsort_imp(void *begin, void *end, const int depth, void *keeper, const size_t size_elem, int leftmost, int (*cmp)(const void *, const void *)) {
    while (begin < end) {
        size_t size = end - begin;

        if (size < THRESHOLD_TO_INSSORT * size_elem) {
            inssort_vt(begin, end, keeper, size_elem, cmp);
            return;
        }

        if (leftmost == 0) {
            void *pivot_pos = partition_right(begin, end, keeper, size_elem, cmp);
            begin = pivot_pos + size_elem;
            leftmost = 1;
            continue;
        }

        swap(begin, begin + size / 2 / size_elem * size_elem, keeper, size_elem);
        int already_partitioned;
        void *pivot_pos = partition_left(begin, end, keeper, size_elem, cmp, &already_partitioned);

        size_t l_size = pivot_pos - begin;
        size_t r_size = end - pivot_pos - size_elem;

        if (l_size < size / 8 || r_size < size / 8) {
            if (depth <= 0) {
                heapsort(begin, (end - begin) / size_elem, size_elem, cmp);
                return;
            }

        } 

        pdqsort_imp(begin, pivot_pos, depth - 1, keeper, size_elem, leftmost, cmp);
        leftmost = 0;
        begin = pivot_pos;
    }
}

void pdqsort(void *mem, const size_t len, const size_t size_elem, int (*cmp)(const void *, const void *)) {
    void *keeper = malloc(size_elem);
    int depth = get_depth(len);
    pdqsort_imp(mem, mem + len * size_elem, depth, keeper, size_elem, 1, cmp);
    free(keeper);
}

size_t set_threshold_to_inssort_pdqsort(const size_t len) {
    if (len == 0)
        return THRESHOLD_TO_INSSORT;

    size_t prev = THRESHOLD_TO_INSSORT;
    THRESHOLD_TO_INSSORT = len;
    return prev;
}
