#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "json.h"

static json_t _parse_object(FILE *fp);
static json_t _parse_array(FILE *fp);

static void _ignore_whitespace(FILE *fp) {
    while (isspace(fgetc(fp)));
    fseek(fp, -1, SEEK_CUR);
}

static void _ignore_until(FILE *fp, char c) {
    while (fgetc(fp) != c);
}

static char *_parse_string(FILE *fp) {
    char c, *tmp;
    uint64_t tmp_len = 1;
    bool in_val = false;

    tmp = calloc(tmp_len, sizeof(*tmp));
    while ((c = fgetc(fp))) {
        if (c == '"' && !in_val) {
            in_val = true;
            continue;
        } else if (c == '"') {
            in_val = false;
            break;
        } else if (in_val) {
            tmp = realloc(tmp, ++tmp_len * sizeof(*tmp));
            if (c == '\\') {
                c = fgetc(fp);
            }
            tmp[tmp_len-2] = c;
            tmp[tmp_len-1] = '\0';
        }
    }
    return tmp;
}

static json_t _parse_number(FILE *fp) {
    char c, *tmp;
    uint64_t tmp_len = 1;
    json_t num;

    tmp = calloc(tmp_len, sizeof(*tmp));
    c = fgetc(fp);
    if (c == '+' || c == '-') {
        num.type = JSON_INT;
    } else {
        num.type = JSON_UINT;
    }
    while (isdigit(c) || c == '.' || c == '+' || c == '-' || tolower(c) == 'e') {
        if (c == '.' || tolower(c) == 'e') {
            num.type = JSON_FLOAT;
        }
        tmp = realloc(tmp, ++tmp_len * sizeof(*tmp));
        tmp[tmp_len-2] = c;
        tmp[tmp_len-1] = '\0';
        c = fgetc(fp);
    }
    switch (num.type) {
        case JSON_INT:
            num.value.ival = strtoll(tmp, NULL, 10);
            break;
        case JSON_UINT:
            num.value.uival = strtoull(tmp, NULL, 10);
            break;
        case JSON_FLOAT:
            num.value.fval = strtod(tmp, NULL);
            break;
        default:
            break;
    }
    free(tmp);
    return num;
}

static json_t _parse_bool(FILE *fp) {
    json_t b;
    b.type = JSON_BOOL;
    b.value.bval = (fgetc(fp) == 't');
    while (isalpha(fgetc(fp)));
    fseek(fp, -1, SEEK_CUR);
    return b;
}

static json_t _parse_item(FILE *fp) {
    json_t item;
    char c;

    _ignore_whitespace(fp);
    c = fgetc(fp);
    fseek(fp, -1, SEEK_CUR);
    if (c == '"') {
        item.type = JSON_STR;
        item.value.sval = _parse_string(fp);
    } else if (c == '-' || c == '+' || isdigit(c)) {
        item = _parse_number(fp);
    } else if (c == '{') {
        item = _parse_object(fp);
    } else if (c == '[') {
        item = _parse_array(fp);
    } else if (c == 't' || c == 'f') {
        item = _parse_bool(fp);
    } else if (c == 'n') {
        item.type = JSON_NULL;
    }
    return item;
}

static json_t _parse_object(FILE *fp) {
    char c;
    json_t object;
    char **keys;
    json_t *children;
    uint64_t key_len = 1;

    object.type = JSON_OBJ;
    _ignore_whitespace(fp);
    keys = calloc(key_len, sizeof(*keys));
    children = NULL;
    
    while ((c = fgetc(fp)) != '}') {
        fseek(fp, -1, SEEK_CUR);
        if (c == ',') 
            fseek(fp, 1, SEEK_CUR);
        keys = realloc(keys, ++key_len * sizeof(*keys));
        children = realloc(children, (key_len-1) * sizeof(*children));
        
        keys[key_len-2] = _parse_string(fp);
        keys[key_len-1] = NULL;
        _ignore_until(fp, ':');
        children[key_len-2] = _parse_item(fp);

        _ignore_whitespace(fp);
    }
                
    object.value.oval.keys = keys;
    object.value.oval.children = children;
    return object;
}

static json_t _json_zero() {
    json_t zero = {0};
    return zero;
}

