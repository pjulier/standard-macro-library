#ifndef INCLUDE_SML_PARSING_UTILS_H
#define INCLUDE_SML_PARSING_UTILS_H

#include <stdbool.h>

static inline bool SML_ParsUtil_isLineEnd(char c) {
    return (c == '\r' || c == '\n' || c == '\0' || c == '\f');
}

static inline bool SML_ParsUtil_isSpace(char c) {
    return (c == ' ' || c == '\t');
}

static inline bool SML_ParsUtil_isLower(char c) {
    return (c >= 'a' && c <= 'z');
}

static inline bool SML_ParsUtil_isUpper(char c) {
    return (c >= 'A' && c <= 'Z');
}

static inline bool SML_ParsUtil_isDigit(char c) {
    return (c >= '0' && c <= '9');
}



#endif /* INCLUDE_SML_PARSING_UTILS_H */
