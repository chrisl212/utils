#include <stdlib.h>
#include <stdarg.h>
#include "mat.h"

vec_t vec_create(uint64_t len, ...) {
    vec_t vector = {0};
    va_list lst;
    double **vals = malloc(sizeof(*vals) * len);
    uint64_t i;

    va_start(lst, len);
    for (i = 0; i < len; i++) {
        vals[i] = malloc(sizeof(*vals[i]));
        vals[i][0] = va_arg(lst, double);
    }
    va_end(lst);
    vector.rows = len;
    vector.cols = 1;
    vector.vals = vals;
    return vector;
}

void vec_free(vec_t vector) {
    mat_free(vector);
}
