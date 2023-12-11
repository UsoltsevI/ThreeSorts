#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pdqsort.h"
#include "../InsSort/inssort.h"

#include <assert.h>

//#define PRT
#ifndef NDEBUG
#include "debug.c"
#endif

#ifdef PRT
#include "debug.c"
#endif 

static size_t THRESHOLD_TO_INSSORT = 32;

static void swap(void *a, void *b, void * const keeper, const size_t size_elem);
static void pdqsort_imp(void *mem, const size_t len, void *keeper, const size_t size_elem, int (*cmp)(const void *, const void *));
static void partition(void *mem, const size_t len, size_t *left_eq, size_t *right_eq, void *keeper, const size_t size_elem, int (*cmp)(const void *, const void *));

void pdqsort(void *mem, const size_t len, const size_t size_elem, int (*cmp)(const void *, const void *)) {
    void *keeper = malloc(size_elem);
    pdqsort_imp(mem, len, keeper, size_elem, cmp);
    free(keeper);
}

static void pdqsort_imp(void *mem, const size_t len, void *keeper, const size_t size_elem, int (*cmp)(const void *, const void *)) {
    if (len < THRESHOLD_TO_INSSORT) {
        inssort_k(mem, len, keeper, size_elem, cmp);

    } else {
        size_t left_eq  = 0;
        size_t right_eq = 0;  

#ifdef PRT
        printf("before partition:\n");
        pint_data(mem, len);
#endif 
        
        size_t r_pos = random() % len;
        swap(mem + (len - 1) * size_elem, mem + r_pos * size_elem, keeper, size_elem);

        partition(mem, len, &left_eq, &right_eq, keeper, size_elem, cmp);

#ifdef PRT
        printf("after partition:\n");
        printf("len = %lu, left_eq = %lu, right_eq = %lu\n", len, left_eq, right_eq);
        pint_data(mem, len);
        printf("\n");
#endif 
        assert(left_eq != -1);
        assert(right_eq != -1);
        assert(right_eq <= len);
        assert(left_eq < len);

        assert(check_part(mem, len, left_eq, right_eq, size_elem, cmp));

        pdqsort_imp(mem, left_eq, keeper, size_elem, cmp);
        pdqsort_imp(mem + right_eq * size_elem, len - right_eq, keeper, size_elem, cmp);
    }
}

static void partition(void *mem, const size_t len, size_t *left_eq, size_t *right_eq, void *keeper, const size_t size_elem, int (*cmp)(const void *, const void *)) {
    void *pivot = mem + (len - 1) * size_elem;
    size_t left = 0;
    size_t right = len - 2;
    size_t l_eq = 0;
    size_t r_eq = len - 1;

    //printf("piot = %d, %lu\n", *((int *) pivot), len - 1); 

    while (left <= right) {
        while ((cmp(mem + left * size_elem, pivot) < 0) && (left < len - 1))
            left++; 
        
        while ((cmp(mem + right * size_elem, pivot) > 0) && (right > 0))
            right--;
        
        if (left >= right)
            break;

        assert(left < len - 1);
        assert(right < len - 1);
        
        swap(mem + left * size_elem, mem + right * size_elem, keeper, size_elem);

        if (cmp(mem + left * size_elem, pivot) == 0) {
            swap(mem + l_eq * size_elem, mem + left * size_elem, keeper, size_elem);
            l_eq++;
        }

        left++;

        if (cmp(mem + right * size_elem, pivot) == 0) {
            r_eq--;
            swap(mem + r_eq * size_elem, mem + right * size_elem, keeper, size_elem);
        }

        right--;
    }

#ifdef PRT 
    printf("in partition 1: left = %lu, right = %lu, l_eq = %lu, r_eq = %lu\n", left, right, l_eq, r_eq);
    pint_data(mem, len);
#endif

    if ((left != r_eq) && (cmp(mem + left * size_elem, pivot) == 0)) {
        r_eq--;
        swap(mem + r_eq * size_elem, mem + left * size_elem, keeper, size_elem);
    }

    assert(left < len);

    swap(mem + left * size_elem, pivot, keeper, size_elem);

    right = left - 1;
    left++; 

#ifdef PRT 
    printf("in partition 2: left = %lu, right = %lu, l_eq = %lu, r_eq = %lu\n", left, right, l_eq, r_eq);
    pint_data(mem, len);
#endif

    for (size_t k = 0; k < l_eq; k++, right--) {
        if ((k >= left) || (right == -1)) {
            right = -1;
            break;
        }

        assert(k < len);
        assert(right != -1);

        swap(mem + k * size_elem, mem + right * size_elem, keeper, size_elem);
    }

    *left_eq = right + 1;

#ifdef PRT 
    printf("in partition 3:\n");
    pint_data(mem, len);
#endif 

    for (size_t k = len - 2; k >= r_eq; k--, left++) {
        if (k < *left_eq) {
            left = len;
            break;
        }

        swap(mem + k * size_elem, mem + left * size_elem, keeper, size_elem);
    }

    *right_eq = left - 1;
}

static void swap(void *a, void *b, void * const keeper, const size_t size_elem) {
    memcpy(keeper, a, size_elem);
    memcpy(a, b, size_elem);
    memcpy(b, keeper, size_elem);
}

size_t set_threshold_to_inssort_pdqsort(const size_t len) {
    if (len == 0)
        return THRESHOLD_TO_INSSORT;

    size_t prev = THRESHOLD_TO_INSSORT;
    THRESHOLD_TO_INSSORT = len;
    return prev;
}
