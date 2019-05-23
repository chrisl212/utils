//
//  main.c
//  utils
//
//  Created by Christopher Loonam on 5/20/19.
//  Copyright © 2019 Christopher Loonam. All rights reserved.
//

#include <stdio.h>
#include "../mat/mat.h"

char *vals[5] = {"342.0", "22.23", "35", "2342", NULL};

int main(int argc, char **argv) {
    mat_t col = mat_columnFromStrings(vals);
    mat_t row = mat_rowFromStrings(vals);
    mat_t dot = mat_mult(2, row, col);
    vec_t angles = vec_create(3, 3.14159/2.0, 0.0, 0.0);
    mat_t rotation = mat_rotation(angles);
    mat_t identity = mat_identity(4);

//    mat_print(dot);
    mat_print(rotation);
//    mat_print(identity);

    mat_free(dot);
    mat_free(row);
    mat_free(col);
    mat_free(rotation);
    mat_free(identity);
    vec_free(angles);
    return 0;
}
