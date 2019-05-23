//
//  mat.h
//  utils
//
//  Created by Christopher Loonam on 5/20/19.
//  Copyright © 2019 Christopher Loonam. All rights reserved.
//

#ifndef MAT_H
#define MAT_H

#include <stdint.h>

typedef struct {
    uint64_t rows;
    uint64_t cols;
    double **vals;
} mat_t;

#include "vec.h"

//Creating
mat_t mat_create(uint64_t rows, uint64_t cols, ...);
mat_t mat_columnFromStrings(char **vals);
mat_t mat_rowFromStrings(char **vals);

//Arithmetic
mat_t mat_mult(uint64_t num, ...);
mat_t mat_add(uint64_t num, ...);
mat_t mat_sub(uint64_t num, ...);
mat_t mat_scale(mat_t matrix, double scale);
mat_t mat_transpose(mat_t matrix);
mat_t mat_inverse(mat_t matrix);
mat_t mat_identity(uint64_t dim);

//Transformations
mat_t mat_rotation(vec_t angles);
mat_t mat_translation(vec_t delta);

void mat_print(mat_t matrix);
void mat_free(mat_t matrix);

#endif
