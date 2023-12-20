#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <assert.h>
#include "HeapSort/heapsort.h"
#include "InsSort/inssort.h"
#include "IntroSort/introsort.h"
#include "PatDefQSort/pdqsort.h"
#include "TimSort/timsort.h"

int cmp_int(const void *a, const void *b) {
    int *ai = (int *) a;
    int *bi = (int *) b;
    return (*ai) - (*bi);
}

static double diff(struct timespec start, struct timespec end);

const int MICROSEC_AS_NSEC = 1000;
const int SEC_AS_NSEC = 1000000000;

#define SEC_AS_MICROSEC (SEC_AS_NSEC / MICROSEC_AS_NSEC)

static double diff(struct timespec start, struct timespec end) {
    struct timespec temp;

    if (end.tv_nsec - start.tv_nsec < 0) {
        temp.tv_sec = end.tv_sec - start.tv_sec - 1;
        temp.tv_nsec = SEC_AS_NSEC + end.tv_nsec - start.tv_nsec;

    } else {
        temp.tv_sec = end.tv_sec - start.tv_sec;
        temp.tv_nsec = end.tv_nsec - start.tv_nsec;
    }

    double msec = temp.tv_sec * SEC_AS_MICROSEC + temp.tv_nsec / MICROSEC_AS_NSEC;
    
    return msec / SEC_AS_MICROSEC;
}

void test_sort(int *data, const size_t len, void (*sort)(void *, const size_t, const size_t, int (*cmp)(const void *, const void *)), const int out, const char *file_ans_name);
int *read_data_int(FILE *file, size_t *len);
void test_sort_all(int *data, const size_t len, const char *file_ans);
void check_ans(const char *file_ans_name, int *data, const size_t len);

int main(int argc, char * *argv) {
    if ((argc < 2) || (4 < argc)) {
        printf("Invalid enter\n");
        printf("See make instructions\n");
        return -1;
    }

    int out = 0;
    size_t len = 0;
    int *data = read_data_int(stdin, &len);

    if (!strcmp(argv[1], "all")) {
        if (argc == 2) {
            test_sort_all(data, len, NULL);
            free(data);
            return 0;

        } else if (argc == 3) {
            test_sort_all(data, len, argv[2]);
            free(data);
            return 0;
        } else {
            printf("Invalid enter\n");
            printf("See make instructions\n");
            free(data);
            return -1;
        }
    }

    char *file_ans_name = NULL;

    if (argc > 2) {
        if  (!strcmp(argv[2], "out")) {
            out = 1;
        } else {
            file_ans_name = argv[2];
        }
    } 

    if (argc == 4)
        file_ans_name = argv[3];

    if (!strcmp(argv[1], "heapsort")) {
        test_sort(data, len, &heapsort, out, file_ans_name);

    } else if (!strcmp(argv[1], "inssort")) {
        test_sort(data, len, &inssort, out, file_ans_name);

    } else if (!strcmp(argv[1], "introsort")) {
        test_sort(data, len, &introsort, out, file_ans_name);

    } else if (!strcmp(argv[1], "qsort")) {
        test_sort(data, len, &qsort, out, file_ans_name);

    } else if (!strcmp(argv[1], "pdqsort")) {
        test_sort(data, len, &pdqsort, out, file_ans_name);

    } else if (!strcmp(argv[1], "timsort")) {
        test_sort(data, len, &timsort, out, file_ans_name);

    } else {
        printf("Invalid sort type\n");
        printf("Sorting options: heapsort, inssort, introsort, qsort, pdsqort\n");
    }
 
    free(data);

    return 0;
}

int *read_data_int(FILE *file, size_t *len) {
    size_t res = 0;
    int *data = NULL;
    
    if (!file) {
        fprintf(stderr, "Failure opening file <%p>\n", file);
        abort();
    }

    res = fscanf(file, "%lu", len);

    if (res != 1) {
        fprintf(stderr, "Failure reading nata: It is impossible to read the data for len\n");
        abort();
    }

    data = (int *) calloc(*len, sizeof(int));

    for (size_t i = 0; i < *len; i++)
        res += fscanf(file, "%d", data + i);

    if (res != *len + 1) {
        fprintf(stderr, "failure reading data: <%lu readed elem> vs <%lu = len + 1>\n", res, *len + 1);
        return NULL;
    }

    return data;
}

void test_sort(int *data, const size_t len, void (*sort)(void *, const size_t, const size_t, int (*cmp)(const void *, const void *)), const int out, const char *file_ans) {
    struct timespec start, finish;
    
    timespec_get(&start, TIME_UTC);
    sort(data, len, sizeof(int), &cmp_int);
    timespec_get(&finish, TIME_UTC);

    if (out > 0) {
        //printf("OUTPUT:\n");
        for (size_t i = 0; i < len; i++)
            printf("%d ", data[i]);
        
        printf("\n");
    }

    if (file_ans != NULL) {
        check_ans(file_ans, data, len);

    } else {
        printf("   -    | ");
    }

    printf("execution time: %lf\n", diff(start, finish));
}

void test_sort_all(int *data, const size_t len, const char *file_ans) {
    int *data_cpy = (int *) malloc(len * sizeof(int));

    printf("N = %lu\n", len);
    printf("HEAPSORT:  | ");
    memcpy(data_cpy, data, len * sizeof(int));
    test_sort(data_cpy, len, &heapsort, 0, file_ans);
    printf("-----------|---------|--------------------------\n");

    printf("INTROSORT: | ");
    memcpy(data_cpy, data, len * sizeof(int));
    test_sort(data_cpy, len, &introsort, 0, file_ans);
    printf("-----------|---------|--------------------------\n");

    printf("TIMSORT:   | ");
    memcpy(data_cpy, data, len * sizeof(int));
    test_sort(data_cpy, len, &timsort, 0, file_ans);
    printf("-----------|---------|--------------------------\n");

    printf("QSORT:     | ");
    memcpy(data_cpy, data, len * sizeof(int));
    test_sort(data_cpy, len, &qsort, 0, file_ans);
    printf("-----------|---------|--------------------------\n");

    printf("PDQSORT:   | ");
    memcpy(data_cpy, data, len * sizeof(int));
    test_sort(data_cpy, len, &pdqsort, 0, file_ans);

    free(data_cpy);
}

void check_ans(const char *file_ans_name, int *data, const size_t len) {
    size_t len_ans = 0;
    size_t num_incor = 0;
    FILE *file = fopen(file_ans_name, "r");

    if (!file) {
        fprintf(stderr, "Failure opening file <%s>:", file_ans_name);
        perror(":");
        return;
    }

    int *data_ans = read_data_int(file, &len_ans);
    fclose(file);

    assert(len == len_ans);

    for (size_t i = 0; i < len; i++)
        if (data_ans[i] != data[i]) {
            fprintf(stderr, "error: (data_ans[%lu] = %d) != (data[%lu] = %d)\n", i, data_ans[i], i, data[i]);
            num_incor++;
        }

    free(data_ans);

    if (num_incor) {
        printf("FAILURE | ");

    } else {
        printf("OK      | ");
    }
}
