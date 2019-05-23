//
//  mat.c
//  utils
//
//  Created by Christopher Loonam on 5/20/19.
//  Copyright © 2019 Christopher Loonam. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <math.h>
#include "mat.h"

typedef enum {
    MAT_MULT,
    MAT_ADD,
    MAT_SUB
} matOp_t;

mat_t mat_create(uint64_t rows, uint64_t cols, ...) {
    mat_t matrix;
    uint64_t i, j;
    va_list lst;
    
    matrix.rows = rows;
    matrix.cols = cols;
    matrix.vals = malloc(rows * sizeof(*matrix.vals));
    va_start(lst, cols);
    for (i = 0; i < rows; i++) {
        matrix.vals[i] = malloc(cols * sizeof(*matrix.vals[i]));
        for (j = 0; j < cols; j++) {
            matrix.vals[i][j] = va_arg(lst, double);
        }
    }
    va_end(lst);
    return matrix;
}

mat_t mat_columnFromStrings(char **vals) {
    mat_t matrix;
    matrix.cols = 1;
    matrix.rows = 0;
    matrix.vals = NULL;

    while (*vals) {
        matrix.vals = realloc(matrix.vals, ++matrix.rows * sizeof(*matrix.vals));
        matrix.vals[matrix.rows-1] = malloc(sizeof(*matrix.vals[matrix.rows-1]));
        matrix.vals[matrix.rows-1][0] = strtod(*vals, NULL);
        vals++;
    }
    return matrix;
}

mat_t mat_rowFromStrings(char **vals) {
    mat_t matrix;
    matrix.cols = 0;
    matrix.rows = 1;
    matrix.vals = malloc(sizeof(*matrix.vals));

    while (*vals) {
        matrix.vals[0] = realloc(matrix.vals[0], ++matrix.cols * sizeof(*matrix.vals[0]));
        matrix.vals[0][matrix.cols-1] = strtod(*vals, NULL);
        vals++;
    }
    return matrix;
}

static double _product(mat_t matrix1, mat_t matrix2, uint64_t r, uint64_t c) {
    double sum = 0.0;
    uint64_t i;
    for (i = 0; i < matrix1.cols; i++) {
        sum += matrix1.vals[r][i] * matrix2.vals[i][c];
    }
    return sum;
}

static mat_t _mult(mat_t matrix1, mat_t matrix2) {
    mat_t res;
    uint64_t i, j;

    res.rows = matrix1.rows;
    res.cols = matrix2.cols;
    res.vals = malloc(res.rows * sizeof(*res.vals));

    for (i = 0; i < res.rows; i++) {
        res.vals[i] = malloc(res.cols * sizeof(*res.vals[i]));
        for (j = 0; j < res.cols; j++) {
            res.vals[i][j] = _product(matrix1, matrix2, i, j);
        }
    }
    return res;
}

static mat_t _add(mat_t matrix1, mat_t matrix2) {
    mat_t res = {0};
    uint64_t i, j;

    if (matrix1.rows != matrix2.rows || matrix1.cols != matrix2.cols) {
        return res;
    }
    res.rows = matrix1.rows;
    res.cols = matrix1.cols;
    res.vals = malloc(res.rows * sizeof(*res.vals));

    for (i = 0; i < res.rows; i++) {
        res.vals[i] = malloc(res.cols * sizeof(*res.vals[i]));
        for (j = 0; j < res.cols; j++) {
            res.vals[i][j] = matrix1.vals[i][j] + matrix2.vals[i][j];
        }
    }
    return res;
}

static mat_t _sub(mat_t matrix1, mat_t matrix2) {
    mat_t res = {0};
    uint64_t i, j;

    if (matrix1.rows != matrix2.rows || matrix1.cols != matrix2.cols) {
        return res;
    }
    res.rows = matrix1.rows;
    res.cols = matrix1.cols;
    res.vals = malloc(res.rows * sizeof(*res.vals));

    for (i = 0; i < res.rows; i++) {
        res.vals[i] = malloc(res.cols * sizeof(*res.vals[i]));
        for (j = 0; j < res.cols; j++) {
            res.vals[i][j] = matrix1.vals[i][j] - matrix2.vals[i][j];
        }
    }
    return res;
}

static mat_t _op(matOp_t op, va_list lst, uint64_t num) {
    mat_t tmp, res = {0};
    uint64_t i;

    if (num <= 1) {
        return res;
    }

    tmp = va_arg(lst, mat_t);
    for (i = 0; i < num-1; i++) {
        switch (op) {
            case MAT_MULT:
                res = _mult(tmp, va_arg(lst, mat_t));
                break;
            case MAT_ADD:
                res = _add(tmp, va_arg(lst, mat_t));
                break;
            case MAT_SUB:
                res = _sub(tmp, va_arg(lst, mat_t));
                break;
            default:
                break;
        }
        if (i != 0) {
            mat_free(tmp);
        }
        tmp = res;
    }

    return res;
}

