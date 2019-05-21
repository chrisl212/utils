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

typedef struct {
    bool valid;
    pngChunk_t *chunks;
    uint32_t chunksLen;
} png_t;

typedef struct img img_t;

img_t png_loadFromPath(const char *fpath);

#endif
