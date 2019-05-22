//
//  bmp.h
//  utils
//
//  Created by Christopher Loonam on 5/21/19.
//  Copyright © 2019 Christopher Loonam. All rights reserved.
//

#ifndef BMP_H
#define BMP_H

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t reserved;
} __attribute__((packed)) bmpColor_t;

typedef struct {
    uint8_t signature[2];
    uint32_t fileSize;
    uint32_t reserved;
    uint32_t dataOffset;
    uint32_t headerSize;
    uint32_t width;
    uint32_t height;
    uint16_t planes;
    uint16_t bitsPerPixel;
    uint32_t compression;
    uint32_t imageSize;
    uint32_t pixelsPerMx;
    uint32_t pixelsPerMy;
    uint32_t numColors;
    uint32_t importantColors;
} __attribute__((packed)) bmpHeader_t;

typedef struct {
    bmpColor_t *colors;
    uint32_t len;
} bmpColorTable_t;

typedef struct {
    bool valid;
    bmpHeader_t header;
    bmpColorTable_t colorTable;
    char *pixelData;
} bmp_t;

bmp_t bmp_loadFromPath(const char *fpath);
void bmp_free(bmp_t bmp);

#endif
