#include <string.h>
#include <stdlib.h>

#include "SML/sml_string.h"

void SML_uint64tobin(uint64_t value, char *str, int places)
{
    if (places > 64) {
        places = 64;
    }
    while (places--) {
		*str++ = value & ((uint64_t)1 << places) ? '1' : '0';
    }
    *str = '\0';
}

void SML_uint32tobin(uint32_t value, char *str, int places)
{
    if (places > 32) {
        places = 32;
    }
    while (places--) {
		*str++ = value & ((uint32_t)1 << places) ? '1' : '0';
    }
    *str = '\0';
}

char* SML_strpcpy(char *dst, const char *src)
{
  const size_t len = strlen(src);
  return (char *)memcpy(dst, src, len + 1) + len;
}

size_t SML_strscpy(char *dst, const char *src, size_t dstSize)
{
    if (dstSize == 0) {
        return 0;
    }
    --dstSize;
    size_t i = 0;
    for ( ; i < dstSize; ++i) {
        if (*src == '\0')
            break;
        *dst++ = *src++;
    }
    *dst = '\0';
    return i + (i == dstSize);
}

char* SML_strdup(const char *src)
{
    const size_t len = strlen(src);
    char *dst = malloc((len + 1) * sizeof(*dst));
    memcpy(dst, src, len * sizeof(*dst));
    dst[len] = '\0';
    return dst;
}

char* SML_strviewdup(const char *src, unsigned int size)
{
    char *dst = malloc((size + 1) * sizeof(*dst));
    memcpy(dst, src, size * sizeof(*dst));
    dst[size] = '\0';
    return dst;
}

int SML_strtoi(const char *str, const char **end)
{
    const char *begin = str;
    int result = 0;
    unsigned int numDigits;
    const char *l_end;
    int sign;

    /* discard leading whitespace */
    while (*str == ' ' || *str == '\t') {++str;}

    /* detect possible sign */
    if (*str == '-') {
        sign = -1;
        ++str;
    } else {
        sign = 1;
    }

    /* set the local end ptr to the first digit */
    l_end = str;

    /* count number of digits */
    while (*l_end > 0x2F && *l_end < 0x3A) {
        ++l_end;
    }

    numDigits = l_end - str;

    /* not a valid number? set end to original beginning and return zero */
    if (numDigits == 0) {
        *end = begin;
        return 0;
    }

    /* set the output ptr to the first character after the number */
    *end = l_end;

    /* consume trailing decimal places */
    if (**end == '.') {
        ++*end;
        while (**end > 0x2F && **end < 0x3A) {
            ++*end;
        }
    }

    --l_end;
    int mult = 1;

    while (--numDigits) { // TODO: this can be done without numDigits
        result += mult * (*l_end - '0');
        --l_end;
        mult *= 10;
    };
    result += mult * (*l_end - '0');

    return sign * result;
}

char* SML_itoa(char *dst, unsigned int size, int val, int base)
{
    static const char digits[] = "ZYXWVUTSRQPONMLKJIHGFEDCBA9876543210123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    
    /* max length with val = INT_MIN and base = 2 */
    char buffer[8 * sizeof(val) + 2]; /* +1 for sign, +1 for null character */

    if (base < 2 || base > 36) {
        return NULL;
    }

    /* start from the end */
    char* p = &buffer[sizeof(buffer) - 1];
    *p = '\0';

    const int isNeg = val < 0;

    do {
        *(--p) = digits[35 + (val % base)];
        val /= base;
    } while (val);

    if (isNeg) {
        *(--p) = '-';
    }

    const unsigned int l_size = &buffer[sizeof(buffer)] - p;

    /* if str does not fit we only copy the end part */
    if (l_size > size) {
       /* no memmove necessary since we copy to the left */
        return memcpy(dst, &buffer[sizeof(buffer) - size], size);
    }
    /* no memmove necessary since we copy to the left */
    return memcpy(dst, p, l_size);
}