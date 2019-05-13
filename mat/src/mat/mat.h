#ifndef MAT_H
#define MAT_H

#include <stdint.h>

typedef struct {
    uint64_t rows;
    uint64_t cols;
    double **vals;
} mat_t;

mat_t mat_create(uint64_t rows, uint64_t cols, double **vals);
mat_t mat_columnFromStrings(char **vals);
mat_t mat_rowFromStrings(char **vals);
mat_t mat_mult(mat_t matrix1, mat_t matrix2);
mat_t mat_add(mat_t matrix1, mat_t matrix2);
mat_t mat_sub(mat_t matrix1, mat_t matrix2);
mat_t mat_scale(mat_t matrix, double scale);
mat_t mat_transpose(mat_t matrix);
mat_t mat_inverse(mat_t matrix);
void mat_print(mat_t matrix);
void mat_free(mat_t matrix);

#endif
