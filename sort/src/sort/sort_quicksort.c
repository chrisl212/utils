//
//  sort_quicksort.c
//  utils
//
//  Created by Christopher Loonam on 5/20/19.
//  Copyright © 2019 Christopher Loonam. All rights reserved.
//

#include <stdlib.h>
#include <string.h>
#include "sort.h"

int partition(void *data, int start, int end, int size, sort_compare_t compare) {
    int i, j;
    void *pivot, *p1;

    i = start-1;
    j = end+1;
    p1 = _ptr(data, start + (end - start) / 2, size);
    pivot = malloc(size);
    memcpy(pivot, p1, size);
    while (1) {
        do {
            p1 = _ptr(data, ++i, size);
        } while (compare(p1, pivot) == SORT_LT);

        do {
            p1 = _ptr(data, --j, size);
        } while (compare(p1, pivot) == SORT_GT);

        if (i >= j) {
            break;
        }
        _swap(data, i, j, size);
    }
    free(pivot);
    return j;
}

static void _qsort(void *data, int start, int end, int size, sort_compare_t compare) {
    int p;
    if (start >= end) {
        return;
    }
    p = partition(data, start, end, size, compare);
    _qsort(data, start, p, size, compare);
    _qsort(data, p+1, end, size, compare);
}

void sort_quicksort(void *data, int len, int size, sort_compare_t compare) {
    _qsort(data, 0, len-1, size, compare);
}
