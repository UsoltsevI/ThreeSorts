#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "inssort.h"

static size_t moveright(void *arr, void *key, const size_t last, const size_t size_elem, int (*cmp)(const void *, const void *)) {
    size_t ise = 0;

    for (size_t i = 0; i < last; i++) {
        ise = i * size_elem;

        if (cmp(key, arr + ise) < 0) {     
            memmove(arr + ise + size_elem, arr + ise, size_elem * (last - i));

            return i;
        }
    }
    
    return last;
}

void inssort(void *arr, const size_t len, const size_t size_elem, int (*cmp)(const void *, const void *)) {
    size_t i = 0, pos = 0;
    void *key = malloc(size_elem);

    for (i = 0; i < len; i++) { 
        memcpy(key, arr + i * size_elem, size_elem);
        pos = moveright(arr, key, i, size_elem, cmp);
        memcpy(arr + pos * size_elem, key, size_elem);
    }

    free(key);
}

void inssort_k(void *arr, const size_t len, void *key_keeper, const size_t size_elem, int (*cmp)(const void *, const void *)) {
    size_t i = 0, pos = 0;

    for (i = 0; i < len; i++) { 
        memcpy(key_keeper, arr + i * size_elem, size_elem);
        pos = moveright(arr, key_keeper, i, size_elem, cmp);
        memcpy(arr + pos * size_elem, key_keeper, size_elem);
    }
}
