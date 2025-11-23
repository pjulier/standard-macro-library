#ifndef INCLUDE_SML_STRING_H
#define INCLUDE_SML_STRING_H

#include <stdint.h>
#include <stddef.h>

#include "sml_common.h"

#ifdef __cplusplus
extern "C" {
#endif

void  SML_uint64tobin(uint64_t value, char *str, int places);
void  SML_uint32tobin(uint32_t value, char *str, int places);
char* SML_strpcpy(char *dst, const char *src);

/**
 * @brief Safe string copy function
 * Copies a string from src to buf. Inspects no more than 
 * dstSize characters of src and always null terminates 
 * the dst buffer, except if dstSize == 0.
 * Nominally returns the number of written bytes without the
 * null character. Thus, the maximum return value for a 
 * non-truncated string copy is dstSize - 1, whereas
 * a return value of dstSize indicates a truncation.
 * 
 * @param dst destination buffer
 * @param src source buffer
 * @param dstSize size of destination buffer
 * @return size_t 
 */
size_t SML_strscpy(char *dst, const char *src, size_t dstSize);

char* SML_strdup(const char *src);
char* SML_strviewdup(const char *src, unsigned int size);
int   SML_strtoi(const char *str, const char **end);
char* SML_itoa(char *dst, unsigned int size, int val, int base);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_SML_STRING_H */