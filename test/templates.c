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

/**
 * Implement hash map of type unsigned int
*/
#ifndef SML_EHASH_uint_IMPL
#define SML_EHASH_uint_IMPL
#define SML_EHASH_T unsigned int
#define SML_EHASH_KEYT char *
#define SML_EHASH_ID uint
#define SML_EHASH_ISKEYSTRING true
#define SML_EHASH_ISKEYCSTRING true
#include "SML/sml_extendible_hash.inl"
#undef SML_EHASH_ISKEYCSTRING
#undef SML_EHASH_ISKEYSTRING
#undef SML_EHASH_ID
#undef SML_EHASH_KEYT
#undef SML_EHASH_T
#endif /* SML_EHASH_uint_IMPL */

/**
 * Implement circular buffer of unsigned int and capacity of 8
*/
#ifndef SML_CIRCBUF_uint_IMPL
#define SML_CIRCBUF_uint_IMPL
#define SML_CIRCBUF_T unsigned int
#define SML_CIRCBUF_CAPACITY 8
#define SML_CIRCBUF_ID uint
#include "SML/sml_circular_buffer.inl"
#undef SML_CIRCBUF_ID
#undef SML_CIRCBUF_CAPACITY
#undef SML_CIRCBUF_T
#endif /* SML_DVEC_uint_IMPL */ 

/**
 * Implement a dynamic queue of unsigned int
*/
#ifndef SML_DQUEUE_uint_IMPL
#define SML_DQUEUE_uint_IMPL
#define SML_DQUEUE_T unsigned int
#define SML_DQUEUE_ID uint
#include "SML/sml_dyn_queue.inl"
#undef SML_DQUEUE_ID
#undef SML_DQUEUE_T
#endif /* SML_DQUEUE_uint_IMPL */ 