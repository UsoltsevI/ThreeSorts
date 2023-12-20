#ifndef TIMSORT_H_INCLUDED
#define TIMSORT_H_INCLIDED

void timsort(void * const mem, const size_t len, const size_t size_elem, int (*cmp)(const void *, const void *));

#endif //TIMSORT_H_INCLUDED