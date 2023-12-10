#ifndef INSSORT_H_INCLUDED
#define INSSORT_H_INCLUDED

void inssort(void * arr, const size_t len, const size_t size_elem, int (*cmp)(const void *, const void *));

void inssort_k(void *arr, const size_t len, void *key_keeper, const size_t size_elem, int (*cmp)(const void *, const void *));

#endif //INSSORT_H_INCLUDED