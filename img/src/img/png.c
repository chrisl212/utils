#include <stdio.h>
#include "img.h"

uint8_t magic[8] = {0x89,'P','N','G',0x0D,0x0A,0x1A,0x0A};

typedef struct {
    uint32_t length;
    uint8_t type[4];
} __attribute__((packed)) pngChunkHeader_t;

typedef struct {
    uint8_t magic[8];
    pngChunkHeader_t IHDRHeader;
    uint32_t width;
    uint32_t height;
    uint8_t bitDepth;
    uint8_t colorType;
    uint8_t compressionMethod;
    uint8_t filterMethod;
    uint8_t interlaceMethod;
} __attribute__((packed)) pngHeader_t;

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

static imgHeader_t _getHeader(FILE *f) {
    pngHeader_t header = {0};
    imgHeader_t imgHeader = {0};

    fread(&header, sizeof(header), 1, f);
    if (_strcmp(header.magic, magic, 8) != 0) {
        imgHeader.valid = false;
        return imgHeader;
    } else {
        imgHeader.valid = true;
    }
    header.width = _convert(header.width);
    header.height = _convert(header.height);

    printf("Width: %u Height: %u\n", header.width, header.height);

    return imgHeader;
}

img_t png_loadFromPath(const char *fpath) {
    img_t img;
    FILE *f = fopen(fpath, "rb");
    
    img.type = IMG_PNG;
    img.header = _getHeader(f);

    return img;
}
