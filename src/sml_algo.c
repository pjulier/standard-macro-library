#include <string.h>
#include <stdint.h>

#include "SML/sml_algo.h"

void SML_fill_void(void *dst, void *val, size_t count, size_t width)
{
    uint8_t *_dst = (uint8_t *)dst;

    while (count-- > 0)
    {
        enum { FILL_GENERIC_SIZE = 32 };
        uint8_t *_src = (uint8_t *)val;
        size_t bytes = width;
        while (bytes > FILL_GENERIC_SIZE)
        {
            memcpy(_dst, _src, FILL_GENERIC_SIZE);
            _dst += FILL_GENERIC_SIZE;
            _src += FILL_GENERIC_SIZE;
            bytes -= FILL_GENERIC_SIZE;
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
