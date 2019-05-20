#ifndef PNG_CHUNK_H
#define PNG_CHUNK_H

typedef enum {
    
}

typedef struct {
    uint32_t length;
    uint8_t type[4];
} __attribute__((packed)) pngChunkHeader_t;

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
    uint8_t *pallete[3];
    uint8_t crc[4];
} __attribute__((packed)) pngChunkPLTE_t;

typedef struct {
    uint8_t *samples;
    uint8_t crc[4];
} __attribute__((packed)) pngChunkIDAT_t;

typedef struct {
    uint8_t crc[4];
} __attribute__((packed)) pngChunkIEND_t;

union

typedef struct {
    pngChunkHeader_t header;
    
}

#endif
