//
//  gifBlock.h
//  utils
//
//  Created by Christopher Loonam on 5/21/19.
//  Copyright © 2019 Christopher Loonam. All rights reserved.
//

#ifndef GIF_BLOCK_H
#define GIF_BLOCK_H

#include <stdint.h>

typedef struct {
    uint8_t signature[3];
    uint8_t version[3];
} __attribute__((packed)) gifBlockHeader_t;

typedef struct {
    uint16_t width;
    uint16_t height;
    uint8_t packed;
    uint8_t backgroundColorIndex;
    uint8_t pixelAspectRatio;
} __attribute__((packed)) gifBlockLogicalScreenDescriptor_t;

#endif
