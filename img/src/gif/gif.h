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
    gifBlockHeader_t header;
    gifBlockLogicalScreenDescriptor_t logicalScreenDescriptor;
} gif_t;

gif_t gif_loadFromPath(const char *fpath);
void gif_free(gif_t jpg);

#endif
