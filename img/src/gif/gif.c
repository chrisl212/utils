//
//  gif.c
//  utils
//
//  Created by Christopher Loonam on 5/21/19.
//  Copyright © 2019 Christopher Loonam. All rights reserved.
//

#include <stdio.h>
#include "gif.h"

static int _strcmp(uint8_t *b1, uint8_t *b2, int len) {
    while (--len >= 0) {
        if (b1[len] - b2[len] != 0) {
            return b1[len] - b2[len];
        }
    }
    return 0;
}

gif_t gif_loadFromPath(const char *fpath) {
    gif_t gif = {0};
    FILE *f = fopen(fpath, "r");
    
    fread(&gif.header, sizeof(gif.header), 1, f);
    if (_strcmp(gif.header.signature, (uint8_t *)"GIF", 3) != 0 || (_strcmp(gif.header.version, (uint8_t *)"87a", 3) != 0 && _strcmp(gif.header.version, (uint8_t *)"89a", 3) !=0)) {
        return gif;
    } else {
        gif.valid = true;
    }
    fread(&gif.logicalScreenDescriptor, sizeof(gif.logicalScreenDescriptor), 1, f);
    
    fclose(f);
    return gif;
}

void gif_free(gif_t jpg) {
    
}
