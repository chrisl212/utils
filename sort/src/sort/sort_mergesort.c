//
//  sort_mergesort.c
//  utils
//
//  Created by Christopher Loonam on 5/20/19.
//  Copyright © 2019 Christopher Loonam. All rights reserved.
//

#include <stdlib.h>
#include <string.h>
#include "sort.h"

static void _merge(void *data, void *tmp, int begin, int end, int size, sort_compare_t compare) {
    int mid, i, j, k;
    void *p1, *p2;
    
    if (begin == end - 1) {
        return;
    }
    
    mid = (begin + end) / 2;
    _merge(tmp, data, begin, mid, size, compare);
    _merge(tmp, data, mid, end, size, compare);

    j = begin;
    k = mid;
    for (i = begin; i < end; i++) {
        p1 = _ptr(tmp, j, size);
        p2 = _ptr(tmp, k, size);
        if (j < mid && (k >= end || compare(p1, p2) != SORT_GT)) {
            p2 = _ptr(data, i, size);
            memcpy(p2, p1, size);
            j++;
        } else {
            p1 = _ptr(data, i, size);
            memcpy(p1, p2, size);
            k++;
        }
    }
    p1 = _ptr(data, begin, size);
}

void sort_mergesort(void *data, int len, int size, sort_compare_t compare) {
    if (len == 0) {
        return;
    }
    void *tmp = malloc(size * len);
    memcpy(tmp, data, len * size);
    _merge(data, tmp, 0, len, size, compare); 
    free(tmp);
}
