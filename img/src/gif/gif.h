//
//  gif.h
//  utils
//
//  Created by Christopher Loonam on 5/20/19.
//  Copyright © 2019 Christopher Loonam. All rights reserved.
//

#ifndef GIF_H
#define GIF_H

#include <stdbool.h>
#include "gifBlock.h"

typedef struct {
    bool valid;
    gifBlock_t *blocks;
    uint16_t len;
} gif_t;

gif_t gif_loadFromPath(const char *fpath);
gifBlock_t gif_getBlock(gif_t gif, gifBlockType_t type);
void gif_free(gif_t gif);

#endif
