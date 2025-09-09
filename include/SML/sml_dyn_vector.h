
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

static void SML_DVEC_IMPLNAME(init)(SML_DVEC_TNAME *me);
static void SML_DVEC_IMPLNAME(initWithCapacity)(SML_DVEC_TNAME *me, size_t capacity);
static void SML_DVEC_IMPLNAME(initWithSize)(SML_DVEC_TNAME *me, size_t size);
static void SML_DVEC_IMPLNAME(destroy)(SML_DVEC_TNAME *me);
static SML_DVEC_TNAME *SML_DVEC_IMPLNAME(create)(void);
static void SML_DVEC_IMPLNAME(free)(SML_DVEC_TNAME *me);
static void SML_DVEC_IMPLNAME(clear)(SML_DVEC_TNAME *me);
static void SML_DVEC_IMPLNAME(grow)(SML_DVEC_TNAME *me);
static void SML_DVEC_IMPLNAME(resize)(SML_DVEC_TNAME *me, size_t cnt);
static void SML_DVEC_IMPLNAME(shrink_to_fit)(SML_DVEC_TNAME *me);
static void SML_DVEC_IMPLNAME(reserve)(SML_DVEC_TNAME *me, size_t cnt);
static void SML_DVEC_IMPLNAME(push_back)(SML_DVEC_TNAME *me, SML_DVEC_T val);
static void SML_DVEC_IMPLNAME(push_back_block)(SML_DVEC_TNAME *me, const SML_DVEC_T *data, size_t cnt);
static void SML_DVEC_IMPLNAME(set_block)(SML_DVEC_TNAME *me, size_t idx, const SML_DVEC_T *data, size_t cnt);
static void SML_DVEC_IMPLNAME(fill)(SML_DVEC_TNAME *me, SML_DVEC_T val);
static void SML_DVEC_IMPLNAME(fillBytes)(SML_DVEC_TNAME *me, unsigned char byteVal);

/*
 * Initialize the vector
 */
static void SML_DVEC_IMPLNAME(init)(SML_DVEC_TNAME *me)
{
    SML_DVEC_IMPLNAME(initWithCapacity)(me, SML_DVEC_INITIAL_CAPACITY);
}

/**
 * Initialize the vector with a given capacity
*/
static void SML_DVEC_IMPLNAME(initWithCapacity)(SML_DVEC_TNAME *me, size_t capacity)
{
    if (capacity == 0)
        capacity = 1;

    me->mem = (SML_DVEC_T *) malloc(sizeof(*me->mem) * capacity);
    assert(me->mem != NULL);
    me->capelems = capacity;
    me->elems = 0;
}

/**
 * Initialize the vector with a given size
*/
static void SML_DVEC_IMPLNAME(initWithSize)(SML_DVEC_TNAME *me, size_t size)
{
    if (size == 0)
        size = 1;

    me->mem = (SML_DVEC_T *) malloc(sizeof(*me->mem) * size);
    assert(me->mem != NULL);
    me->capelems = size;
    me->elems = size;
}

/**
 * Free the underlying resources
*/
static void SML_DVEC_IMPLNAME(destroy)(SML_DVEC_TNAME *me)
{
    free(me->mem);
    me->mem = NULL;
    me->elems = 0;
    me->capelems = 0;
}

/**
 * Create a vector dynamically
*/
static SML_DVEC_TNAME *SML_DVEC_IMPLNAME(create)(void)
{
    SML_DVEC_TNAME *me = (SML_DVEC_TNAME *) malloc(sizeof(SML_DVEC_TNAME));
    assert(me != NULL);
    SML_DVEC_IMPLNAME(init)(me);
    return me;
}

/**
 * Free a dynamically created vector
*/
static void SML_DVEC_IMPLNAME(free)(SML_DVEC_TNAME *me)
{
    if (me) {
        SML_DVEC_IMPLNAME(destroy)(me);
        free(me);
    }
}

/**
 * Clear a vector
*/
static void SML_DVEC_IMPLNAME(clear)(SML_DVEC_TNAME *me)
{
    me->elems = 0;
}

/**
 * Grow the vector by the standard ratio
*/
static void SML_DVEC_IMPLNAME(grow)(SML_DVEC_TNAME *me)
{
    size_t newCap = SML_DVEC_GROWTH_FACTOR_NUM * me->capelems / SML_DVEC_GROWTH_FACTOR_DEN + 1; /* plus one for initial alloc */
    me->mem = (SML_DVEC_T *)realloc(me->mem, newCap * sizeof(*me->mem));
    assert(me->mem);
    me->capelems = newCap;
}

