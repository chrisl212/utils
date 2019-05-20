#include <string.h>
#include <stdlib.h>
#include "sort.h"

void *_ptr(void *data, int idx, int size) {
    return (char *)data + (idx * size);
}

void _swap(void *data, int idx1, int idx2, int size) {
    void *tmp = malloc(size);
    void *p1 = _ptr(data, idx1, size);
    void *p2 = _ptr(data, idx2, size);
    memcpy(tmp, p1, size);
    memcpy(p1, p2, size);
    memcpy(p2, tmp, size);
    free(tmp);
}

