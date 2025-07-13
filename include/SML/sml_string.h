#ifndef INCLUDE_SML_STRING_H
#define INCLUDE_SML_STRING_H

#include <stdint.h>

void  SML_uint32tobin(uint32_t value, char *str, int places);
char* SML_strpcpy(char *dst, const char *src);
char* SML_strdup(const char *src);
char* SML_strviewdup(const char *src, unsigned int size);
int   SML_strtoi(const char *str, const char **end);

#endif /* INCLUDE_SML_STRING_H */