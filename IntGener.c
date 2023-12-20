#include <stdio.h> 
#include <stdlib.h>

static void generate_data(const int num_elem, const int max_num);

int main(int argc, char * *argv) {
    if (argc != 3) {
        printf("Usage: %s <num_elem> <max_num> > <output_file>\n", argv[0]);
        return -1;
    }

    char* endptr;
    int num_elem = strtol(argv[1], &endptr, 10);

    if (endptr == argv[1]) {
        printf("<%s> cannot be converted to int\n", argv[1]);
        return -1;
    }
    
    int max_num = strtol(argv[2], &endptr, 10);

    if (endptr == argv[2]) {
        printf("<%s> cannot be converted to int\n", argv[3]);
        return -1;
    }

    generate_data(num_elem, max_num);

    return 0;
}

static void generate_data(const int num_elem, const int max_num) {
    printf("%d\n", num_elem);

    for (int i = 0; i < num_elem; i++)
        printf("%d ", (int) (random() % max_num));

    printf("\n");
}
