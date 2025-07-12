#include "templates.h"

/**
 * Implement dynamic vector of type unsigned int
*/
#ifndef SML_DVEC_uint_IMPL
#define SML_DVEC_uint_IMPL
#define SML_DVEC_T unsigned int
#define SML_DVEC_ID uint
#include "SML/sml_dyn_vector.inl"
#undef SML_DVEC_ID
#undef SML_DVEC_T
#endif /* SML_DVEC_uint_IMPL */