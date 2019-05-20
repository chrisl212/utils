#include "sort.h"

void sort_selection(void *data, int len, int size, sort_compare_t compare) {
    int i, j, min_idx;
    void *p1, *p2;

    for (i = 0; i < len-1; i++) {
        min_idx = i;
        for (j = i+1; j < len; j++) {
            p1 = _ptr(data, j, size);
            p2 = _ptr(data, min_idx, size);
            if (compare(p1, p2) == SORT_LT) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            _swap(data, i, min_idx, size);
        }
    }
}
