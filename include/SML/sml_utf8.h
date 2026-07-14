/**
 * @file sml_utf8.h
 * @brief Utf-8 validation and decoding
 * 
 */
#ifndef INCLUDE_SML_UTF8_H
#define INCLUDE_SML_UTF8_H

#include "stdint.h"
#include "stddef.h"
#include "stdbool.h"

#define SML_UTF8_ERROR  0
#define SML_UTF8_OK     6

bool SML_utf8_validate(const char *src, uint64_t len);


#endif /* INCLUDE_SML_UTF8_H */