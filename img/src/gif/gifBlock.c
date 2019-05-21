//
//  gifBlock.c
//  utils
//
//  Created by Christopher Loonam on 5/21/19.
//  Copyright © 2019 Christopher Loonam. All rights reserved.
//

#include <stdlib.h>
#include "gifBlock.h"

#define IMG_LCT 0x80
#define IMG_INT 0x40
#define IMG_SOR 0x20
#define IMG_LSZ 0x07

static uint8_t _pow(uint8_t num, uint8_t pow) {
	uint8_t res = 1;
	while (pow-- > 0)
		res *= num;
	return res;
}

gifBlockCT_t gifBlock_loadCT(FILE *f, uint8_t size) {
	gifBlockCT_t CT;

	size = _pow(2, size+1);
	CT.colorTable = malloc(sizeof(*CT.colorTable) * size);
	fread(CT.colorTable, sizeof(*CT.colorTable), size, f);
	CT.len = size;
	CT.present = true;

	return CT;
}

gifBlockData_t *gifBlock_loadData(FILE *f, uint16_t *len) {
	gifBlockData_t data, *dataBlocks = NULL;
	*len = 0;

	while (fgetc(f) != 0) {
		fseek(f, -1, SEEK_CUR);
		fread(&data.len, sizeof(data.len), 1, f);
		data.data = malloc(sizeof(*data.data) * data.len);
		fread(data.data, sizeof(*data.data), data.len, f);
		dataBlocks = realloc(dataBlocks, sizeof(*dataBlocks) * ++(*len));
		dataBlocks[*len-1] = data;
	}

	return dataBlocks;
}

gifBlockIMG_t gifBlock_loadIMG(FILE *f) {
	gifBlockIMG_t IMG = {0};

	fread(&IMG.desc, sizeof(IMG.desc), 1, f);
	if (IMG.desc.packed & IMG_LCT) {
		IMG.LCT = gifBlock_loadCT(f, IMG.desc.packed & IMG_LSZ);
	}
	IMG.dataBlocks = gifBlock_loadData(f, &IMG.len);

	return IMG;
}

gifBlockGCE_t gifBlock_loadGCE(FILE *f) {
	gifBlockGCE_t GCE;
	fread(&GCE, sizeof(GCE), 1, f);
	return GCE;
}

gifBlockCOM_t gifBlock_loadCOM(FILE *f) {
	gifBlockCOM_t COM;
	COM.dataBlocks = gifBlock_loadData(f, &COM.len);
	return COM;
}

gifBlockPLT_t gifBlock_loadPLT(FILE *f) {
	gifBlockPLT_t PLT;
	fread(&PLT.header, sizeof(PLT.header), 1, f);
	PLT.dataBlocks = gifBlock_loadData(f, &PLT.len);
	return PLT;
}

gifBlockAPL_t gifBlock_loadAPL(FILE *f) {
	gifBlockAPL_t APL;
	fread(&APL.header, sizeof(APL.header), 1, f);
	APL.dataBlocks = gifBlock_loadData(f, &APL.len);
	return APL;
}

void gifBlock_free(gifBlock_t block) {
	int i;
	switch (block.type) {
		case BLK_GCT:
			free(block.data.GCT.colorTable);
			break;
		case BLK_IMG:
			for (i = 0; i < block.data.IMG.len; i++)
				free(block.data.IMG.dataBlocks[i].data);
			if (block.data.IMG.LCT.colorTable)
				free(block.data.IMG.LCT.colorTable);
			free(block.data.IMG.dataBlocks);
			break;
		case BLK_COM:
			for (i = 0; i < block.data.COM.len; i++)
				free(block.data.COM.dataBlocks[i].data);
			free(block.data.COM.dataBlocks);
			break;
		case BLK_PLT:
			for (i = 0; i < block.data.PLT.len; i++)
				free(block.data.PLT.dataBlocks[i].data);
			free(block.data.PLT.dataBlocks);
			break;
		case BLK_APL:
			for (i = 0; i < block.data.APL.len; i++)
				free(block.data.APL.dataBlocks[i].data);
			free(block.data.APL.dataBlocks);
			break;
		default:
			break;
	}
}
