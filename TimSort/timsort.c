#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "timsort.h"
#include "../InsSort/inssort.h"

static const size_t MIN_LEN_TO_MERGESORT = 64;

#define IS_INCREASES 1
#define IS_DECREASES -1
#define NOT_SORTED   0
#define UNDEF_MONOTON 2

//#define DEBUGTS //it is not a comment)

struct subarray {
    size_t beg;
    size_t size;
};

//we don't need to kepp a capacity in the stack structure
//because we allocate memory for the stack once
//and this stack should work fast
//without unnecessary checks
struct subarr_stack {
    struct subarray* data;
    size_t size;
};

static void timsort_imp(void *mem, const size_t len, const size_t size_elem, int (*cmp)(const void *, const void *));
static void timsort_merge(void *mem, const size_t size_elem, const size_t left, const size_t mid, const size_t right, int (*cmp)(const void *, const void *));
static void timsort_blance(void *mem, const size_t size_elem, const size_t len, struct subarr_stack *stk, int (*cmp)(const void *, const void *));
static void reverse(void *arr, const size_t first, const size_t last, void *keeper, const size_t size_elem);

static size_t get_minrun(size_t n);

static void new_stack(struct subarr_stack *stk, const size_t beg_capacity);
static void push_stack(struct subarr_stack *stk, struct subarray *new_elem);
static void pop_stack(struct subarr_stack *stk, struct subarray *out);

void timsort(void *mem, const size_t len, const size_t size_elem, int (*cmp)(const void *, const void *)) {
    if (len <= MIN_LEN_TO_MERGESORT) {
        inssort(mem, len, size_elem, cmp);
        return;
    } 

    timsort_imp(mem, len, size_elem, cmp);
}

#ifdef DEBUGTS
#include <assert.h>

//this function exists to verify that the sorting is performed correctly at each step
int check_array(void *mem, const size_t size_elem, const size_t left, const size_t right, int (*cmp)(const void *, const void *)) {
    for (size_t i = left; i < right; i++) {
        if (cmp(mem + i * size_elem, mem + (i + 1) * size_elem) > 0)
            return NOT_SORTED;
    }

    return IS_INCREASES;
}
#endif

static void timsort_imp(void *mem, const size_t len, const size_t size_elem, int (*cmp)(const void *, const void *)) {
    size_t minrun = get_minrun(len);
    size_t add_step = 0, cur_pos = 0;
    size_t is_sorted = UNDEF_MONOTON;
    void *keeper = malloc(size_elem);

    struct subarr_stack stk;
    struct subarray subarr_x; 
    struct subarray subarr_y; 

    //we create a stack for the maximum possible number of items at runtime
    new_stack(&stk, len / minrun + 1);

    while (cur_pos < len) {
        add_step = cur_pos;
        is_sorted = UNDEF_MONOTON;

        //the last elements we add to the the last subarray
        //all arrays have len >= minrun
        //if array is sorted we continue until it is not
        while ((add_step < len) && ((add_step > len - minrun) || (add_step - cur_pos < minrun) || (is_sorted != NOT_SORTED))) {
            //if the array isn't sirted, we do not need other checks and conditions
            if ((is_sorted != NOT_SORTED) && (add_step + 1 < len)) {
                int check_cmp = cmp(mem + add_step * size_elem, mem + (add_step + 1) * size_elem);

                if (is_sorted == UNDEF_MONOTON) {
                    if (check_cmp < 0) {
                        is_sorted = IS_INCREASES;

                    } else if (check_cmp > 0) {
                        is_sorted = IS_DECREASES;
                    } 
                    //in the last case we leave UNDEF_MONOTON

                } else if ((is_sorted == IS_INCREASES) && (check_cmp > 0)) {
                    is_sorted = NOT_SORTED;
                    
                } else if ((is_sorted == IS_DECREASES) && (check_cmp < 0)) {
                    is_sorted = NOT_SORTED;
                }
            }
            
            add_step++;
        }

        if (is_sorted == IS_DECREASES)
            reverse(mem, cur_pos, add_step - 1, keeper, size_elem);

        if (is_sorted == NOT_SORTED)
            inssort_k(mem + cur_pos * size_elem, add_step - cur_pos, keeper, size_elem, cmp);

        subarr_x.beg = cur_pos;
        subarr_x.size = add_step - cur_pos;
        cur_pos = add_step; 

        push_stack(&stk, &subarr_x);

        timsort_blance(mem, size_elem, len, &stk, cmp);
    }

    //emptying the stack
    while (stk.size > 1) { 
        pop_stack(&stk, &subarr_x);
        pop_stack(&stk, &subarr_y);

        timsort_merge(mem, size_elem, subarr_y.beg, subarr_x.beg, subarr_x.beg + subarr_x.size - 1, cmp);

        subarr_y.size += subarr_x.size;

        push_stack(&stk, &subarr_y);
    } 

#ifdef DEBUGTS
    assert(check_array(mem, size_elem, 0, len - 1, cmp));
#endif

    free(keeper);
    free(stk.data);
}   

