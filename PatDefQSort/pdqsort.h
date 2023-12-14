#ifndef PATDEFQSORT_H_INCLUDED
#define PATDEFQSORT_H_INCLUDED

void pdqsort(void *mem, const size_t len, const size_t size_elem, int (*cmp)(const void *, const void *));

size_t set_threshold_to_inssort_pdqsort(const size_t len);
size_t set_min_depth_to_heapsort_cond_pdqsort(const size_t len);

#endif //PATDEFQSORT_H_INCLUDED