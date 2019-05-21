//
//  pngChunk.c
//  utils
//
//  Created by Christopher Loonam on 5/20/19.
//  Copyright © 2019 Christopher Loonam. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zlib.h>
#include "pngChunk.h"

static char *_decompress(char *s) {
    size_t len = 20 * strlen(s);
    char *buf = malloc(sizeof(*buf) * len);
    
    uncompress((Bytef *)buf, &len, (Bytef *)s, strlen(s));
    buf = realloc(buf, sizeof(*buf) * (len+1));
    buf[len] = '\0';
    return buf;
}

static uint32_t _convert32(uint32_t num) {
    uint32_t ret = 0;
    ret |= (num & 0x000000FF) << 24;
    ret |= (num & 0x0000FF00) << 8;
    ret |= (num & 0x00FF0000) >> 8;
    ret |= (num & 0xFF000000) >> 24;
    return ret;
}

static uint16_t _convert16(uint16_t num) {
    uint16_t ret = 0;
    ret |= (num & 0x00FF) << 8;
    ret |= (num & 0xFF00) >> 8;
    return ret;
}

pngChunkIHDR_t pngChunk_loadIHDR(FILE *f) {
    pngChunkIHDR_t IHDR;
    fread(&IHDR, sizeof(IHDR), 1, f);
    IHDR.width = _convert32(IHDR.width);
    IHDR.height = _convert32(IHDR.height);
    return IHDR;
}

pngChunkPLTE_t pngChunk_loadPLTE(FILE *f, uint32_t len) {
    pngChunkPLTE_t PLTE;
    PLTE.pallete = malloc(sizeof(*PLTE.pallete) * len/3);
    fread(PLTE.pallete, sizeof(*PLTE.pallete), len/3, f);
    fread(&PLTE.crc, sizeof(PLTE.crc), 1, f);
    return PLTE;
}

pngChunkIDAT_t pngChunk_loadIDAT(FILE *f, uint32_t len) {
    pngChunkIDAT_t IDAT;
    IDAT.samples = malloc(sizeof(*IDAT.samples) * len);
    fread(IDAT.samples, sizeof(*IDAT.samples), len, f);
    fread(&IDAT.crc, sizeof(IDAT.crc), 1, f);
    return IDAT;
}

pngChunkIEND_t pngChunk_loadIEND(FILE *f) {
    pngChunkIEND_t IEND;
    fread(&IEND.crc, sizeof(IEND.crc), 1, f);
    return IEND;
}

pngChunktRNS_t pngChunk_loadtRNS(FILE *f, uint32_t len, uint8_t colorMode) {
    pngChunktRNS_t tRNS = {0};
    int i;
    
    if (colorMode == 0) {
        //grayscale
        fread(&tRNS.grayscaleAlpha, sizeof(tRNS.grayscaleAlpha), 1, f);
        tRNS.grayscaleAlpha = _convert16(tRNS.grayscaleAlpha);
    } else if (colorMode == 2) {
        //truecolor
        fread(&tRNS.truecolorAlpha, sizeof(tRNS.truecolorAlpha), 1, f);
        for (i = 0; i < 3; i++) {
            tRNS.truecolorAlpha[i] = _convert16(tRNS.truecolorAlpha[i]);
        }
    } else if (colorMode == 3) {
        tRNS.indexedAlpha = malloc(sizeof(*tRNS.indexedAlpha) * len);
        fread(tRNS.indexedAlpha, sizeof(*tRNS.indexedAlpha), len, f);
    }
    fread(&tRNS.crc, sizeof(tRNS.crc), 1, f);
    return tRNS;
}

pngChunkgAMA_t pngChunk_loadgAMA(FILE *f) {
    pngChunkgAMA_t gAMA;
    fread(&gAMA, sizeof(gAMA), 1, f);
    gAMA.gamma = _convert32(gAMA.gamma);
    return gAMA;
}