static void timsort_merge(void *mem, const size_t size_elem, const size_t left, const size_t mid, const size_t right, int (*cmp)(const void *, const void *)) {
    size_t len_copy = mid - left;
    void *mem_copy = malloc(len_copy * size_elem);
    size_t pos_copy = 0, pos_right = mid;
    size_t first_run_elem = 0;
    int cmp_res = 0;

    //the calling function guarantees (right - mid) >= minrun > 0 and (mid - left) >= minrun > 0
    memcpy(mem_copy, mem + left * size_elem, len_copy * size_elem);

    cmp_res = cmp(mem_copy + pos_copy * size_elem, mem + pos_right * size_elem);

    for (size_t i = left; i < right + 1; i++) {
        if ((pos_right > right) || ((pos_copy < len_copy) && (cmp_res <= 0))) {
            first_run_elem = pos_copy;
 
            if (pos_right > right)
                pos_copy = len_copy - 1;
            
            pos_copy++;
            while ((pos_copy < len_copy) && ((cmp_res = cmp(mem_copy + pos_copy * size_elem, mem + pos_right * size_elem)) <= 0))
                pos_copy++;
        
            memcpy(mem + i * size_elem, mem_copy + first_run_elem * size_elem, (pos_copy - first_run_elem) * size_elem);
            i += pos_copy - first_run_elem - 1;

        } else {
            first_run_elem = pos_right;
 
            if (pos_copy >= len_copy)
                pos_right = right;

            pos_right++;
            while ((pos_right <= right) && ((cmp_res = cmp(mem_copy + pos_copy * size_elem, mem + pos_right * size_elem)) > 0))
                pos_right++;
            
            memmove(mem + i * size_elem, mem + first_run_elem * size_elem, (pos_right - first_run_elem) * size_elem);
            i += pos_right - first_run_elem - 1;
        }
    }

#ifdef DEBUGTS
    assert(check_array(mem, size_elem, left, right, cmp));
#endif

    free(mem_copy);
}

static void timsort_blance(void *mem, const size_t size_elem, const size_t len, struct subarr_stack *stk, int (*cmp)(const void *, const void *)) {
    int loop_on = 1;

    while ((stk->size > 2) && (loop_on)) {
        struct subarray subarr_x;
        struct subarray subarr_y;
        struct subarray subarr_z;

        pop_stack(stk, &subarr_x);
        pop_stack(stk, &subarr_y);
        pop_stack(stk, &subarr_z);

        if ((subarr_z.size < subarr_y.size + subarr_x.size) || (subarr_y.size < subarr_x.size)) {
            if ((subarr_x.size < subarr_z.size) && (subarr_y.size < subarr_z.size) && (subarr_z.size > subarr_y.size + subarr_x.size)) {
                timsort_merge(mem, size_elem, subarr_y.beg, subarr_x.beg, subarr_x.beg + subarr_x.size - 1, cmp);

                subarr_y.size += subarr_x.size;

                push_stack(stk, &subarr_z);
                push_stack(stk, &subarr_y);

            } else {
                timsort_merge(mem, size_elem, subarr_z.beg, subarr_y.beg, subarr_y.beg + subarr_y.size - 1, cmp);
                subarr_z.size += subarr_y.size; 

                push_stack(stk, &subarr_z);
                push_stack(stk, &subarr_x);
            }

        } else {
            push_stack(stk, &subarr_z);
            push_stack(stk, &subarr_y);
            push_stack(stk, &subarr_x);
            loop_on = 0;
        }
    } 
}

static void reverse(void *arr, const size_t first, const size_t last, void *keeper, const size_t size_elem) {
    for (size_t i = 0; i <= (last - first) / 2; i++) {
        memcpy(keeper, arr + (i + first) * size_elem, size_elem);
        memcpy(arr + (i + first) * size_elem, arr + (last - i) * size_elem, size_elem);
        memcpy(arr + (last - i) * size_elem, keeper, size_elem);
    }
}

//copied from vikipedia
static size_t get_minrun(size_t n) {
    size_t r = 0;

    while (n >= MIN_LEN_TO_MERGESORT) {
        r |= (n & 1u);
        n >>= 1u;
    }

    return r + n;
}

//simple and fast stack functions
//without unnecessary checks
static void new_stack(struct subarr_stack *stk, const size_t beg_capacity) {
    stk->size = 0;
    stk->data = (struct subarray *) malloc(beg_capacity * sizeof(struct subarray));
}

static void push_stack(struct subarr_stack *stk, struct subarray *new_elem) {
    stk->data[stk->size].beg  = new_elem->beg;
    stk->data[stk->size].size = new_elem->size;
    stk->size++;
}

static void pop_stack(struct subarr_stack *stk, struct subarray *out) {
    stk->size--;
    out->beg  = stk->data[stk->size].beg;
    out->size = stk->data[stk->size].size;
}

#undef IS_INCREASES
#undef IS_DECREASES
#undef NOT_SORTED
#undef UNDEF_MONOTON
