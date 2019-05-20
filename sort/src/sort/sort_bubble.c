#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "sort.h"

void sort_bubble(void *data, int len, int size, sort_compare_t compare) {
    int i, n = len, no;
    void *p1, *p2;

    do {
        no = 0;
        for (i = 1; i < n; i++) {
            p1 = _ptr(data, i-1, size);
            p2 = _ptr(data, i, size);
            if (compare(p1, p2) == SORT_GT) {
                _swap(data, i-1, i, size);
                no = i;
            }
        }
        n = no;
    } while (n > 1);
}