static json_t _parse_array(FILE *fp) {
    char c;
    json_t array, *children;
    uint64_t child_len = 1;

    array.type = JSON_ARR;
    _ignore_whitespace(fp);
    
    if (fgetc(fp) != '[') 
        fseek(fp, -1, SEEK_CUR);
    children = calloc(child_len, sizeof(*children));
    while ((c = fgetc(fp)) != ']') {
        fseek(fp, -1, SEEK_CUR);
        if (c == ',') 
            fseek(fp, 1, SEEK_CUR);
        children = realloc(children, ++child_len * sizeof(*children));
        
        children[child_len-2] = _parse_item(fp);
        children[child_len-1] = _json_zero();

        _ignore_whitespace(fp);
    }
                
    array.value.aval = children;
    return array;
}

static json_t _parse(FILE *fp) {
    json_t j = {0};
    char c;
    
    while (isspace((c = fgetc(fp))));
    if (c == '{')
       return _parse_object(fp);
    else if (c == '[')
       return _parse_array(fp);
    return j;
}

static json_t _json_null() {
    json_t null;
    null.type = JSON_NULL;
    return null;
}

json_t json_loadFromPath(const char *fpath) {
    json_t json;
    FILE *fp = fopen(fpath, "r");
    json = json_loadFromFile(fp);
    fclose(fp);
    return json;
}

json_t json_loadFromFile(FILE *fp) {
    json_t json = {0};

    if (!fp) 
        return _json_null();

    json = _parse(fp);
    return json;
}

json_t json_objectForKey(json_t dict, const char *key) {
    jsonChild_t children = dict.value.oval;
    json_t res = _json_null();

    uint64_t idx = 0;
    for (idx = 0; children.keys[idx] != NULL; idx++) {
        if (strcmp(children.keys[idx], key) == 0) {
            res = children.children[idx];
            break;
        }
    }
    return res;    
}

json_t json_objectAtIndex(json_t arr, uint32_t idx) {
    return arr.value.aval[idx];
}

static void _write_indents(FILE *fp, uint16_t indents) {
    while (indents--) {
        fprintf(fp, "    ");
    }
}

static void _write(json_t json, FILE *fp, uint16_t indents) {
    uint64_t i;

    if (json.type == JSON_OBJ) {
        fprintf(fp, "{\n");
        for (i = 0; json.value.oval.keys[i] != NULL; i++) {
            _write_indents(fp, indents);
            fprintf(fp, "\"%s\" : ", json.value.oval.keys[i]);
            _write(json.value.oval.children[i], fp, indents+1);
            if (json.value.oval.keys[i+1]) 
                fprintf(fp, ",");
            fprintf(fp, "\n");
        }
        _write_indents(fp, indents-1);
        fprintf(fp, "}");
    } else if (json.type == JSON_ARR) {
        fprintf(fp, "[\n");
        for (i = 0; json.value.aval[i].type != 0 || json.value.aval[i].value.sval != NULL; i++) {
            _write_indents(fp, indents);
            _write(json.value.aval[i], fp, indents+1);
            if (json.value.aval[i+1].type != 0 || json.value.aval[i+1].value.sval != NULL)
                fprintf(fp, ",");
            fprintf(fp, "\n");
        }
        _write_indents(fp, indents-1);
        fprintf(fp, "]");
    } else if (json.type == JSON_STR) {
        fputc('"', fp);
        for (i = 0; i < strlen(json.value.sval); i++) {
            if (json.value.sval[i] == '"') {
                fputc('\\', fp);
            }
            fputc(json.value.sval[i], fp);
        }
        fputc('"', fp);
    } else if (json.type == JSON_INT) {
        fprintf(fp, "%ld", json.value.ival);
    } else if (json.type == JSON_UINT) {
        fprintf(fp, "%lu", json.value.uival);
    } else if (json.type == JSON_FLOAT) {
        fprintf(fp, "%lf", json.value.fval);
    } else if (json.type == JSON_BOOL) {
        fprintf(fp, "%s", (json.value.bval) ? "true" : "false");
    } else if (json.type == JSON_NULL) {
        fprintf(fp, "NULL");
    }
}

void json_write(json_t json, FILE *fp) {
    _write(json, fp, 1);
    fprintf(fp, "\n");
}

void json_free(json_t json) {
    uint64_t i;

    if (json.type == JSON_STR && json.value.sval) {
        free(json.value.sval);
    } else if (json.type == JSON_ARR) {
        for (i = 0; json.value.aval[i].type != 0 || json.value.aval[i].value.sval != NULL; i++) {
            json_free(json.value.aval[i]);
        }
        free(json.value.aval);
    } else if (json.type == JSON_OBJ) {
        for (i = 0; json.value.oval.keys[i] != NULL; i++) {
            json_free(json.value.oval.children[i]);
            free(json.value.oval.keys[i]);
        }
        free(json.value.oval.children);
        free(json.value.oval.keys);
    }
}