/**
 * Unconditionally resize the vector to cnt elements
 * NOTE: can also shrink the vector
*/
static void SML_DVEC_IMPLNAME(resize)(SML_DVEC_TNAME *me, size_t cnt)
{
    me->mem = (SML_DVEC_T *)realloc(me->mem, cnt * sizeof(*me->mem));
    assert(me->mem);
    me->elems = cnt;
    me->capelems = cnt;
}

/**
 * @brief Shrink the allocated memory to the number of elements
 * NOTE: Does nothing if the vector is empty or size == capacity
 */
static void SML_DVEC_IMPLNAME(shrink_to_fit)(SML_DVEC_TNAME *me)
{
    if (me->elems > 0 && me->elems != me->capelems) {
        me->mem = (SML_DVEC_T *)realloc(me->mem, me->elems * sizeof(*me->mem));
        assert(me->mem);
        me->capelems = me->elems;
    }
}

/**
 * Reserve enough capacity for cnt elements
 * Does nothing if cnt <= capacity
*/
static void SML_DVEC_IMPLNAME(reserve)(SML_DVEC_TNAME *me, size_t cnt)
{
    if (cnt <= me->capelems) 
        return;

    me->mem = (SML_DVEC_T *)realloc(me->mem, cnt * sizeof(*me->mem));
    assert(me->mem);
    me->capelems = cnt;
}

/**
 * Append an element to the back and 
 * if necessary, allocate new memory
*/
static void SML_DVEC_IMPLNAME(push_back)(SML_DVEC_TNAME *me, SML_DVEC_T val)
{
    if (me->elems >= me->capelems) {
        SML_DVEC_IMPLNAME(grow)(me);
    }
    me->mem[me->elems++] = val;
}

/**
 * Push back a contiguous block of cnt elements
 * and if necessary, allocate new memory
*/
static void SML_DVEC_IMPLNAME(push_back_block)(SML_DVEC_TNAME *me, const SML_DVEC_T *data, size_t cnt)
{
    const size_t reqCap = me->elems + cnt;
    const size_t tempElems = me->elems;

    if (me->capelems < reqCap) {
        size_t newCap = me->capelems;
        /* search for the growth iteration that fits */
        while (newCap < reqCap) { newCap = SML_DVEC_GROWTH_FACTOR_NUM * newCap / SML_DVEC_GROWTH_FACTOR_DEN + 1; }
        /* resize the vector, also updates number of elements */
        SML_DVEC_IMPLNAME(resize)(me, newCap);
    }

    /* copy at original end of vector */
    memcpy(me->mem + tempElems, data, cnt * sizeof(*me->mem));

    /* set size to correct value */
    me->elems = reqCap;
}

/**
 * Set a contiguous block of cnt elements
 * NOTE: does not check for sufficient available memory!
*/
static void SML_DVEC_IMPLNAME(set_block)(SML_DVEC_TNAME *me, size_t idx, const SML_DVEC_T *data, size_t cnt)
{
    memcpy(me->mem + idx, data, cnt * sizeof(*me->mem));
}

/**
 * @brief Fill the vector up to its current size with elements val
 * 
 */
static void SML_DVEC_IMPLNAME(fill)(SML_DVEC_TNAME *me, SML_DVEC_T val)
{
    for (size_t i = 0; i < me->elems; ++i) {
        me->mem[i] = val;
    }
}

/**
 * @brief Fill the vector up to its current size with raw bytes byteVal
 * 
 */
static void SML_DVEC_IMPLNAME(fillBytes)(SML_DVEC_TNAME *me, unsigned char byteVal)
{
    memset(&me->mem, byteVal, sizeof(*me->mem) * me->elems);
}

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
 * Return a pointer to the first element
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
 * NOTE: if size == 0, the beginning of memory buffer is returned
*/
static inline SML_DVEC_T* SML_DVEC_IMPLNAME(back_p)(const SML_DVEC_TNAME *me)
{
    return &me->mem[(me->elems != 0) * (me->elems - 1)];
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
 * Return a const pointer to the beginning of memory buffer
*/
static inline const SML_DVEC_T * SML_DVEC_IMPLNAME(cbegin)(const SML_DVEC_TNAME *me)
{
    return (const SML_DVEC_T *)me->mem;
}

/**
 * Return a pointer to the theoretical element that would
 * follow the last element
 * NOTE: if size == 0, the beginning of memory buffer is returned
*/
static inline SML_DVEC_T * SML_DVEC_IMPLNAME(end)(const SML_DVEC_TNAME *me)
{
    return &me->mem[me->elems];
}

/**
 * Return a const pointer to the theoretical element that would
 * follow the last element
 * NOTE: if size == 0, the beginning of memory buffer is returned
*/
static inline const SML_DVEC_T * SML_DVEC_IMPLNAME(cend)(const SML_DVEC_TNAME *me)
{
    return (const SML_DVEC_T *)&me->mem[me->elems];
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