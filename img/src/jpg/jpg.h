//
//  jpg.h
//  utils
//
//  Created by Christopher Loonam on 5/20/19.
//  Copyright © 2019 Christopher Loonam. All rights reserved.
//

#ifndef JPG_H
#define JPG_H

#include <stdbool.h>

typedef struct {
    bool valid;
} jpg_t;

jpg_t jpg_loadFromPath(const char *fpath);
void jpg_free(jpg_t jpg);

#endif
