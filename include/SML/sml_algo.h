#ifndef INCLUDE_SML_ALGO_H
#define INCLUDE_SML_ALGO_H

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

void SML_fill_void(void *dst, void *val, size_t count, size_t width);
void SML_fill_float(float *dst, float val, size_t count);
void SML_fill_int(int *dst, int val, size_t count);
void SML_fill_uint(unsigned int *dst, unsigned int val, size_t count);

void SML_quicksort_r(void *base, size_t count, size_t width, int (*compare_fn)(const void *a, const void *b, void *ctx), void *ctx);

static inline uint32_t SML_pcg_hash(uint32_t input)
{
    uint32_t state = input * 747796405u + 2891336453u;
    uint32_t word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
    return (word >> 22u) ^ word;
}

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_SML_ALGO_H */