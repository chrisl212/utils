#include <stdlib.h>
#include <string.h>
#include "img.h"

static void _strrev(char *s) {
    size_t len = strlen(s);
    int i;
    char *tmp = malloc((len+1) * sizeof(*tmp));

    for (i = 0; i < len; i++) {
        tmp[i] = s[len-i-1];
    }
    tmp[len] = '\0';
    memcpy(s, tmp, len+1);
    free(tmp);
}

static char *_getExt(const char *path) {
    char *ext = NULL;
    int sz = 1, i = strlen(path) - 1;

    while (i >= 0 && path[i] != '.') {
        ext = realloc(ext, sizeof(*ext) * ++sz);
        ext[sz-2] = path[i--];
    }
    ext[sz-1] = '\0';
    _strrev(ext);
    return ext;
}

img_t img_loadFromPath(const char *fpath) {
    img_t img; 
    char *ext = _getExt(fpath);

    if (strcmp(ext, "png") == 0) {
        img = png_loadFromPath(fpath);
    }

    free(ext);
    return img;
}

void img_free(img_t img) {
    free(img.bitmap);
}
