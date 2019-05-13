#ifndef CSV_H
#define CSV_H

#include <stdio.h>
#include <stdint.h>

typedef struct {
    uint8_t **values;
    uint32_t len;
} row_t;

typedef struct {
    row_t *rows;
    uint32_t len;
} csv_t;

csv_t *csv_loadFromPath(const char *fpath);
csv_t *csv_loadFromFile(FILE *fp);
void csv_addRow(csv_t *csv, row_t row);
void csv_writeToPath(csv_t *csv, const char *fpath);
void csv_writeToFile(csv_t *csv, FILE *fp);
char **csv_getValuesInColumn(csv_t *csv, uint32_t col);
void csv_free(csv_t *csv);

#endif
