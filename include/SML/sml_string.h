#ifndef INCLUDE_SML_STRING_H
#define INCLUDE_SML_STRING_H

#include <stdint.h>
#include <stddef.h>

#include "sml_common.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Safe string copy function.
 *
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
 * @return size_t number of characters written without null termination or dstSize on truncation
 */
size_t SML_strscpy(char *dst, const char *src, size_t dstSize);

/**
 * @brief Copy string from source to destination.
 * 
 * \caution Does not check for overflow.
 * 
 * @param dst destination
 * @param src source
 * @return char* pointer to destination
 */
char* SML_strpcpy(char *dst, const char *src);

/**
 * @brief Duplicate a string.
 * 
  * Allocates a new string by calling malloc and null-terminates
 * the copied string.
 * 
 * @param src string to duplicate
 * @return char* duplicated string
 */
char* SML_strdup(const char *src);

/**
 * @brief Duplicate a string from a string view.
 * 
 * Allocates a new string by calling malloc and null-terminates
 * the copied string.
 * 
 * @param src pointer to character span
 * @param size length of character span
 * @return char* duplicated string
 */
char* SML_strviewdup(const char *src, unsigned int size);

/**
 * @brief Convert characters to a 64 bit integer.
 * 
 * @param src pointer to character beginning
 * @param[out] end address of pointer to be set to the character end or NULL
 * @param base integer base representation
 * @param[out] value_out converted result
 * @return SMLReturn return code
 */
SMLReturn SML_charstoi64(const char *src, const char **end, unsigned int base, int64_t *value_out);

/**
 * @brief Convert a 64 bit integer to characters.
 * 
 * Always null terminates the destination buffer, except if dstSize == 0.
 * Nominally returns the number of written characters without NULL, except for
 * when truncation occurrs, in which case dstSize is returned.
 * 
 * @param dst destination buffer
 * @param dstSize size of destination buffer
 * @param val value to convert
 * @param base integer base representation
 * @return size_t number of characters written without null termination or dstSize on overflow
 */
size_t SML_i64tochars(char *dst, unsigned int dstSize, int64_t val, int base);

/**
 * @brief Convert a 64 bit integer to characters.
 * 
 * Always null terminates the destination buffer, except if dstSize == 0.
 * Returns a pointer to the end of the string (one after last written non-NULL character), 
 * making it suitable to chain multiple calls together.
 * 
 * @param dst destination buffer
 * @param dstSize size of the destination buffer
 * @param val value to convert
 * @param base integer base representation
 * @return char* pointer to the end of the string
 */
char* SML_i64tocharsp(char *dst, unsigned int dstSize, int64_t val, int base);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_SML_STRING_H */
