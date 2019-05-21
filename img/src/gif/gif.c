//
//  gif.c
//  utils
//
//  Created by Christopher Loonam on 5/21/19.
//  Copyright © 2019 Christopher Loonam. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "gif.h"

#define GIF_GCT 0x80
#define GIF_RES 0x70
#define GIF_SORT 0x08
#define GIF_GSZ 0x07
#define GIF_IMG 0x2C
#define GIF_EXT 0x21
#define EXT_GCE 0xF9
#define EXT_COM 0xFE
#define EXT_PLT 0x01
#define EXT_APL 0xFF

static int _strcmp(uint8_t *b1, uint8_t *b2, int len) {
    while (--len >= 0) {
        if (b1[len] - b2[len] != 0) {
            return b1[len] - b2[len];
        }
    }
    return 0;
}

static void _appendBlock(gif_t *gif, gifBlock_t block) {
	gif->blocks = realloc(gif->blocks, sizeof(*gif->blocks) * ++gif->len);
	gif->blocks[gif->len-1] = block;
}

gif_t gif_loadFromPath(const char *fpath) {
    gif_t gif = {0};
    gifBlock_t blk;
    FILE *f = fopen(fpath, "r");
    uint8_t intro, ext;
    
    fread(&blk.data.HDR, sizeof(blk.data.HDR), 1, f);
    if (_strcmp(blk.data.HDR.signature, (uint8_t *)"GIF", 3) != 0 || (_strcmp(blk.data.HDR.version, (uint8_t *)"87a", 3) != 0 && _strcmp(blk.data.HDR.version, (uint8_t *)"89a", 3) !=0)) {
        return gif;
    } else {
        gif.valid = true;
        blk.type = BLK_HDR;
        _appendBlock(&gif, blk);
    }
    fread(&blk.data.LSD, sizeof(blk.data.LSD), 1, f);
    blk.type = BLK_LSD;
    _appendBlock(&gif, blk);

    if (blk.data.LSD.packed & GIF_GCT) {
    	blk.data.GCT = gifBlock_loadCT(f, blk.data.LSD.packed & GIF_GSZ);
    	blk.type = BLK_GCT;
    	_appendBlock(&gif, blk);
    }

    while ((intro = fgetc(f)) != 0x3B) {
    	if (intro == GIF_IMG) {
    		printf("IMG\n");
    		blk.data.IMG = gifBlock_loadIMG(f);
    		blk.type = BLK_IMG;
    		_appendBlock(&gif, blk);
    	} else if (intro == GIF_EXT) {
    		ext = fgetc(f);
    		if (ext == EXT_GCE) {
    			printf("GCE\n");
    			blk.data.GCE = gifBlock_loadGCE(f);
    			blk.type = BLK_GCE;
    			_appendBlock(&gif, blk);
    		} else if (ext == EXT_COM) {
    			printf("COM\n");
    			blk.data.COM = gifBlock_loadCOM(f);
    			blk.type = BLK_COM;
    			_appendBlock(&gif, blk);
    		} else if (ext == EXT_PLT) {
    			printf("PTL\n");
    			blk.data.PLT = gifBlock_loadPLT(f);
    			blk.type = BLK_PLT;
    			_appendBlock(&gif, blk);
    		} else if (ext == EXT_APL) {
    			printf("APL\n");
    			blk.data.APL = gifBlock_loadAPL(f);
    			blk.type = BLK_APL;
    			_appendBlock(&gif, blk);
    		}
    	}
    }
    
    fclose(f);
    return gif;
}

gifBlock_t gif_getBlock(gif_t gif, gifBlockType_t type) {
	gifBlock_t blk = {0};
	int i;

	for (i = 0; i < gif.len; i++) {
		if (gif.blocks[i].type == type) {
			blk = gif.blocks[i];
			break;
		}
	}
	return blk;
}

void gif_free(gif_t gif) {
	int i;

	for (i = 0; i < gif.len; i++) {
		gifBlock_free(gif.blocks[i]);
	}
	free(gif.blocks);
}
