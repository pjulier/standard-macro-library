#include "SML/sml.h"

void SML_getVersion(int *major, int *minor, int *release)
{
    *major   = SML_VERSION_MAJOR;
    *minor   = SML_VERSION_MINOR;
    *release = SML_VERSION_RELEASE;
}