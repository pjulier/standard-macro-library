#ifndef INCLUDE_SML_ALGO_H
#define INCLUDE_SML_ALGO_H

#include <stddef.h>

void SML_fill_void(void *dst, void *val, size_t count, size_t width);
void SML_fill_float(float *dst, float val, size_t count);
void SML_fill_int(int *dst, int val, size_t count);
void SML_fill_uint(unsigned int *dst, unsigned int val, size_t count);

void SML_quicksort_r(void *base, size_t count, size_t width, int (*compare_fn)(const void *a, const void *b, void *ctx), void *ctx);

#endif /* INCLUDE_SML_ALGO_H */