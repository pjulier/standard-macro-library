
#ifndef INCLUDE_SML_DYN_VECTOR
#define INCLUDE_SML_DYN_VECTOR

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h> /* malloc */
#include <string.h> /* memcpy */

#define SML_DVEC_INITIAL_CAPACITY 1
#define SML_DVEC_GROWTH_FACTOR_NUM 3
#define SML_DVEC_GROWTH_FACTOR_DEN 2

/* function names */
#define SML_DVEC_GLUE_IMPL(type, name) SML_DVec_ ## type ## _ ## name 
#define SML_DVEC_IMPL(type, name) SML_DVEC_GLUE_IMPL(type, name) 
#define SML_DVEC_IMPLNAME(name) SML_DVEC_IMPL(SML_DVEC_IDENT, name)
/* type names */
#define SML_DVEC_GLUE_TYPE(type) SML_DVec_ ## type 
#define SML_DVEC_TYPE(type) SML_DVEC_GLUE_TYPE(type)
#define SML_DVEC_TNAME SML_DVEC_TYPE(SML_DVEC_IDENT)

#endif /* INCLUDE_SML_DYN_VECTOR */

/* 
 * NOTE: The following is not protected by header 
 * guard and intended to be included multiple times
 */

#ifndef SML_DVEC_T
#error "SML_DVEC_T (typename) must be defined"
#endif

#ifdef SML_DVEC_ID
#define SML_DVEC_IDENT SML_DVEC_ID
#else
#define SML_DVEC_IDENT SML_DVEC_T
#endif

/* 
 * Vector type definition
 */
typedef struct SML_DVEC_TNAME {
    SML_DVEC_T *mem;
    size_t elems;
    size_t capelems;
} SML_DVEC_TNAME;

void SML_DVEC_IMPLNAME(init)(SML_DVEC_TNAME *me);
void SML_DVEC_IMPLNAME(initWithCapacity)(SML_DVEC_TNAME *me, size_t capacity);
void SML_DVEC_IMPLNAME(initWithSize)(SML_DVEC_TNAME *me, size_t size);
void SML_DVEC_IMPLNAME(destroy)(SML_DVEC_TNAME *me);
SML_DVEC_TNAME *SML_DVEC_IMPLNAME(create)(void);
void SML_DVEC_IMPLNAME(free)(SML_DVEC_TNAME *me);
void SML_DVEC_IMPLNAME(clear)(SML_DVEC_TNAME *me);
void SML_DVEC_IMPLNAME(grow)(SML_DVEC_TNAME *me);
void SML_DVEC_IMPLNAME(resize)(SML_DVEC_TNAME *me, size_t cnt);
void SML_DVEC_IMPLNAME(reserve)(SML_DVEC_TNAME *me, size_t cnt);
void SML_DVEC_IMPLNAME(push_back)(SML_DVEC_TNAME *me, SML_DVEC_T val);
void SML_DVEC_IMPLNAME(push_back_block)(SML_DVEC_TNAME *me, const SML_DVEC_T *data, size_t cnt);
void SML_DVEC_IMPLNAME(set_block)(SML_DVEC_TNAME *me, size_t idx, const SML_DVEC_T *data, size_t cnt);
void SML_DVEC_IMPLNAME(fill)(SML_DVEC_TNAME *me, SML_DVEC_T val);
void SML_DVEC_IMPLNAME(fillBytes)(SML_DVEC_TNAME *me, unsigned char byteVal);

/**
 * Returns true if the vector is empty
*/
static inline bool SML_DVEC_IMPLNAME(empty)(const SML_DVEC_TNAME *me)
{
    return me->elems == 0;
}

/**
 * Return the first element
 * NOTE: undefined behaviour if size == 0
*/
static inline SML_DVEC_T SML_DVEC_IMPLNAME(front)(const SML_DVEC_TNAME *me)
{
    return me->mem[0];
}

/**
 * Return a pointer to the  first element
 * NOTE: undefined behaviour if size == 0
*/
static inline SML_DVEC_T* SML_DVEC_IMPLNAME(front_p)(const SML_DVEC_TNAME *me)
{
    return me->mem;
}

/**
 * Return the last element
 * NOTE: undefined behaviour if size == 0
*/
static inline SML_DVEC_T SML_DVEC_IMPLNAME(back)(const SML_DVEC_TNAME *me)
{
    return me->mem[me->elems - 1];
}

/**
 * Return a pointer to the last element
 * NOTE: undefined behaviour if size == 0
*/
static inline SML_DVEC_T* SML_DVEC_IMPLNAME(back_p)(const SML_DVEC_TNAME *me)
{
    return &me->mem[me->elems - 1];
}

/**
 * Set val at index idx
 * NOTE: Does not check for out of range!
*/
static inline void SML_DVEC_IMPLNAME(set)(SML_DVEC_TNAME *me, size_t idx, SML_DVEC_T val)
{
    me->mem[idx] = val;
}

/** 
 * Return the current capacity
 */
static inline size_t SML_DVEC_IMPLNAME(capacity)(const SML_DVEC_TNAME *me)
{
    return me->capelems;
}

/**
 * Return a pointer to the beginning of memory buffer
*/
static inline SML_DVEC_T * SML_DVEC_IMPLNAME(begin)(const SML_DVEC_TNAME *me)
{
    return me->mem;
}

/**
 * Return a pointer to the nth element
*/
static inline SML_DVEC_T * SML_DVEC_IMPLNAME(get_p)(const SML_DVEC_TNAME *me, size_t idx)
{
    return &me->mem[idx];
}

/**
 * Return the current number of elements
*/
static inline size_t SML_DVEC_IMPLNAME(size)(const SML_DVEC_TNAME *me)
{
    return me->elems;
}

/**
 * Return the element at index idx
*/
static inline SML_DVEC_T SML_DVEC_IMPLNAME(get)(const SML_DVEC_TNAME *me, size_t idx)
{
    return me->mem[idx];
}

#undef SML_DVEC_IDENT