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
    void *beg;
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
static void timsort_merge(void *left, void *mid, void *right, const size_t size_elem, int (*cmp)(const void *, const void *));
static void timsort_blance(struct subarr_stack *stk, const size_t size_elem, int (*cmp)(const void *, const void *));
static void reverse(void *first, void *last, void *keeper, const size_t size_elem);

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

static void timsort_imp(void *mem, const size_t len, const size_t size_elem, int (*cmp)(const void *, const void *)) {
    size_t minrun = get_minrun(len);
    size_t minrun_bytes = minrun * size_elem;
    //size_t add_step = 0, cur_pos = 0;
    void *add_step; void *cur_pos = mem;
    void *right = mem + len * size_elem;
    int is_sorted = UNDEF_MONOTON;
    void *keeper = malloc(size_elem);

    struct subarr_stack stk;
    struct subarray subarr_x; 
    struct subarray subarr_y; 

    //we create a stack for the maximum possible number of items at runtime
    new_stack(&stk, len / minrun + 1);

    while (cur_pos < right) {
        add_step = cur_pos;
        is_sorted = UNDEF_MONOTON;

        //the last elements we add to the the last subarray
        //all arrays have len >= minrun
        //if array is sorted we continue until it is not
        while ((add_step < right) && ((add_step > right - minrun_bytes) || (add_step - cur_pos < minrun_bytes) || (is_sorted != NOT_SORTED))) {
            //if the array isn't sirted, we do not need other checks and conditions
            if ((is_sorted != NOT_SORTED) && (add_step + size_elem < right)) {
                int check_cmp = cmp(add_step, add_step + size_elem);

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
            
            add_step += size_elem;
        }

        if (is_sorted == IS_DECREASES)
            reverse(cur_pos, add_step - size_elem, keeper, size_elem);

        if (is_sorted == NOT_SORTED)
            inssort_vt(cur_pos, add_step, keeper, size_elem, cmp);

        subarr_x.beg = cur_pos;
        subarr_x.size = add_step - cur_pos;
        cur_pos = add_step; 

        push_stack(&stk, &subarr_x);

        timsort_blance(&stk, size_elem, cmp);
    }

    //emptying the stack
    while (stk.size > 1) { 
        pop_stack(&stk, &subarr_x);
        pop_stack(&stk, &subarr_y);

        timsort_merge(subarr_y.beg, subarr_x.beg, subarr_x.beg + subarr_x.size - size_elem, size_elem, cmp);

        subarr_y.size += subarr_x.size;

        push_stack(&stk, &subarr_y);
    } 

    free(keeper);
    free(stk.data);
}   

static void timsort_merge(void *left, void *mid, void *right, const size_t size_elem, int (*cmp)(const void *, const void *)) {
    size_t len_copy_byte = mid - left;
    void *mem_copy = malloc(len_copy_byte);
    //size_t pos_copy = 0, pos_right = mid;
    void *pos_right = mid;
    void *first_run_elem = NULL;
    int cmp_res = 0;

    //the calling function guarantees (right - mid) >= minrun > 0 and (mid - left) >= minrun > 0
    memcpy(mem_copy, left, len_copy_byte);
    void *pos_copy = mem_copy;
    void *right_copy_bdr = mem_copy + len_copy_byte;

    cmp_res = cmp(pos_copy, pos_right);

    for (void *i = left; i < right + size_elem; i += size_elem) {
        if ((pos_right > right) || ((pos_copy < right_copy_bdr) && (cmp_res <= 0))) {
            first_run_elem = pos_copy;
 
            if (pos_right > right)
                pos_copy = right_copy_bdr - size_elem;
            
            pos_copy += size_elem;
            while ((pos_copy < right_copy_bdr) && ((cmp_res = cmp(pos_copy, pos_right)) <= 0))
                pos_copy += size_elem;
        
            memcpy(i, first_run_elem, pos_copy - first_run_elem);
            i += pos_copy - first_run_elem - size_elem;

        } else {
            first_run_elem = pos_right;
 
            if (pos_copy >= right_copy_bdr)
                pos_right = right;

            pos_right += size_elem;
            while ((pos_right <= right) && ((cmp_res = cmp(pos_copy, pos_right)) > 0))
                pos_right += size_elem;
            
            memmove(i, first_run_elem, pos_right - first_run_elem);
            i += pos_right - first_run_elem - size_elem;
        }
    }

    free(mem_copy);
}

static void timsort_blance(struct subarr_stack *stk, const size_t size_elem, int (*cmp)(const void *, const void *)) {
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
                timsort_merge(subarr_y.beg, subarr_x.beg, subarr_x.beg + subarr_x.size - size_elem, size_elem, cmp);

                subarr_y.size += subarr_x.size;

                push_stack(stk, &subarr_z);
                push_stack(stk, &subarr_y);

            } else {
                timsort_merge(subarr_z.beg, subarr_y.beg, subarr_y.beg + subarr_y.size - size_elem, size_elem, cmp);
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

static void reverse(void *first, void *last, void *keeper, const size_t size_elem) {
    void *l = first; void *r = last;
    while (l < r) {
        memcpy(keeper, l, size_elem);
        memcpy(l, r, size_elem);
        memcpy(r, keeper, size_elem);
        l += size_elem; r -= size_elem;
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
