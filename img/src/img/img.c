//
//  img.c
//  utils
//
//  Created by Christopher Loonam on 5/20/19.
//  Copyright © 2019 Christopher Loonam. All rights reserved.
//

#include <stdlib.h>
#include <string.h>
#include <ctype.h>
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
    size_t sz = 1, i = strlen(path) - 1;

    while (i >= 0 && path[i] != '.') {
        ext = realloc(ext, sizeof(*ext) * ++sz);
        ext[sz-2] = tolower(path[i--]);
    }
    ext[sz-1] = '\0';
    _strrev(ext);
    return ext;
}

img_t img_loadFromPath(const char *fpath) {
    img_t img = {0}; 
    char *ext = _getExt(fpath);

    if (strcmp(ext, "png") == 0) {
        img.type = IMG_PNG;
        img.data.png = png_loadFromPath(fpath);
        if (img.data.png.valid == false) {
            img.type = IMG_INVALID;
        }
    } else if (strcmp(ext, "jpg") == 0) {
        img.type = IMG_JPG;
        img.data.jpg = jpg_loadFromPath(fpath);
        if (img.data.jpg.valid == false) {
            img.type = IMG_INVALID;
        }
    } else if (strcmp(ext, "gif") == 0) {
        img.type = IMG_GIF;
        img.data.gif = gif_loadFromPath(fpath);
        if (img.data.gif.valid == false) {
            img.type = IMG_INVALID;
        }
    }

    free(ext);
    return img;
}

void img_getDimensions(img_t img, uint32_t *width, uint32_t *height) {
    pngChunk_t IHDR;
    
    if (img.type == IMG_PNG) {
        IHDR = png_getChunk(img.data.png, "IHDR");
        *width = IHDR.data.IHDR.width;
        *height = IHDR.data.IHDR.height;
    } else if (img.type == IMG_GIF) {
        *width = img.data.gif.logicalScreenDescriptor.width;
        *height = img.data.gif.logicalScreenDescriptor.height;
    }
}

void img_free(img_t img) {
    switch (img.type) {
        case IMG_PNG:
            png_free(img.data.png);
            break;
            
        default:
            break;
    }
}
