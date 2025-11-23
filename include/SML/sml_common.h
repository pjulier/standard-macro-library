#ifndef INCLUDE_SML_COMMON_H
#define INCLUDE_SML_COMMON_H

#include <string.h> /* for memset */

/*
 * Some useful macros
 */
#define SML_ARRCOUNT(x) (sizeof(x) / sizeof(x[0]))
#define SML_ZERO(x)  (memset(&(x), 0, sizeof(x)))
#define SML_ZEROA(x) (memset(x, 0, sizeof(x)))
#define SML_ZEROP(x) (memset(x, 0, sizeof(*x)))

#endif /* INCLUDE_SML_COMMON_H */