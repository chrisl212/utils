#ifndef VEC_H
#define VEC_H

typedef mat_t vec_t;

vec_t vec_create(uint64_t len, ...);
void vec_free(vec_t vector);

#endif
