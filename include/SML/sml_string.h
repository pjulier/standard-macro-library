#ifndef INCLUDE_SML_STRING_H
#define INCLUDE_SML_STRING_H

#include <stdint.h>

#include "sml_common.h"


void  SML_uint64tobin(uint64_t value, char *str, int places);
void  SML_uint32tobin(uint32_t value, char *str, int places);
char* SML_strpcpy(char *dst, const char *src);
char* SML_strdup(const char *src);
char* SML_strviewdup(const char *src, unsigned int size);
int   SML_strtoi(const char *str, const char **end);
char* SML_itoa(char *dst, unsigned int size, int val, int base);

#endif /* INCLUDE_SML_STRING_H */