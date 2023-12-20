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

static void *moveright_vt(void *first, void *key, void *last, const size_t size_elem, int (*cmp)(const void *, const void *)) {
    for (void *i = first; i < last; i += size_elem) {
        if (cmp(key, i) < 0) {     
            memmove(i + size_elem, i, last - i);

            return i;
        }
    }
    
    return last;
}

void inssort_vt(void *first, void *last, void *key_keeper, const size_t size_elem, int (*cmp)(const void *, const void *)) {
    void *pos;
    for (void *i = first; i < last; i += size_elem) { 
        memcpy(key_keeper, i, size_elem);
        pos = moveright_vt(first, key_keeper, i, size_elem, cmp);
        memcpy(pos, key_keeper, size_elem);
    }
}