mat_t mat_mult(uint64_t num, ...) {
    va_list lst;
    mat_t res;

    va_start(lst, num);
    res = _op(MAT_MULT, lst, num);
    va_end(lst);
    return res;
}

mat_t mat_add(uint64_t num, ...) {
    va_list lst;
    mat_t res;

    va_start(lst, num);
    res = _op(MAT_ADD, lst, num);
    va_end(lst);
    return res;
}

mat_t mat_sub(uint64_t num, ...) {
    va_list lst;
    mat_t res;

    va_start(lst, num);
    res = _op(MAT_SUB, lst, num);
    va_end(lst);
    return res;
}

mat_t mat_scale(mat_t matrix, double scale) {
    mat_t res = {0};
    uint64_t i, j;

    res.rows = matrix.rows;
    res.cols = matrix.cols;
    res.vals = malloc(res.rows * sizeof(*res.vals));

    for (i = 0; i < res.rows; i++) {
        res.vals[i] = malloc(res.cols * sizeof(*res.vals[i]));
        for (j = 0; j < res.cols; j++) {
            res.vals[i][j] = matrix.vals[i][j] * scale;
        }
    }
    return res;
}

mat_t mat_transpose(mat_t matrix) {
    mat_t res = {0};
    uint64_t i, j;

    res.rows = matrix.cols;
    res.cols = matrix.rows;
    res.vals = malloc(res.rows * sizeof(*res.vals));

    for (i = 0; i < res.rows; i++) {
        res.vals[i] = malloc(res.cols * sizeof(*res.vals[i]));
        for (j = 0; j < res.cols; j++) {
            res.vals[i][j] = matrix.vals[j][i];
        }
    }
    return res;
}

mat_t mat_inverse(mat_t matrix) {
    return matrix;
}

mat_t mat_identity(uint64_t dim) {
    mat_t matrix = {0};
    uint64_t i, j;
    matrix.rows = matrix.cols = dim;
    matrix.vals = malloc(sizeof(*matrix.vals) * dim);

    for (i = 0; i < dim; i++) {
        matrix.vals[i] = malloc(sizeof(*matrix.vals[i]) * dim);
        for (j = 0; j < dim; j++) {
            if (i == j) {
                matrix.vals[i][j] = 1.0;
            } else {
                matrix.vals[i][j] = 0.0;
            }
        }
    }
    return matrix;
}

mat_t mat_rotation(vec_t angles) {
    mat_t rotX, rotY, rotZ, rotation = {0};
    double cosA, sinA, cosB, sinB, cosC, sinC;

    if (angles.rows != 3 || angles.cols != 1) {
        return rotation;
    }

    cosA = cos(angles.vals[0][0]);
    cosB = cos(angles.vals[1][0]);
    cosC = cos(angles.vals[2][0]);
    sinA = sin(angles.vals[0][0]);
    sinB = sin(angles.vals[1][0]);
    sinC = sin(angles.vals[2][0]);

    rotX = mat_create(4, 4,
                      1.0, 0.0, 0.0, 0.0,
                      0.0, cosA, sinA, 0.0,
                      0.0, -sinA, cosA, 0.0,
                      0.0, 0.0, 0.0, 1.0);
    rotY = mat_create(4, 4,
                      cosB, 0.0, sinB, 0.0,
                      0.0, 1.0, 0.0, 0.0,
                      -sinB, 0.0, cosB, 0.0,
                      0.0, 0.0, 0.0, 1.0);
    rotZ = mat_create(4, 4,
                      cosC, -sinC, 0.0, 0.0,
                      sinC, cosC, 0.0, 0.0,
                      0.0, 0.0, 1.0, 0.0,
                      0.0, 0.0, 0.0, 1.0);

    rotation = mat_mult(3, rotX, rotY, rotZ);
    mat_free(rotX);
    mat_free(rotY);
    mat_free(rotZ);
    return rotation;
}

mat_t mat_translation(vec_t delta) {
    return mat_create(4, 4,
                      1.0, 0.0, 0.0, delta.vals[0][0],
                      0.0, 1.0, 0.0, delta.vals[1][0],
                      0.0, 0.0, 1.0, delta.vals[2][0],
                      0.0, 0.0, 0.0, 1.0);
}

void mat_print(mat_t matrix) {
    uint64_t i, j;

    printf("[");
    for (i = 0; i < matrix.rows; i++) {
        for (j = 0; j < matrix.cols; j++) {
            printf("%lf", matrix.vals[i][j]);
            if (j != matrix.cols-1) {
                printf(" ");
            }
        }
        if (i == matrix.rows-1) 
            printf("]");
        printf("\n");
    }
    printf("\n");
}

void mat_free(mat_t matrix) {
    uint64_t rows, i;
    rows = matrix.rows;

    if (!matrix.vals) {
        return;
    }

    for (i = 0; i < rows; i++) {
        free(matrix.vals[i]);
    }
    free(matrix.vals);
}
