#ifndef PNG_H
#define PNG_H

typedef struct {
    pngHeader_t header;
    pngChunk_t *chunks;
} png_t;

typedef struct img_t img_t;

img_t png_loadFromPath(const char *fpath);

#endif
