#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include "../csv/csv.h"

int main(int argc, char **argv) {
    csv_t *csv;
    uint32_t i;
    char **values;

    if (argc != 2) {
        printf("csv [file]\n");
        return -1;
    }

    csv = csv_loadFromPath(argv[1]);
    if (!csv) {
        printf("file %s could not be read.\n", argv[1]);
        return -1;
    }
    values = csv_getValuesInColumn(csv, 1);
    for (i = 0; values[i] != NULL; i++) {
        printf("%s\n", values[i]);
    }

    csv_writeToPath(csv, "diff.csv"); 
    csv_free(csv);
    free(values);
    return 0;
}
