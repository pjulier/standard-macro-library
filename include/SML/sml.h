#ifndef INCLUDE_SML_H
#define INCLUDE_SML_H

#include "sml_common.h"

/*
 * SML library version macros
 */
#define SML_VERSION_MAJOR   0
#define SML_VERSION_MINOR   2
#define SML_VERSION_RELEASE 1

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Return the library version at the time of compilation
 * 
 * Can be used to check against the version defined in this header
 * file at runtime.
 * 
 * @param major   major version number
 * @param minor   minor version number
 * @param release release version number
 */
void SML_getVersion(int *major, int *minor, int *release);

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_SML_H */