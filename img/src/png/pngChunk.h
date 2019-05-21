#ifndef PNG_CHUNK_H
#define PNG_CHUNK_H

#include <stdint.h>

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} __attribute__((packed)) pngChunkPixel_t;

//TODO: sPLT

typedef enum {
    PNG_INVALID,
    PNG_IHDR,
    PNG_PLTE,
    PNG_IDAT,
    PNG_IEND,
    PNG_tRNS,
    PNG_gAMA,
    PNG_cHRM,
    PNG_sRGB,
    PNG_iCCP,
    PNG_tEXt,
    PNG_zTXt,
    PNG_iTXt,
    PNG_bKGD,
    PNG_pHYs,
    PNG_sBIT,
    PNG_hIST,
    PNG_tIME
} pngChunkType_t;

typedef struct {
    uint32_t length;
    uint8_t type[4];
} __attribute__((packed)) pngChunkHeader_t;

//Critical chunks

typedef struct {
    uint32_t width;
    uint32_t height;
    uint8_t bitDepth;
    uint8_t colorType;
    uint8_t compressionMethod;
    uint8_t filterMethod;
    uint8_t interlaceMethod;
    uint8_t crc[4];
} __attribute__((packed)) pngChunkIHDR_t;

typedef struct {
    pngChunkPixel_t *pallete;
    uint8_t crc[4];
} __attribute__((packed)) pngChunkPLTE_t;

typedef struct {
    uint8_t *samples;
    uint8_t crc[4];
} __attribute__((packed)) pngChunkIDAT_t;

typedef struct {
    uint8_t crc[4];
} __attribute__((packed)) pngChunkIEND_t;

//Ancillary chunks

typedef struct {
    uint8_t *indexedAlpha;
    uint16_t grayscaleAlpha;
    uint16_t truecolorAlpha[3];
    uint8_t crc[4];
} __attribute__((packed)) pngChunktRNS_t;

typedef struct {
    uint32_t gamma;
    uint8_t crc[4];
} __attribute__((packed)) pngChunkgAMA_t;

typedef struct {
    uint32_t whitePointX;
    uint32_t whitePointY;
    uint32_t redX;
    uint32_t redY;
    uint32_t greenX;
    uint32_t greenY;
    uint32_t blueX;
    uint32_t blueY;
    uint8_t crc[4];
} __attribute__((packed)) pngChunkcHRM_t;

typedef struct {
    uint8_t renderingIntent;
    uint8_t crc[4];
} __attribute__((packed)) pngChunksRGB_t;

typedef struct {
    char *name;
    uint8_t compressionMethod;
    char *profile;
    uint8_t crc[4];
} __attribute__((packed)) pngChunkiCCP_t;

typedef struct {
    char *keyword;
    char *text;
    uint8_t crc[4];
} __attribute__((packed)) pngChunktEXt_t;

typedef struct {
    char *keyword;
    uint8_t compressionMethod;
    char *text;
    uint8_t crc[4];
} __attribute__((packed)) pngChunkzTXt_t;

typedef struct {
    char *keyword;
    uint8_t compressionFlag;
    uint8_t compressionMethod;
    char *languageTag;
    char *translatedKeyword;
    char *text;
    uint8_t crc[4];
} __attribute__((packed)) pngChunkiTXt_t;

typedef struct {
    uint8_t indexedBackground;
    uint16_t grayscaleBackground;
    uint16_t truecolorBackground[3];
    uint8_t crc[4];
} __attribute__((packed)) pngChunkbKGD_t;

typedef struct {
    uint32_t x;
    uint32_t y;
    uint8_t unit;
    uint8_t crc[4];
} __attribute__((packed)) pngChunkpHYs_t;

typedef struct {
    uint8_t grayscale;
    uint8_t truecolor[3];
    uint8_t indexed[3];
    uint8_t grayscalea[2];
    uint8_t truecolora[4];
    uint8_t crc[4];
} __attribute__((packed)) pngChunksBIT_t;

typedef struct {
    uint16_t *histogram;
    uint32_t len;
    uint8_t crc[4];
} __attribute__((packed)) pngChunkhIST_t;

typedef struct {
    uint16_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minute;
    uint8_t second;
    uint8_t crc[4];
} __attribute__((packed)) pngChunktIME_t;

//Chunk wrappers

typedef union {
    pngChunkIHDR_t IHDR;
    pngChunkPLTE_t PLTE;
    pngChunkIDAT_t IDAT;
    pngChunkIEND_t IEND;
    pngChunktRNS_t tRNS;
    pngChunkgAMA_t gAMA;
    pngChunkcHRM_t cHRM;
    pngChunksRGB_t sRGB;
    pngChunkiCCP_t iCCP;
    pngChunktEXt_t tEXt;
    pngChunkzTXt_t zTXt;
    pngChunkiTXt_t iTXt;
    pngChunkbKGD_t bKGD;
    pngChunkpHYs_t pHYs;
    pngChunksBIT_t sBIT;
    pngChunkhIST_t hIST;
    pngChunktIME_t tIME;
} pngChunkData_t;

typedef struct {
    pngChunkHeader_t header;
    pngChunkType_t type;
    pngChunkData_t data;
} pngChunk_t;

//Critical chunks
pngChunkIHDR_t pngChunk_loadIHDR(FILE *f);
pngChunkPLTE_t pngChunk_loadPLTE(FILE *f, uint32_t len);
pngChunkIDAT_t pngChunk_loadIDAT(FILE *f, uint32_t len);
pngChunkIEND_t pngChunk_loadIEND(FILE *f);

//Ancillary chunks
pngChunktRNS_t pngChunk_loadtRNS(FILE *f, uint32_t len, uint8_t colorMode);
pngChunkgAMA_t pngChunk_loadgAMA(FILE *f);
pngChunkcHRM_t pngChunk_loadcHRM(FILE *f);
pngChunksRGB_t pngChunk_loadsRGB(FILE *f);
pngChunkiCCP_t pngChunk_loadiCCP(FILE *f, uint32_t len);
pngChunktEXt_t pngChunk_loadtEXt(FILE *f, uint32_t len);
pngChunkzTXt_t pngChunk_loadzTXt(FILE *f, uint32_t len);
pngChunkiTXt_t pngChunk_loadiTXt(FILE *f, uint32_t len);
pngChunkbKGD_t pngChunk_loadbKGD(FILE *f, uint8_t colorMode);
pngChunkpHYs_t pngChunk_loadpHYs(FILE *f);
pngChunksBIT_t pngChunk_loadsBIT(FILE *f, uint8_t colorMode);
pngChunkhIST_t pngChunk_loadhIST(FILE *f, uint32_t len);
pngChunktIME_t pngChunk_loadtIME(FILE *f);

void pngChunk_free(pngChunk_t chunk);

#endif
