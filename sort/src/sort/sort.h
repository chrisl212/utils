#ifndef SORT_H
#define SORT_H

typedef enum {
    SORT_LT,
    SORT_EQ,
    SORT_GT
} sort_compareResult_t;

typedef int (*sort_compare_t)(const void *, const void *);

void sort_bubble(void *data, int len, int size, sort_compare_t compare);
void sort_selection(void *data, int len, int size, sort_compare_t compare);
void sort_insertion(void *data, int len, int size, sort_compare_t compare);
void sort_quicksort(void *data, int len, int size, sort_compare_t compare);
void sort_mergesort(void *data, int len, int size, sort_compare_t compare);
void sort_shellsort(void *data, int len, int size, sort_compare_t compare);
void _swap(void *data, int idx1, int idx2, int size);
void *_ptr(void *data, int idx, int size);

#endif
