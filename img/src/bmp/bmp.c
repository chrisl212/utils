//
//  bmp.c
//  utils
//
//  Created by Christopher Loonam on 5/21/19.
//  Copyright © 2019 Christopher Loonam. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

static int _strcmp(uint8_t *b1, uint8_t *b2, int len) {
    while (--len >= 0) {
        if (b1[len] - b2[len] != 0) {
            return b1[len] - b2[len];
        }
    }
    return 0;
}

bmp_t bmp_loadFromPath(const char *fpath) {
    bmp_t bmp = {0};
    FILE *f = fopen(fpath, "rb");
    
    if (!f) {
        return bmp;
    }
    
    fread(&bmp.header, sizeof(bmp.header), 1, f);
    if (_strcmp(bmp.header.signature, (uint8_t *)"BM", 2) != 0) {
        fclose(f);
        return bmp;
    } else {
        bmp.valid = true;
    }
    if (bmp.header.bitsPerPixel <= 8) {
        fseek(f, bmp.header.headerSize+14, SEEK_SET);
        bmp.colorTable.len = bmp.header.numColors;
        bmp.colorTable.colors = malloc(sizeof(*bmp.colorTable.colors) * bmp.colorTable.len);
        fread(bmp.colorTable.colors, sizeof(*bmp.colorTable.colors), bmp.colorTable.len, f);
    }
    fseek(f, bmp.header.dataOffset, SEEK_SET);
    bmp.pixelData = malloc(sizeof(*bmp.pixelData) * bmp.header.imageSize);
    fread(bmp.pixelData, sizeof(*bmp.pixelData), bmp.header.imageSize, f);
    
    fclose(f);
    return bmp;
}

void bmp_free(bmp_t bmp) {
    free(bmp.pixelData);
    if (bmp.colorTable.colors) {
        free(bmp.colorTable.colors);
    }
}
