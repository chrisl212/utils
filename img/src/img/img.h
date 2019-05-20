#ifndef IMG_H
#define IMG_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum {
    IMG_PNG,
    IMG_JPG,
    IMG_BMP
} imgType_t;

typedef struct {
    bool valid;
} imgHeader_t;

typedef struct {
    uint16_t r;
    uint16_t g;
    uint16_t b;
    uint8_t a;
} imgPixel_t;

typedef struct img_t {
    imgType_t type;
    imgHeader_t header;
    imgPixel_t *bitmap;
} img_t;

img_t img_loadFromPath(const char *fpath);
void img_free(img_t img);

#endif
