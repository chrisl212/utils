//
//  mat.c
//  utils
//
//  Created by Christopher Loonam on 5/20/19.
//  Copyright © 2019 Christopher Loonam. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "mat.h"

mat_t mat_create(uint64_t rows, uint64_t cols, double **vals) {
    mat_t matrix;
    uint64_t i, j;
    
    matrix.rows = rows;
    matrix.cols = cols;
    matrix.vals = malloc(rows * sizeof(*matrix.vals));
    for (i = 0; i < rows; i++) {
        matrix.vals[i] = malloc(cols * sizeof(*matrix.vals[i]));
        for (j = 0; j < cols; j++) {
            matrix.vals[i][j] = vals[i][j];
        }
    }
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

static double _mult(mat_t matrix1, mat_t matrix2, uint64_t r, uint64_t c) {
    double sum = 0.0;
    uint64_t i, j;
    for (i = 0; i < matrix1.cols; i++) {
        for (j = 0; j < matrix2.rows; j++) {
            sum += matrix1.vals[r][i] * matrix2.vals[j][c];
        }
    }
    return sum;
}

mat_t mat_mult(mat_t matrix1, mat_t matrix2) {
    mat_t res;
    uint64_t i, j;

    res.rows = matrix1.rows;
    res.cols = matrix2.cols;
    res.vals = malloc(res.rows * sizeof(*res.vals));

    for (i = 0; i < res.rows; i++) {
        res.vals[i] = malloc(res.cols * sizeof(*res.vals[i]));
        for (j = 0; j < res.cols; j++) {
            res.vals[i][j] = _mult(matrix1, matrix2, i, j);
        }
    }
    return res;
}

mat_t mat_add(mat_t matrix1, mat_t matrix2) {
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

mat_t mat_sub(mat_t matrix1, mat_t matrix2) {
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
}

void mat_free(mat_t matrix) {
    uint64_t rows, i;
    rows = matrix.rows;

    for (i = 0; i < rows; i++) {
        free(matrix.vals[i]);
    }
    free(matrix.vals);
}
