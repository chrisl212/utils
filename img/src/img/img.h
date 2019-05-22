//
//  img.h
//  utils
//
//  Created by Christopher Loonam on 5/20/19.
//  Copyright © 2019 Christopher Loonam. All rights reserved.
//

#ifndef IMG_H
#define IMG_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "../png/png.h"
#include "../jpg/jpg.h"
#include "../gif/gif.h"
#include "../bmp/bmp.h"

typedef enum {
    IMG_INVALID,
    IMG_PNG,
    IMG_JPG,
    IMG_BMP,
    IMG_GIF
} imgType_t;

typedef union {
    png_t png;
    jpg_t jpg;
    bmp_t bmp;
    gif_t gif;
} imgData_t;

typedef struct img {
    imgType_t type;
    imgData_t data;
} img_t;

img_t img_loadFromPath(const char *fpath);
void img_getDimensions(img_t img, uint32_t *width, uint32_t *height);
void img_free(img_t img);

#endif
