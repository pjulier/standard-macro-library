#include <string.h>
#include <stdint.h>

#include "SML/sml_algo.h"

static void  SML__quicksort_r(void *base, void *high, size_t width, int (*compare_fn)(const void *a, const void *b, void *ctx), void *ctx, uint32_t *pcg_seed);
static void* SML__partition(void *base, void *high, size_t width, int (*compare_fn)(const void *a, const void *b, void *ctx), void *ctx, uint32_t *pcg_seed);
static void  SML__swap_void(void *a, void *b, size_t width);

static void SML__quicksort_r(void *base, void *high, size_t width, int (*compare_fn)(const void *a, const void *b, void *ctx), void *ctx, uint32_t *pcg_seed)
{
    if (base >= high)
        return;

    /* partition */
    uint8_t *const pivot = SML__partition(base, high, width, compare_fn, ctx, pcg_seed);
    /* recursion left side */
    SML__quicksort_r(base, pivot - width, width, compare_fn, ctx, pcg_seed);
    /* recursion right side */
    SML__quicksort_r(pivot + width, high, width, compare_fn, ctx, pcg_seed);
}

static void* SML__partition(void *base, void *high, size_t width, int (*compare_fn)(const void *a, const void *b, void *ctx), void *ctx, uint32_t *pcg_seed)
{
    uint8_t *const _base = base;
    uint8_t *const _high = high;

    /* choose random pivot and place at the end */
    const uint32_t random = SML_pcg_hash(*pcg_seed);
    *pcg_seed = random;

    uint8_t *const _pivot = _base + width * (random % ((_high - _base) / width));
    SML__swap_void(_pivot, _high, width);

    uint8_t *_i = _base;

    for (uint8_t *_j = _base; _j < _high; _j += width) {
        if (compare_fn(_high, _j, ctx) > 0) {
            if (_i != _j) {
                SML__swap_void(_i, _j, width);
            }
            _i += width;
        }
    }
    if (_i != _high) {
        SML__swap_void(_i, _high, width);
    }
    return _i;
}

static void SML__swap_void(void *a, void *b, size_t bytes)
{
    uint8_t *_a = a;
    uint8_t *_b = b;
    enum { SML_SWAP_GENERIC_SIZE = 32 };
    uint8_t tmp[SML_SWAP_GENERIC_SIZE];
    while (bytes > SML_SWAP_GENERIC_SIZE)
    {
        memcpy(tmp, _a,  SML_SWAP_GENERIC_SIZE);
        memcpy(_a,  _b,  SML_SWAP_GENERIC_SIZE);
        memcpy(_b,  tmp, SML_SWAP_GENERIC_SIZE);
        _a += SML_SWAP_GENERIC_SIZE;
        _b += SML_SWAP_GENERIC_SIZE;
        bytes -= SML_SWAP_GENERIC_SIZE;
    }
    while (bytes > 0)
    {
        uint8_t t = _a[--bytes];
        _a[bytes] = _b[bytes];
        _b[bytes] = t;
    }
}

void SML_fill_void(void *dst, void *val, size_t count, size_t width)
{
    uint8_t *_dst = (uint8_t *)dst;

    while (count-- > 0)
    {
        enum { SML_FILL_GENERIC_SIZE = 32 };
        uint8_t *_src = (uint8_t *)val;
        size_t bytes = width;
        while (bytes > SML_FILL_GENERIC_SIZE)
        {
            memcpy(_dst, _src, SML_FILL_GENERIC_SIZE);
            _dst += SML_FILL_GENERIC_SIZE;
            _src += SML_FILL_GENERIC_SIZE;
            bytes -= SML_FILL_GENERIC_SIZE;
        }
        while (bytes > 0)
        {
            *_dst++ = *_src++;
            --bytes;
        }
    }
}

void SML_fill_float(float *dst, float val, size_t count)
{
    while (count-- > 0) {
        *dst++ = val;
    }
}

void SML_fill_int(int *dst, int val, size_t count)
{
    while (count-- > 0) {
        *dst++ = val;
    }
}

void SML_fill_uint(unsigned int *dst, unsigned int val, size_t count)
{
    while (count-- > 0) {
        *dst++ = val;
    }
}

void SML_quicksort_r(void *base, size_t count, size_t width, int (*compare_fn)(const void *a, const void *b, void *ctx), void *ctx)
{
    /* use some sort of randomization that does not mess with global seed */
    uint32_t pcg_seed = 999;
    void *const high = (uint8_t *)base + (count - 1) * width;
    SML__quicksort_r(base, high, width, compare_fn, ctx, &pcg_seed);
}