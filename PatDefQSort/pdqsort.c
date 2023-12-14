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
static size_t MIN_DEPTH_TO_HEAPSORT_COND = 8;

static size_t get_depth(const size_t len);
static void swap(void *a, void *b, void * const keeper, const size_t size_elem);
static void pdqsort_imp(void *mem, void *last, const size_t depth, void *keeper, const size_t size_elem, int (*cmp)(const void *, const void *));

static void *mid(void *first, void *last, const size_t size_elem, int (*cmp)(const void *, const void *));
static void *partition_right(void *first, void *last, void *keeper, const size_t size_elem, int (*cmp)(const void *, const void *));
static void *partition_left (void *first, void *last, void *keeper, const size_t size_elem, int (*cmp)(const void *, const void *));

void pdqsort(void *mem, const size_t len, const size_t size_elem, int (*cmp)(const void *, const void *)) {
    void *keeper = malloc(size_elem);
    size_t depth = get_depth(len);
    pdqsort_imp(mem, mem + len * size_elem, depth, keeper, size_elem, cmp);
    free(keeper);
}

static void pdqsort_imp(void *first, void *last, const size_t depth, void *keeper, const size_t size_elem, int (*cmp)(const void *, const void *)) {
    if (last - first < THRESHOLD_TO_INSSORT * size_elem) {
        inssort_vt(first, last, keeper, size_elem, cmp);

    } else if (depth == 0) {
        heapsort(first, (last - first) / size_elem, size_elem, cmp);

    } else {
        size_t threshold = (last - first) / 8;
        swap(first, mid(first, last - size_elem, size_elem, cmp), keeper, size_elem);
        void *q = partition_left (first, last, keeper, size_elem, cmp);

        if ((depth < MIN_DEPTH_TO_HEAPSORT_COND) && ((last - q < threshold) || (q - first < threshold))) {
            heapsort(first, (last - first) / size_elem, size_elem, cmp);
            return;
        }

        void *p = partition_right(first, q, keeper, size_elem, cmp);

        assert(p >= first);
        assert(q <= last);
        assert(p <= q);
        assert(cmp(p, q) <= 0);
        assert(check_part(first, (last - first) / size_elem, (p - first) / size_elem, (q - first) / size_elem, size_elem, cmp));

        pdqsort_imp(q,  last, depth - 1, keeper, size_elem, cmp);
        pdqsort_imp(first, p, depth - 1, keeper, size_elem, cmp);
    }
}

static void *mid(void *first, void *last, const size_t size_elem, int (*cmp)(const void *, const void *)) {
    if (last - size_elem <= first)
        return first;

    void *median = first + (last - first) / 2;

    assert(median < last  );
    assert(first  < median);

    if (cmp(first, last) < 0) {
        if (cmp(last, median) <= 0) {
            return last;

        } else {
            if (cmp(first, median) < 0) {
                return median;

            } else {
                return first;
            }
        }

    } else {
        if (cmp(median, first) < 0) {
            if (cmp(median, last) < 0) {
                return last;

            } else {
                return median;
            }

        } else {
            return first;
        }
    }

    return median;
}

static void* partition_right(void *first, void *last, void *keeper, const size_t size_elem, int (*cmp)(const void *, const void *)) {
    //in partition_right pivot is the last one
    //also, it is obviously pivot from previous partition left
    //which is in position q (last in this function)
    void *lp = first - size_elem; void *rp = last;

    if (lp + size_elem == rp)
        return rp;

    do { lp += size_elem; assert(lp <= last); } while (cmp(lp, last) < 0);
    
    if (lp == first) {
        do { rp -= size_elem; assert(rp >= first); } while ((lp < rp) && (cmp(rp, last) >= 0));

    } else {
        do { rp -= size_elem; assert(rp >= first); } while (cmp(rp, last) >= 0);
    }

    while (lp < rp) {
        swap(lp, rp, keeper, size_elem);
        do { lp += size_elem; assert(lp <  last ); } while (cmp(lp, last) <  0);
        do { rp -= size_elem; assert(rp >= first); } while (cmp(rp, last) >= 0);
    }

    swap(last, lp, keeper, size_elem);

    return lp;
}

static void *partition_left(void *first , void *last, void *keeper, const size_t size_elem, int (*cmp)(const void *, const void *)) {
    //in partition_left pivot is the first one
    //before calling partition left we swap the first element with the
    //average value of the three of the first, median and last
    void *lp = first; void *rp = last;

    do { rp -= size_elem; assert(rp >= first); } while (cmp(rp, first) > 0);

    if (rp + size_elem == last) {
        do { lp += size_elem; assert(lp <  last ); } while ((lp < rp) && (cmp(lp, first) <= 0));
    
    } else {
        do { lp += size_elem; assert(lp <  last ); } while (cmp(lp, first) <= 0);
    }

    while (lp < rp) {
        swap(lp, rp, keeper, size_elem);
        do { rp -= size_elem; assert(rp >= first); } while (cmp(rp, first) >  0);
        do { lp += size_elem; assert(lp <  last ); } while (cmp(lp, first) <= 0);
    }

    swap(first, rp, keeper, size_elem);

    return rp;
}

static void swap(void *a, void *b, void * const keeper, const size_t size_elem) {
    memcpy(keeper, a, size_elem);
    memcpy(a,      b, size_elem);
    memcpy(b, keeper, size_elem);
}

size_t set_threshold_to_inssort_pdqsort(const size_t len) {
    if (len == 0)
        return THRESHOLD_TO_INSSORT;

    size_t prev = THRESHOLD_TO_INSSORT;
    THRESHOLD_TO_INSSORT = len;
    return prev;
}

size_t set_min_depth_to_heapsort_cond_pdqsort(const size_t len) {
    size_t prev = MIN_DEPTH_TO_HEAPSORT_COND;
    MIN_DEPTH_TO_HEAPSORT_COND = len;
    return prev;
}

static size_t get_depth(const size_t len) {
    return (size_t) 2 * log2(len);
}
