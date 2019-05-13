#include <stdio.h>
#include "../mat/mat.h"

char *vals[5] = {"342.0", "22.23", "35", "2342", NULL};

int main(int argc, char **argv) {
    mat_t col = mat_columnFromStrings(vals);
    mat_t row = mat_rowFromStrings(vals);
    mat_t dot = mat_mult(row, col);
    
    mat_print(dot);

    mat_free(dot);
    mat_free(row);
    mat_free(col);
    return 0;
}
