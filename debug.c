#ifndef DEBUG_C_INCLUDED
#define DEBUG_C_INCLUDED

#include <stdio.h>

//this file is for debugging only

void pint_data(void *arr, const size_t len) {
	int *arrint = (int *) arr;

	for (size_t i = 0; i < len; i++)
		//printf("%4d ", *(arrint + i));
		printf("|%lu, %d| ", i, *(arrint + i));

    /*for (size_t i = 0; i < len; i++)
        printf("%4lu ", i);*/
	
	printf("\n");
}

void pint_data2(void *arr, const size_t len) {
	int *arrint = (int *) arr;
    int num = 0;

	for (size_t i = 0; i < len; i++) {
		printf("%d ", *(arrint + i));

        if (*(arrint + i) == 0)
            num++;
    }
	// printf("\n0num = %d", num);
	printf("\n");
}

int check_data(void *mem, const size_t first, const size_t last, const size_t size_elem, int (*cmp)(const void *, const void *)) {
	for (size_t i = first; i < last; i++)
		if (cmp(mem + i * size_elem, mem + (i + 1) * size_elem) > 0) {
			printf("index of couple = %lu\n", i);
			return 0;
		}
	
	return 1;
}

//this function for pdqsort debugging
int check_part(const void *mem, const size_t len, const size_t left_bdr, const size_t right_bdr, const size_t size_elem, int (*cmp)(const void *, const void *)) {
    size_t incrt = 0;

    int *a = (int *) mem;

    assert(left_bdr < len);
    assert(right_bdr <= len);

    size_t mid = (left_bdr + right_bdr) / 2;

    if (left_bdr != 0) {
        for (size_t i = 0; i < left_bdr; i++) {
            if (cmp(a + i, a + mid) >= 0) {
                printf("case 1: %d >= %d in pos %lu\n", *(a + i), *(a + mid), i);
                incrt++;
                break;
            }
        }
    }

    if (cmp(a + left_bdr, a + mid) > 0) {
        printf("case 2: %d >= %d in pos %lu\n", *(a + left_bdr), *(a + mid), left_bdr);
        incrt++;
    }

    for (size_t i = left_bdr + 1; i < right_bdr; i++) {
        if (cmp(a + i, a + mid) != 0) {
            printf("case 3: %d != %d in pos %lu\n", *(a + i), *(a + mid), i);
            incrt++;
            break;
        }
    }

    for (size_t i = right_bdr; (i < right_bdr + 2) && (i < len); i++) {
        if (cmp(a + i, a + mid) < 0) {
            printf("case 4: %d < %d in pos %lu\n", *(a + i), *(a + mid), i);
            incrt++;
            break;
        }
    }

    for (size_t i = right_bdr + 2; i < len; i++) {
        if (cmp(a + i, a + mid) <= 0) {
            printf("case 5: %d <= %d in pos %lu\n", *(a + i), *(a + mid), i);
            incrt++;
            break;
        }
    }

    return !incrt;
}

int check_part_int_right(void *begin, void *end, void *pivot, const size_t size_elem, int (*cmp)(const void *, const void *)) {
    int incrt = 0;
    void *l = begin;
    void *r = end  ;

    printf("pvt = %d, pp = %ld\n", *((int *) pivot), (pivot - begin) / size_elem);

    while (l < pivot) {
        if (cmp(l, pivot) > 0) {
            printf("pvt = %d, pp = %ld\n", *((int *) pivot), (pivot - begin) / size_elem);
            printf("case 1: %d >p %d in pos %ld\n", *((int *) l), *((int *) pivot), (l - begin) / size_elem);
            incrt++;
            break;
        }

        l += size_elem;
    }

    while (r > pivot) {
        if (cmp(pivot, r) >= 0) {
            printf("pvt = %d, pp = %ld\n", *((int *) pivot), (pivot - begin) / size_elem);
            printf("case 2: %d p>= %d in pos %ld\n", *((int *) pivot), *((int *) r), (r - begin) / size_elem);
            incrt++;
            break;
        }

        r -= size_elem;
    }

    return !incrt;
}

int check_part_int_left(void *begin, void *end, void *pivot, const size_t size_elem, int (*cmp)(const void *, const void *)) {
    int incrt = 0;
    void *l = begin;
    void *r = end  ;

    printf("pvt = %d, pp = %ld\n", *((int *) pivot), (pivot - begin) / size_elem);

    while (l < pivot) {
        if (cmp(l, pivot) >= 0) {
            printf("pvt = %d, pp = %ld\n", *((int *) pivot), (pivot - begin) / size_elem);
            printf("case 1: %d <=p %d in pos %ld\n", *((int *) l), *((int *) pivot), (l - begin) / size_elem);
            incrt++;
            break;
        }

        l += size_elem;
    }

    while (r > pivot) {
        if (cmp(pivot, r) > 0) {
            printf("pvt = %d, pp = %ld\n", *((int *) pivot), (pivot - begin) / size_elem);
            printf("case 2: %d p< %d in pos %ld\n", *((int *) pivot), *((int *) r), (r - begin) / size_elem);
            incrt++;
            break;
        }

        r -= size_elem;
    }

    return !incrt;
}

#endif //DEBUG_C_INCLUDED