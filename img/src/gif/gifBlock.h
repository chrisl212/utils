//
//  gifBlock.h
//  utils
//
//  Created by Christopher Loonam on 5/21/19.
//  Copyright © 2019 Christopher Loonam. All rights reserved.
//

#ifndef GIF_BLOCK_H
#define GIF_BLOCK_H

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

typedef enum {
	BLK_INVALID,
	BLK_HDR,
	BLK_LSD,
	BLK_GCT,
	BLK_IMD,
	BLK_IMG,
	BLK_GCE,
	BLK_COM,
	BLK_PLT,
	BLK_APL
} gifBlockType_t;

typedef struct {
	uint8_t r;
	uint8_t g;
	uint8_t b;
} __attribute__((packed)) gifBlockColor_t;

typedef struct {
	uint8_t len;
	char *data;
} gifBlockData_t;

//Required blocks

typedef struct {
    uint8_t signature[3];
    uint8_t version[3];
} __attribute__((packed)) gifBlockHDR_t;

typedef struct {
    uint16_t width;
    uint16_t height;
    uint8_t packed;
    uint8_t backgroundColorIndex;
    uint8_t pixelAspectRatio;
} __attribute__((packed)) gifBlockLSD_t;

typedef struct {
	bool present;
	gifBlockColor_t *colorTable;
	uint8_t len;
} gifBlockCT_t;

typedef struct {
	uint16_t leftPosition;
	uint16_t topPosition;
	uint16_t width;
	uint16_t height;
	uint8_t packed;
} __attribute__((packed)) gifBlockIMD_t;

typedef struct {
	gifBlockIMD_t desc;
	gifBlockCT_t LCT;
	gifBlockData_t *dataBlocks;
	uint16_t len;
} gifBlockIMG_t;

//Extension blocks

typedef struct {
	uint8_t size;
	uint8_t packed;
	uint16_t delayTime;
	uint8_t transparentIndex;
	uint8_t term;
} __attribute__((packed)) gifBlockGCE_t;

typedef struct {
	gifBlockData_t *dataBlocks;
	uint16_t len;
} gifBlockCOM_t;

typedef struct {
	uint8_t size;
	uint16_t leftPosition;
	uint16_t topPosition;
	uint16_t width;
	uint16_t height;
	uint8_t cellWidth;
	uint8_t cellHeight;
	uint8_t foregroundIndex;
	uint8_t backgroundIndex;
} __attribute__((packed)) gifBlockPLTHeader_t;

typedef struct {
	gifBlockPLTHeader_t header;
	gifBlockData_t *dataBlocks;
	uint16_t len;
} gifBlockPLT_t;

typedef struct {
	uint8_t size;
	uint8_t identifier[8];
	uint8_t code[3];
} __attribute__((packed)) gifBlockAPLHeader_t;

typedef struct {
	gifBlockAPLHeader_t header;
	gifBlockData_t *dataBlocks;
	uint16_t len;
} gifBlockAPL_t;

//Block wrappers

typedef union {
	gifBlockHDR_t HDR;
	gifBlockLSD_t LSD;
	gifBlockCT_t GCT;
	gifBlockIMD_t IMD;
	gifBlockIMG_t IMG;
	gifBlockGCE_t GCE;
	gifBlockCOM_t COM;
	gifBlockPLT_t PLT;
	gifBlockAPL_t APL;
} gifBlockWrapper_t;

typedef struct {
	gifBlockType_t type;
	gifBlockWrapper_t data;
} gifBlock_t;

gifBlockCT_t gifBlock_loadCT(FILE *f, uint8_t size);
gifBlockData_t *gifBlock_loadData(FILE *f, uint16_t *len);

gifBlockIMG_t gifBlock_loadIMG(FILE *f);

gifBlockGCE_t gifBlock_loadGCE(FILE *f);
gifBlockCOM_t gifBlock_loadCOM(FILE *f);
gifBlockPLT_t gifBlock_loadPLT(FILE *f);
gifBlockAPL_t gifBlock_loadAPL(FILE *f);

void gifBlock_free(gifBlock_t block);

#endif
