#include <stdio.h>
#include <stdlib.h>
#include "png.h"
#include "../img/img.h"

uint8_t magic[8] = {0x89,'P','N','G',0x0D,0x0A,0x1A,0x0A};

static uint32_t _convert(uint32_t num) {
    uint32_t ret = 0;
    ret |= (num & 0x000000FF) << 24;
    ret |= (num & 0x0000FF00) << 8;
    ret |= (num & 0x00FF0000) >> 8;
    ret |= (num & 0xFF000000) >> 24;
    return ret;
}

static int _strcmp(uint8_t *b1, uint8_t *b2, int len) {
    while (--len >= 0) {
        if (b1[len] - b2[len] != 0) {
            return b1[len] - b2[len];
        }
    }
    return 0;
}

static void _print(uint8_t *s, int len) {
    int i = 0;
    while (i < len) {
        putchar(s[i++]);
    }
}

static pngChunk_t _loadChunk(FILE *f, pngChunkIHDR_t IHDR) {
    pngChunk_t chunk = {0};
    
    fread(&chunk.header, sizeof(chunk.header), 1, f);
    chunk.header.length = _convert(chunk.header.length);
    _print(chunk.header.type, 4);
    putchar('\n');
    if (_strcmp(chunk.header.type, (uint8_t *)"IHDR", 4) == 0) {
        chunk.type = PNG_IHDR;
        chunk.data.IHDR = pngChunk_loadIHDR(f);
    } else if (_strcmp(chunk.header.type, (uint8_t *)"PLTE", 4) == 0) {
        chunk.type = PNG_PLTE;
        chunk.data.PLTE = pngChunk_loadPLTE(f, chunk.header.length);
    } else if (_strcmp(chunk.header.type, (uint8_t *)"IDAT", 4) == 0) {
        chunk.type = PNG_IDAT;
        chunk.data.IDAT = pngChunk_loadIDAT(f, chunk.header.length);
    } else if (_strcmp(chunk.header.type, (uint8_t *)"IEND", 4) == 0) {
        chunk.type = PNG_IEND;
        chunk.data.IEND = pngChunk_loadIEND(f);
    } else if (_strcmp(chunk.header.type, (uint8_t *)"tRNS", 4) == 0) {
        chunk.type = PNG_tRNS;
        chunk.data.tRNS = pngChunk_loadtRNS(f, chunk.header.length, IHDR.colorType);
    } else if (_strcmp(chunk.header.type, (uint8_t *)"gAMA", 4) == 0) {
        chunk.type = PNG_gAMA;
        chunk.data.gAMA = pngChunk_loadgAMA(f);
    } else if (_strcmp(chunk.header.type, (uint8_t *)"cHRM", 4) == 0) {
        chunk.type = PNG_cHRM;
        chunk.data.cHRM = pngChunk_loadcHRM(f);
    } else if (_strcmp(chunk.header.type, (uint8_t *)"sRGB", 4) == 0) {
        chunk.type = PNG_sRGB;
        chunk.data.sRGB = pngChunk_loadsRGB(f);
    } else if (_strcmp(chunk.header.type, (uint8_t *)"iCCP", 4) == 0) {
        chunk.type = PNG_iCCP;
        chunk.data.iCCP = pngChunk_loadiCCP(f, chunk.header.length);
    } else if (_strcmp(chunk.header.type, (uint8_t *)"tEXt", 4) == 0) {
        chunk.type = PNG_tEXt;
        chunk.data.tEXt = pngChunk_loadtEXt(f, chunk.header.length);
    } else if (_strcmp(chunk.header.type, (uint8_t *)"zTXt", 4) == 0) {
        chunk.type = PNG_zTXt;
        chunk.data.zTXt = pngChunk_loadzTXt(f, chunk.header.length);
    } else if (_strcmp(chunk.header.type, (uint8_t *)"iTXt", 4) == 0) {
        chunk.type = PNG_iTXt;
        chunk.data.iTXt = pngChunk_loadiTXt(f, chunk.header.length);
    } else if (_strcmp(chunk.header.type, (uint8_t *)"bKGD", 4) == 0) {
        chunk.type = PNG_bKGD;
        chunk.data.bKGD = pngChunk_loadbKGD(f, IHDR.colorType);
    } else if (_strcmp(chunk.header.type, (uint8_t *)"pHYs", 4) == 0) {
        chunk.type = PNG_pHYs;
        chunk.data.pHYs = pngChunk_loadpHYs(f);
    } else if (_strcmp(chunk.header.type, (uint8_t *)"sBIT", 4) == 0) {
        chunk.type = PNG_sBIT;
        chunk.data.sBIT = pngChunk_loadsBIT(f, IHDR.colorType);
    } else if (_strcmp(chunk.header.type, (uint8_t *)"hIST", 4) == 0) {
        chunk.type = PNG_hIST;
        chunk.data.hIST = pngChunk_loadhIST(f, chunk.header.length);
    } else if (_strcmp(chunk.header.type, (uint8_t *)"tIME", 4) == 0) {
        chunk.type = PNG_tIME;
        chunk.data.tIME = pngChunk_loadtIME(f);
    } else {
        fseek(f, chunk.header.length+4, SEEK_CUR);
    }
    return chunk;
}

static png_t _loadPng(FILE *f) {
    png_t png = {0};
    pngChunk_t *chunks = NULL;
    pngChunkIHDR_t IHDR = {0};
    uint8_t header[8];
    
    fread(&header, sizeof(header), 1, f);
    if (_strcmp(header, magic, 8) != 0) {
        return png;
    } else {
        png.valid = true;
    }
    
    while (!feof(f)) {
        chunks = realloc(chunks, sizeof(*chunks) * ++png.chunksLen);
        chunks[png.chunksLen-1] = _loadChunk(f, IHDR);
        if (chunks[png.chunksLen-1].type == PNG_IHDR) {
            IHDR = chunks[png.chunksLen-1].data.IHDR;
        }
        if (chunks[png.chunksLen-1].type == PNG_IEND) {
            break;
        }
    }

    png.chunks = chunks;
    return png;
}

void _pngFree(png_t png) {
    int i;
    for (i = 0; i < png.chunksLen; i++) {
        pngChunk_free(png.chunks[i]);
    }
    free(png.chunks);
}

img_t png_loadFromPath(const char *fpath) {
    img_t img;
    png_t png;
    
    FILE *f = fopen(fpath, "rb");
    png = _loadPng(f);
    
    img.type = IMG_PNG;

    fclose(f);
    _pngFree(png);
    return img;
}