pngChunkcHRM_t pngChunk_loadcHRM(FILE *f) {
    pngChunkcHRM_t cHRM;
    fread(&cHRM, sizeof(cHRM), 1, f);
    cHRM.whitePointX = _convert32(cHRM.whitePointX);
    cHRM.whitePointY = _convert32(cHRM.whitePointY);
    cHRM.redX = _convert32(cHRM.redX);
    cHRM.redY = _convert32(cHRM.redY);
    cHRM.greenX = _convert32(cHRM.greenX);
    cHRM.greenY = _convert32(cHRM.greenY);
    cHRM.blueX = _convert32(cHRM.blueX);
    cHRM.blueY = _convert32(cHRM.blueY);
    return cHRM;
}

pngChunksRGB_t pngChunk_loadsRGB(FILE *f) {
    pngChunksRGB_t sRGB;
    fread(&sRGB, sizeof(sRGB), 1, f);
    return sRGB;
}

pngChunkiCCP_t pngChunk_loadiCCP(FILE *f, uint32_t len) {
    pngChunkiCCP_t iCCP;
    char *buf = malloc((len + 1) * sizeof(*buf));
    
    fread(buf, sizeof(*buf), len, f);
    buf[len] = '\0';
    iCCP.name = buf;
    iCCP.compressionMethod = buf[strlen(buf)+1];
    iCCP.profile = &buf[strlen(buf)+2];
    fread(&iCCP.crc, sizeof(iCCP.crc), 1, f);
    iCCP.profile = _decompress(iCCP.profile);
    return iCCP;
}

pngChunktEXt_t pngChunk_loadtEXt(FILE *f, uint32_t len) {
    pngChunktEXt_t tEXt;
    char *buf = malloc((len + 1) * sizeof(*buf));
    
    fread(buf, sizeof(*buf), len, f);
    buf[len] = '\0';
    tEXt.keyword = buf;
    tEXt.text = &buf[strlen(buf)+1];
    fread(&tEXt.crc, sizeof(tEXt.crc), 1, f);
    return tEXt;
}

pngChunkzTXt_t pngChunk_loadzTXt(FILE *f, uint32_t len) {
    pngChunkzTXt_t zTXt;
    char *buf = malloc((len + 1) * sizeof(*buf));
    
    fread(buf, sizeof(*buf), len, f);
    buf[len] = '\0';
    zTXt.keyword = buf;
    zTXt.compressionMethod = buf[strlen(buf)+1];
    zTXt.text = &buf[strlen(buf)+2];
    fread(&zTXt.crc, sizeof(zTXt.crc), 1, f);
    zTXt.text = _decompress(zTXt.text);
    return zTXt;
}

pngChunkiTXt_t pngChunk_loadiTXt(FILE *f, uint32_t len) {
    pngChunkiTXt_t iTXt;
    char *buf = malloc((len + 1) * sizeof(*buf));
    
    fread(buf, sizeof(*buf), len, f);
    buf[len] = '\0';
    iTXt.keyword = buf;
    iTXt.compressionFlag = buf[strlen(buf)+1];
    iTXt.compressionMethod = buf[strlen(buf)+2];
    iTXt.languageTag = &buf[strlen(buf)+3];
    iTXt.translatedKeyword = &buf[strlen(buf)+strlen(iTXt.languageTag)+4];
    iTXt.text = &buf[strlen(buf)+strlen(iTXt.languageTag)+strlen(iTXt.translatedKeyword)+5];
    if (iTXt.compressionFlag) {
        iTXt.text = _decompress(iTXt.text);
    }
    fread(&iTXt.crc, sizeof(iTXt.crc), 1, f);
    return iTXt;
}

