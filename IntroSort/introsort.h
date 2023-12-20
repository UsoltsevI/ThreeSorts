#ifndef INTROSORT_H_INCLUDED
#define INTROSORT_H_INCLUDED

void introsort(void *mem, const size_t len, const size_t size_elem, int (*cmp)(const void *, const void *));

size_t set_threshold_to_inssort_introsort(const size_t len);

#endif //INTROSORT_H_INCLUDED