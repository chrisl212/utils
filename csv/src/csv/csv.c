#include <stdlib.h>
#include <string.h>
#include "csv.h"

static void _loadRowsFromFile(csv_t *csv, FILE *fp) {
    uint32_t row_cnt = 1, i;
    row_t *rows;
    uint8_t *tmp, tmp_len, quotes = 0, in_val = 0;
    int16_t c, prev = '\n';

    fseek(fp, 0, SEEK_SET);
    while ((c = fgetc(fp))) {
        if (c == '\n' && prev != '\n')
            row_cnt++;
        if (c == EOF) {
            if (prev == '\n')
                row_cnt--;
            break;
        }       
        prev = c;
    }
    fseek(fp, 0, SEEK_SET);
    while ((c = fgetc(fp)) == '\n');
    if (ftell(fp) != 0)
        fseek(fp, -1, SEEK_CUR);

    csv->len = row_cnt;
    rows = calloc(row_cnt, sizeof(*rows));
    for (i = 0; i < row_cnt; i++) {
        rows[i].values = NULL;
        rows[i].len = 0;

        tmp_len = 1;
        tmp = calloc(tmp_len, sizeof(*tmp));
        while ((c = fgetc(fp)) != EOF) {
            if (tmp_len == 1 && i == 0 && c == '"' && !quotes)
                quotes = 1;
            if (c == '"' && (tmp_len == 1 || tmp[tmp_len-2] != '\\') && quotes) {
                in_val ^= 1;
                if (in_val == 1)
                    continue;
            }
            if ((c == ',' && !in_val) || c == '\n') {
                rows[i].values = realloc(rows[i].values, ++rows[i].len * sizeof(*rows[i].values));
                rows[i].values[rows[i].len - 1] = tmp;
                if (c == '\n')
                    break;
                tmp_len = 1;
                tmp = calloc(tmp_len, sizeof(*tmp));
            } else if (in_val || !quotes) {
                if (c != '"') {
                    tmp_len++;
                    tmp = realloc(tmp, sizeof(*tmp) * tmp_len);
                    tmp[tmp_len - 1] = '\0';
                }
                tmp[tmp_len - 2] = c;
            }
        }
    }

    csv->rows = rows;
}

csv_t *csv_loadFromPath(const char *fpath) {
    csv_t *csv;
    FILE *fp = fopen(fpath, "r");

    csv = csv_loadFromFile(fp);
    fclose(fp);
    return csv;
}

csv_t *csv_loadFromFile(FILE *fp) {
    csv_t *csv;
    if (!fp) {
        return NULL;
    }
    csv = malloc(sizeof(*csv));
    _loadRowsFromFile(csv, fp);
    return csv;
}

void csv_addRow(csv_t *csv, row_t row) {
    csv->rows = realloc(csv->rows, sizeof(*csv->rows) * ++csv->len);
    csv->rows[csv->len-1] = row;
}

void csv_writeToPath(csv_t *csv, const char *fpath) {
    FILE *fp = fopen(fpath, "w");
    csv_writeToFile(csv, fp);
    fclose(fp);
}

void csv_writeToFile(csv_t *csv, FILE *fp) {
    uint32_t i, j;
    row_t row;
    uint8_t *value;

    if (!fp) {
        return;
    }
    for (i = 0; i < csv->len; i++) {
        row = csv->rows[i];
        for (j = 0; j < row.len; j++) {
            value = row.values[j];
            fputc('"', fp);
            while (*value) {
                if (*value == '"') {
                    fputc('\\', fp);
                }
                fputc(*(value++), fp);
            }
            fputc('"', fp);
            if (j != row.len - 1)
                fputc(',', fp);
        }
        fputc('\n', fp);
    }
}

char **csv_getValuesInColumn(csv_t *csv, uint32_t col) {
    uint32_t i;
    char **values;

    if (csv->len == 0 || col >= csv->rows[0].len) {
        return NULL;
    }
    values = calloc(csv->len+1, sizeof(*values));
    
    for (i = 0; i < csv->len; i++) {
        values[i] = (char *)csv->rows[i].values[col];
    }
    return values;
}

static void _row_free(row_t row) {
    uint32_t i;
    for (i = 0; i < row.len; i++) {
        free(row.values[i]);
    }
    free(row.values);
}

void csv_free(csv_t *csv) {
    uint32_t i;
    for (i = 0; i < csv->len; i++) {
        _row_free(csv->rows[i]);
    }
    free(csv->rows);
    free(csv);
}
