//
//  json.h
//  utils
//
//  Created by Christopher Loonam on 5/20/19.
//  Copyright © 2019 Christopher Loonam. All rights reserved.
//

#ifndef JSON_H
#define JSON_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum {
    JSON_STR,
    JSON_INT,
    JSON_UINT,
    JSON_FLOAT,
    JSON_BOOL,
    JSON_NULL,
    JSON_OBJ,
    JSON_ARR
} jsonType_t;

typedef struct json_t json_t;

typedef struct {
    json_t *children;
    char **keys;
} jsonObject_t;

typedef struct {
    json_t *children;
    uint64_t len;
} jsonArray_t;

typedef union {
    char *sval;
    int64_t ival;
    uint64_t uival;
    double fval;
    bool bval;
    bool null;
    jsonArray_t aval;
    jsonObject_t oval;
} jsonValue_t;

struct json_t {
    jsonValue_t value;
    jsonType_t type;
};

json_t json_loadFromPath(const char *fpath);
json_t json_loadFromFile(FILE *fp);
json_t json_objectForKey(json_t dict, const char *key);
json_t json_objectAtIndex(json_t arr, uint32_t idx);
void json_write(json_t json, FILE *fp);
void json_free(json_t json);

#endif
