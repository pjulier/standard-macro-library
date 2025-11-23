
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
    SML_DVEC_T *data;
    size_t size;
    size_t capacity;
} SML_DVEC_TNAME;

static SML_DVEC_TNAME* SML_DVEC_IMPLNAME(create)(void);
static SML_DVEC_TNAME* SML_DVEC_IMPLNAME(createWithCapacity)(size_t capacity);
static SML_DVEC_TNAME* SML_DVEC_IMPLNAME(createWithSize)(size_t size);
static void SML_DVEC_IMPLNAME(free)(SML_DVEC_TNAME *me);
static void SML_DVEC_IMPLNAME(init)(SML_DVEC_TNAME *me);
static void SML_DVEC_IMPLNAME(initWithCapacity)(SML_DVEC_TNAME *me, size_t capacity);
static void SML_DVEC_IMPLNAME(initWithSize)(SML_DVEC_TNAME *me, size_t size);
static void SML_DVEC_IMPLNAME(destroy)(SML_DVEC_TNAME *me);
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

    me->data = (SML_DVEC_T *) malloc(sizeof(*me->data) * capacity);
    assert(me->data != NULL);
    me->capacity = capacity;
    me->size = 0;
}

/**
 * Initialize the vector with a given size
*/
static void SML_DVEC_IMPLNAME(initWithSize)(SML_DVEC_TNAME *me, size_t size)
{
    if (size == 0)
        size = 1;

    me->data = (SML_DVEC_T *) malloc(sizeof(*me->data) * size);
    assert(me->data != NULL);
    me->capacity = size;
    me->size = size;
}

/**
 * Free the underlying resources
*/
static void SML_DVEC_IMPLNAME(destroy)(SML_DVEC_TNAME *me)
{
    free(me->data);
    me->data = NULL;
    me->size = 0;
    me->capacity = 0;
}

/**
 * Create a vector dynamically
*/
static SML_DVEC_TNAME *SML_DVEC_IMPLNAME(create)(void)
{
    SML_DVEC_TNAME *me = (SML_DVEC_TNAME *)malloc(sizeof(SML_DVEC_TNAME));
    assert(me != NULL);
    SML_DVEC_IMPLNAME(init)(me);
    return me;
}

/**
 * Create a vector dynamically with initial capacity
*/
static SML_DVEC_TNAME *SML_DVEC_IMPLNAME(createWithCapacity)(size_t capacity)
{
    SML_DVEC_TNAME *me = (SML_DVEC_TNAME *)malloc(sizeof(SML_DVEC_TNAME));
    assert(me != NULL);
    SML_DVEC_IMPLNAME(initWithCapacity)(me, capacity);
    return me;
}

/**
 * Create a vector dynamically with initial size
*/
static SML_DVEC_TNAME *SML_DVEC_IMPLNAME(createWithSize)(size_t size)
{
    SML_DVEC_TNAME *me = (SML_DVEC_TNAME *)malloc(sizeof(SML_DVEC_TNAME));
    assert(me != NULL);
    SML_DVEC_IMPLNAME(initWithSize)(me, size);
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
    me->size = 0;
}

/**
 * Grow the vector by the standard ratio
*/
static void SML_DVEC_IMPLNAME(grow)(SML_DVEC_TNAME *me)
{
    size_t newCap = SML_DVEC_GROWTH_FACTOR_NUM * me->capacity / SML_DVEC_GROWTH_FACTOR_DEN + (me->capacity == 1); /* plus one for initial alloc */
    me->data = (SML_DVEC_T *)realloc(me->data, newCap * sizeof(*me->data));
    assert(me->data);
    me->capacity = newCap;
}

/**
 * Unconditionally resize the vector to cnt elements
 * NOTE: can also shrink the vector
*/
static void SML_DVEC_IMPLNAME(resize)(SML_DVEC_TNAME *me, size_t cnt)
{
    me->data = (SML_DVEC_T *)realloc(me->data, cnt * sizeof(*me->data));
    assert(me->data);
    me->size = cnt;
    me->capacity = cnt;
}

/**
 * @brief Shrink the allocated memory to the number of elements
 * NOTE: Does nothing if the vector is empty or size == capacity
 */
static void SML_DVEC_IMPLNAME(shrink_to_fit)(SML_DVEC_TNAME *me)
{
    if (me->size > 0 && me->size != me->capacity) {
        me->data = (SML_DVEC_T *)realloc(me->data, me->size * sizeof(*me->data));
        assert(me->data);
        me->capacity = me->size;
    }
}

/**
 * Reserve enough capacity for cnt elements
 * Does nothing if cnt <= capacity
*/
static void SML_DVEC_IMPLNAME(reserve)(SML_DVEC_TNAME *me, size_t cnt)
{
    if (cnt <= me->capacity) 
        return;

    me->data = (SML_DVEC_T *)realloc(me->data, cnt * sizeof(*me->data));
    assert(me->data);
    me->capacity = cnt;
}

/**
 * Append an element to the back and 
 * if necessary, allocate new memory
*/
static void SML_DVEC_IMPLNAME(push_back)(SML_DVEC_TNAME *me, SML_DVEC_T val)
{
    if (me->size >= me->capacity) {
        SML_DVEC_IMPLNAME(grow)(me);
    }
    me->data[me->size++] = val;
}

