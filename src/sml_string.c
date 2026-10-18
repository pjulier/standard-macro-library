#include <string.h>
#include <stdlib.h>

#include "SML/sml_string.h"

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

char* SML_strpcpy(char *dst, const char *src)
{
  const size_t len = strlen(src);
  return (char *)memcpy(dst, src, len + 1) + len;
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

SMLReturn SML_charstoi64(const char *src, const char **end, unsigned int base, int64_t *value_out)
{
    const char *begin = src;
    uint64_t value = 0;
    int isNeg = 0;
    unsigned int state = 0;
    unsigned int c;
    uint64_t valuelim;
    unsigned int clim;
    SMLReturn ret;

    c = *src;

    /* check base input */
    if (base < 2 || base > 36) {
        return SML_RET_EINVAL;
    }
    /* skip leading whitespace */
    while (c == ' ' || c == '\t') {
        c = *++src;
    }
    /* skip possible minus sign */
    if (c == '-') {
        isNeg = 1;
        c = *++src;
    }

    /* determine the limits used for overflow detection */
    valuelim = (isNeg == 0) ? INT64_MAX : -(uint64_t)INT64_MIN;
    clim = valuelim % (uint64_t)base;
    valuelim /= (uint64_t)base;
    /* process chars */
    while (1) {
        if (c >= '0' && c <= '9') {
            c -= '0';
        } else if (c >= 'A' && c <= 'Z') {
            c = c - 'A' + 10;
        } else if (c >= 'a' && c <= 'z') {
            c = c - 'a' + 10;
        } else {
            /* invalid character for any base */
            break;
        }
        if (c >= base) {
            /* character too high for the current base */
            break;
        }
        /* detect overflow resulting from the current character */
        if (state == 2 || value > valuelim || (value == valuelim && c > clim)) {
            state = 2;
        } else {
            state = 1;
            value *= base;
            value += c;
        }
        c = *++src;
    }
    /* assign end pointer if it was passed */
    if (end) {
        *end = state == 0 ? begin : src;
    }
    /* assign the out value and return code */
    ret = SML_RET_OK;
    if (state == 0) {
        ret = SML_RET_EINVAL;
    } else if (state == 2) {
        ret = SML_RET_ERANGE;
        value = isNeg == 0 ? INT64_MAX : INT64_MIN;
    } else if (isNeg) {
        value = -value;
    }
    *value_out = (int64_t)value;
    return ret;
}

size_t SML_i64tochars(char *dst, unsigned int dstSize, int64_t val, int base)
{
    static const char digits[] = "ZYXWVUTSRQPONMLKJIHGFEDCBA9876543210123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    
    /* max length with val = INT64_MIN and base = 2 */
    char buffer[8 * sizeof(val) + 2]; /* +1 for sign, +1 for null character */

    /* sanity checks */
    if (base < 2 || base > 36 || dstSize == 0) {
        return 0;
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

    /* if str does not fit we only copy the front part and null terminate manually */
    if (l_size > dstSize) {
        memcpy(dst, p, dstSize - 1);
        dst[dstSize - 1] = '\0';
        return dstSize;
    }
    memcpy(dst, p, l_size);
    return l_size - 1;
}

char* SML_i64tocharsp(char *dst, unsigned int dstSize, int64_t val, int base)
{
    static const char digits[] = "ZYXWVUTSRQPONMLKJIHGFEDCBA9876543210123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
    
    /* max length with val = INT64_MIN and base = 2 */
    char buffer[8 * sizeof(val) + 2]; /* +1 for sign, +1 for null character */

    /* sanity checks */
    if (base < 2 || base > 36 || dstSize == 0) {
        return dst;
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

    /* if str does not fit we only copy the front part and null terminate manually */
    if (l_size > dstSize) {
        memcpy(dst, p, dstSize - 1);
        dst[dstSize - 1] = '\0';
        return dst + dstSize - 1;
    }

    memcpy(dst, p, l_size);
    return dst + l_size - 1;
}
