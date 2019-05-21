//
//  png.h
//  utils
//
//  Created by Christopher Loonam on 5/20/19.
//  Copyright © 2019 Christopher Loonam. All rights reserved.
//

#ifndef PNG_H
#define PNG_H

#include <stdbool.h>
#include "pngChunk.h"

typedef struct png {
    bool valid;
    pngChunk_t *chunks;
    uint32_t chunksLen;
} png_t;

png_t png_loadFromPath(const char *fpath);
pngChunk_t png_getChunk(png_t png, const char *chunk);
void png_free(png_t png);

#endif
