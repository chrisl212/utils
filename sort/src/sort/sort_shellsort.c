//
//  sort_shellsort.c
//  utils
//
//  Created by Christopher Loonam on 5/20/19.
//  Copyright © 2019 Christopher Loonam. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "sort.h"

static inline int _ceil(double d) {
    return ((int)d) + 1;
}

static int *_gaps(int len, int *gaps_len) {
    int idx, gap = 0, *gaps = NULL;

    *gaps_len = 0;
    do {
        gaps = realloc(gaps, ++(*gaps_len) * sizeof(*gaps));
        idx = *gaps_len - 1;
        if (idx == 0) {
            gaps[idx] = 1;
        } else {
            gaps[idx] = gap;
        }
        gap = _ceil(2.25*gaps[idx]+1);
    } while (gap < len);
    return gaps;
}

void sort_shellsort(void *data, int len, int size, sort_compare_t compare) {
    int i, j, gap_idx, gap, gaps_len, *gaps;
    void *tmp, *p1, *p2;

    if (len == 0) {
        return;
    }
    gaps = _gaps(len, &gaps_len);
    for (gap_idx = gaps_len-1; gap_idx >= 0; gap_idx--) {
        gap = gaps[gap_idx];
        for (i = gap; i < len; i++) {
            p1 = _ptr(data, i, size);
            tmp = malloc(size);
            memcpy(tmp, p1, size);
            for (j = i; j >= gap; j -= gap) {
                p1 = _ptr(data, j - gap, size);
                if (compare(p1, tmp) != SORT_GT) {
                    break;
                }
                p2 = _ptr(data, j, size);
                memcpy(p2, p1, size);
            }
            p1 = _ptr(data, j, size);
            memcpy(p1, tmp, size);
            free(tmp);
        }
    }

    free(gaps);
}