/**
 * Push back a contiguous block of cnt elements
 * and if necessary, allocate new memory
*/
static void SML_DVEC_IMPLNAME(push_back_block)(SML_DVEC_TNAME *me, const SML_DVEC_T *data, size_t cnt)
{
    const size_t reqCap = me->size + cnt;
    const size_t tempSize = me->size;

    if (me->capacity < reqCap) {
        size_t newCap = me->capacity;
        /* search for the growth iteration that fits */
        while (newCap < reqCap) { newCap = SML_DVEC_GROWTH_FACTOR_NUM * newCap / SML_DVEC_GROWTH_FACTOR_DEN + (newCap == 1); }
        /* resize the vector, also updates number of elements */
        SML_DVEC_IMPLNAME(resize)(me, newCap);
    }

    /* copy at original end of vector */
    memcpy(me->data + tempSize, data, cnt * sizeof(*me->data));

    /* set size to correct value */
    me->size = reqCap;
}

/**
 * Set a contiguous block of cnt elements
 * NOTE: does not check for sufficient available memory!
*/
static void SML_DVEC_IMPLNAME(set_block)(SML_DVEC_TNAME *me, size_t idx, const SML_DVEC_T *data, size_t cnt)
{
    memcpy(me->data + idx, data, cnt * sizeof(*me->data));
}

/**
 * @brief Fill the vector up to its current size with elements val
 * 
 */
static void SML_DVEC_IMPLNAME(fill)(SML_DVEC_TNAME *me, SML_DVEC_T val)
{
    for (size_t i = 0; i < me->size; ++i) {
        me->data[i] = val;
    }
}

/**
 * @brief Fill the vector up to its current size with raw bytes byteVal
 * 
 */
static void SML_DVEC_IMPLNAME(fillBytes)(SML_DVEC_TNAME *me, unsigned char byteVal)
{
    memset(&me->data, byteVal, sizeof(*me->data) * me->size);
}

/**
 * Returns true if the vector is empty
*/
static inline bool SML_DVEC_IMPLNAME(empty)(const SML_DVEC_TNAME *me)
{
    return me->size == 0;
}

/**
 * Return the first element
 * NOTE: undefined behaviour if size == 0
*/
static inline SML_DVEC_T SML_DVEC_IMPLNAME(front)(const SML_DVEC_TNAME *me)
{
    return me->data[0];
}

/**
 * Return a pointer to the first element
 * NOTE: undefined behaviour if size == 0
*/
static inline SML_DVEC_T* SML_DVEC_IMPLNAME(front_p)(const SML_DVEC_TNAME *me)
{
    return me->data;
}

/**
 * Return the last element
 * NOTE: undefined behaviour if size == 0
*/
static inline SML_DVEC_T SML_DVEC_IMPLNAME(back)(const SML_DVEC_TNAME *me)
{
    return me->data[me->size - 1];
}

/**
 * Return a pointer to the last element
 * NOTE: if size == 0, the beginning of memory buffer is returned
*/
static inline SML_DVEC_T* SML_DVEC_IMPLNAME(back_p)(const SML_DVEC_TNAME *me)
{
    return &me->data[(me->size != 0) * (me->size - 1)];
}

/**
 * Set val at index idx
 * NOTE: Does not check for out of range!
*/
static inline void SML_DVEC_IMPLNAME(set)(SML_DVEC_TNAME *me, size_t idx, SML_DVEC_T val)
{
    me->data[idx] = val;
}

/** 
 * Return the current capacity
 */
static inline size_t SML_DVEC_IMPLNAME(capacity)(const SML_DVEC_TNAME *me)
{
    return me->capacity;
}

/**
 * Return a pointer to the beginning of memory buffer
*/
static inline SML_DVEC_T * SML_DVEC_IMPLNAME(begin)(const SML_DVEC_TNAME *me)
{
    return me->data;
}

/**
 * Return a const pointer to the beginning of memory buffer
*/
static inline const SML_DVEC_T * SML_DVEC_IMPLNAME(cbegin)(const SML_DVEC_TNAME *me)
{
    return (const SML_DVEC_T *)me->data;
}

/**
 * Return a pointer to the theoretical element that would
 * follow the last element
 * NOTE: if size == 0, the beginning of memory buffer is returned
*/
static inline SML_DVEC_T * SML_DVEC_IMPLNAME(end)(const SML_DVEC_TNAME *me)
{
    return &me->data[me->size];
}

/**
 * Return a const pointer to the theoretical element that would
 * follow the last element
 * NOTE: if size == 0, the beginning of memory buffer is returned
*/
static inline const SML_DVEC_T * SML_DVEC_IMPLNAME(cend)(const SML_DVEC_TNAME *me)
{
    return (const SML_DVEC_T *)&me->data[me->size];
}

/**
 * Return a pointer to the nth element
*/
static inline SML_DVEC_T * SML_DVEC_IMPLNAME(get_p)(const SML_DVEC_TNAME *me, size_t idx)
{
    return &me->data[idx];
}

/**
 * Return the current number of elements
*/
static inline size_t SML_DVEC_IMPLNAME(size)(const SML_DVEC_TNAME *me)
{
    return me->size;
}

/**
 * Return the element at index idx
*/
static inline SML_DVEC_T SML_DVEC_IMPLNAME(get)(const SML_DVEC_TNAME *me, size_t idx)
{
    return me->data[idx];
}

#undef SML_DVEC_IDENT