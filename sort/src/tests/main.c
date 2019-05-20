#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../sort/sort.h"

int int_cmp(const void *p1, const void *p2) {
    int i1, i2;
    i1 = *(int *)p1;
    i2 = *(int *)p2;
    if (i1 < i2) {
        return SORT_LT;
    } else if (i1 == i2) {
        return SORT_EQ;
    } else {
        return SORT_GT;
    }
}

int *load_data(int *size, const char *fname) {
    int tmp;
    int *data = NULL;
    FILE *f = fopen(fname, "r");
    
    *size = 0;
    fscanf(f, "%d", &tmp);
    while (!feof(f)) {
        data = realloc(data, ++(*size) * sizeof(*data));
        data[*size-1] = tmp;
        fscanf(f, "%d", &tmp);
    }

    fclose(f);
    return data;
}

void write_data(int *data, int size, const char *fname) {
    int i;
    FILE *f = fopen(fname, "w");

    for (i = 0; i < size; i++) {
        fprintf(f, "%d\n", data[i]);
    }

    fclose(f);
}

int main(int argc, char **argv) {
    char *option;
    int len = 0, *data;

    if (argc != 3) {
        printf("sort -[b|s|i|q|m|sh] [FILE]\n");
        return -1;
    }
    option = argv[1];
    data = load_data(&len, argv[2]);

    if (option[1] == 'b') {
        sort_bubble(data, len, sizeof(*data), int_cmp);
    } else if (strcmp(option, "-s") == 0) {
        sort_selection(data, len, sizeof(*data), int_cmp);
    } else if (option[1] == 'i') {
        sort_insertion(data, len, sizeof(*data), int_cmp);
    } else if (option[1] == 'm') {
        sort_mergesort(data, len, sizeof(*data), int_cmp);
    } else if (option[1] == 'q') {
        sort_quicksort(data, len, sizeof(*data), int_cmp);
    } else if (strcmp(option, "-sh") == 0) {
        sort_shellsort(data, len, sizeof(*data), int_cmp);
    } else if (strcmp(option, "-cq") == 0) {
        qsort(data, len, sizeof(*data), int_cmp);
    }

    if (len > 0) {
        write_data(data, len, "sort.out");
    }
   
    free(data); 
    return 0;
}