pngChunkbKGD_t pngChunk_loadbKGD(FILE *f, uint8_t colorMode) {
    pngChunkbKGD_t bKGD = {0};
    int i;
    
    if (colorMode == 0 || colorMode == 4) {
        fread(&bKGD.grayscaleBackground, sizeof(bKGD.grayscaleBackground), 1, f);
        bKGD.grayscaleBackground = _convert16(bKGD.grayscaleBackground);
    } else if (colorMode == 2 || colorMode == 6) {
        fread(&bKGD.truecolorBackground, sizeof(bKGD.truecolorBackground), 1, f);
        for (i = 0; i < 3; i++) {
            bKGD.truecolorBackground[i] = _convert16(bKGD.truecolorBackground[i]);
        }
    } else if (colorMode == 3) {
        fread(&bKGD.indexedBackground, sizeof(bKGD.indexedBackground), 1, f);
    }
    fread(&bKGD.crc, sizeof(bKGD.crc), 1, f);
    return bKGD;
}

pngChunkpHYs_t pngChunk_loadpHYs(FILE *f) {
    pngChunkpHYs_t pHYs;
    fread(&pHYs, sizeof(pHYs), 1, f);
    pHYs.x = _convert32(pHYs.x);
    pHYs.y = _convert32(pHYs.y);
    return pHYs;
}

pngChunksBIT_t pngChunk_loadsBIT(FILE *f, uint8_t colorMode) {
    pngChunksBIT_t sBIT = {0};
    
    if (colorMode == 0) {
        fread(&sBIT.grayscale, sizeof(sBIT.grayscale), 1, f);
    } else if (colorMode == 2) {
        fread(&sBIT.truecolor, sizeof(sBIT.truecolor), 1, f);
    } else if (colorMode == 3) {
        fread(&sBIT.indexed, sizeof(sBIT.indexed), 1, f);
    } else if (colorMode == 4) {
        fread(&sBIT.grayscalea, sizeof(sBIT.grayscalea), 1, f);
    } else if (colorMode == 6) {
        fread(&sBIT.truecolora, sizeof(sBIT.truecolora), 1, f);
    }
    fread(&sBIT.crc, sizeof(sBIT.crc), 1, f);
    return sBIT;
}

pngChunkhIST_t pngChunk_loadhIST(FILE *f, uint32_t len) {
    pngChunkhIST_t hIST;
    int i;
    
    hIST.len = len/2;
    hIST.histogram = malloc(sizeof(*hIST.histogram) * hIST.len);
    fread(hIST.histogram, sizeof(*hIST.histogram), hIST.len, f);
    for (i = 0; i < hIST.len; i++) {
        hIST.histogram[i] = _convert16(hIST.histogram[i]);
    }
    
    fread(&hIST.crc, sizeof(hIST.crc), 1, f);
    return hIST;
}

pngChunktIME_t pngChunk_loadtIME(FILE *f) {
    pngChunktIME_t tIME;
    fread(&tIME, sizeof(tIME), 1, f);
    tIME.year = _convert16(tIME.year);
    return tIME;
}

void pngChunk_free(pngChunk_t chunk) {
    switch (chunk.type) {
        case PNG_PLTE:
            free(chunk.data.PLTE.pallete);
            break;
            
        case PNG_IDAT:
            free(chunk.data.IDAT.samples);
            break;
            
        case PNG_tRNS:
            if (chunk.data.tRNS.indexedAlpha)
                free(chunk.data.tRNS.indexedAlpha);
            break;
            
        case PNG_iCCP:
            free(chunk.data.iCCP.name);
            free(chunk.data.iCCP.profile);
            break;
            
        case PNG_tEXt:
            free(chunk.data.tEXt.keyword);
            break;
            
        case PNG_zTXt:
            free(chunk.data.zTXt.keyword);
            free(chunk.data.zTXt.text);
            break;
            
        case PNG_iTXt:
            free(chunk.data.iTXt.keyword);
            if (chunk.data.iTXt.compressionFlag)
                free(chunk.data.iTXt.text);
            break;
            
        case PNG_hIST:
            free(chunk.data.hIST.histogram);
            break;
            
        default:
            break;
    }
}
