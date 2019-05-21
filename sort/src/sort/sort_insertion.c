//
//  sort_insertion.c
//  utils
//
//  Created by Christopher Loonam on 5/20/19.
//  Copyright © 2019 Christopher Loonam. All rights reserved.
//

#include "sort.h"

void sort_insertion(void *data, int len, int size, sort_compare_t compare) {
    int i, j;
    void *p1, *p2;

    for (i = 1; i < len; i++) {
        j = i;
        while (j > 0) {
            p1 = _ptr(data, j-1, size);
            p2 = _ptr(data, j, size);
            if (compare(p1, p2) != SORT_GT) {
                break;
            }
            _swap(data, j-1, j, size);
            j--;
        }
    }
}
