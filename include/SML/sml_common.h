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
#define SML_STRLEN_LITERAL(s) (sizeof(s) / sizeof(s[0]) - 1)

/*
 * SML logger tag
 */
#define SML_LOGTAG "SML"

/*
 * Common types
 */
typedef int SMLReturn;
typedef enum SMLReturnEnum {
    SML_RET_OK = 0,
    SML_RET_EINVAL,
    SML_RET_ERANGE,
    SML_RET_ENOMEM
} SMLReturnEnum;

#endif /* INCLUDE_SML_COMMON_H */
