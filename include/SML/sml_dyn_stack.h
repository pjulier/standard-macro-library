#ifndef INCLUDE_SML_DYN_STACK_H
#define INCLUDE_SML_DYN_STACK_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* function names */
#define SML_DSTACK_GLUE_IMPL(type, name) SML_DStack_ ## type ## _ ## name 
#define SML_DSTACK_IMPL(type, name) SML_DSTACK_GLUE_IMPL(type, name) 
#define SML_DSTACK_IMPLNAME(name) SML_DSTACK_IMPL(SML_DSTACK_IDENT, name)
/* type names */
#define SML_DSTACK_GLUE_TYPE(type) SML_DStack_ ## type 
#define SML_DSTACK_TYPE(type) SML_DSTACK_GLUE_TYPE(type)
#define SML_DSTACK_TNAME SML_DSTACK_TYPE(SML_DSTACK_IDENT)

#define SML_DSTACK_INITIAL_CAPACITY    2
#define SML_DSTACK_GROWTH_FACTOR_NUM   3
#define SML_DSTACK_GROWTH_FACTOR_DEN   2

#endif /* INCLUDE_SML_DYN_STACK_H */

/* 
 * NOTE: The following is not protected by header 
 * guard and intended to be included multiple times
 */
#ifndef SML_DSTACK_T
#error "SML_DSTACK_T (typename) must be defined"
#endif

#ifdef SML_DSTACK_ID
#define SML_DSTACK_IDENT SML_DSTACK_ID
#else
#define SML_DSTACK_IDENT SML_DSTACK_T
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 * DStack definition
 */
typedef struct SML_DSTACK_TNAME {
    SML_DSTACK_T *data;
    size_t capacity;
    size_t size;
} SML_DSTACK_TNAME;

static SML_DSTACK_TNAME* SML_DSTACK_IMPLNAME(create)(void);
static SML_DSTACK_TNAME* SML_DSTACK_IMPLNAME(createWithCapacity)(size_t initialCapacity);
static void SML_DSTACK_IMPLNAME(free)(SML_DSTACK_TNAME *me);
static void SML_DSTACK_IMPLNAME(init)(SML_DSTACK_TNAME *me);
static void SML_DSTACK_IMPLNAME(initWithCapacity)(SML_DSTACK_TNAME *me, size_t initialCapacity);
static void SML_DSTACK_IMPLNAME(destroy)(SML_DSTACK_TNAME *me);
static size_t SML_DSTACK_IMPLNAME(size)(SML_DSTACK_TNAME *me);
static SML_DSTACK_T SML_DSTACK_IMPLNAME(back)(SML_DSTACK_TNAME *me);
static void SML_DSTACK_IMPLNAME(push)(SML_DSTACK_TNAME *me, SML_DSTACK_T val);
static void SML_DSTACK_IMPLNAME(pop)(SML_DSTACK_TNAME *me);
static void SML_DSTACK_IMPLNAME(grow)(SML_DSTACK_TNAME *me);
static void SML_DSTACK_IMPLNAME(clear)(SML_DSTACK_TNAME *me);
static inline bool SML_DSTACK_IMPLNAME(empty)(SML_DSTACK_TNAME *me);

static SML_DSTACK_TNAME * SML_DSTACK_IMPLNAME(create)(void)
{
    SML_DSTACK_TNAME *me = (SML_DSTACK_TNAME *)malloc(sizeof(*me));
    assert(me != NULL);
    SML_DSTACK_IMPLNAME(initWithCapacity)(me, SML_DSTACK_INITIAL_CAPACITY);
    return me;
}

static SML_DSTACK_TNAME * SML_DSTACK_IMPLNAME(createWithCapacity)(size_t initialCapacity)
{
    SML_DSTACK_TNAME *me = (SML_DSTACK_TNAME *)malloc(sizeof(*me));
    assert(me != NULL);
    SML_DSTACK_IMPLNAME(initWithCapacity)(me, initialCapacity);
    return me;
}

static void SML_DSTACK_IMPLNAME(free)(SML_DSTACK_TNAME *me)
{
    if (me != NULL) {
        SML_DSTACK_IMPLNAME(destroy)(me);
        free(me);
    }
}

static void SML_DSTACK_IMPLNAME(init)(SML_DSTACK_TNAME *me)
{
    SML_DSTACK_IMPLNAME(initWithCapacity)(me, SML_DSTACK_INITIAL_CAPACITY);
}

static void SML_DSTACK_IMPLNAME(initWithCapacity)(SML_DSTACK_TNAME *me, size_t initialCapacity)
{
    if (initialCapacity == 0)
        initialCapacity = 1;

    me->capacity = initialCapacity;
    me->size = 0;
    me->data = (SML_DSTACK_T *)malloc(me->capacity * sizeof(*me->data));
    assert(me->data);
}

static void SML_DSTACK_IMPLNAME(destroy)(SML_DSTACK_TNAME *me)
{
    free(me->data);
}

static size_t SML_DSTACK_IMPLNAME(size)(SML_DSTACK_TNAME *me)
{
    return me->size;
}

static void SML_DSTACK_IMPLNAME(push)(SML_DSTACK_TNAME *me, SML_DSTACK_T val)
{
    if (me->size >= me->capacity) {
        SML_DSTACK_IMPLNAME(grow)(me);
    }
    me->data[me->size++] = val;
}

static void SML_DSTACK_IMPLNAME(grow)(SML_DSTACK_TNAME *me)
{
    size_t newCap = SML_DSTACK_GROWTH_FACTOR_NUM * me->capacity / SML_DSTACK_GROWTH_FACTOR_DEN + (me->capacity == 1); /* plus one for initial alloc */
    me->data = (SML_DSTACK_T *)realloc(me->data, newCap * sizeof(*me->data));
    assert(me->data);
    me->capacity = newCap;
}

static void SML_DSTACK_IMPLNAME(reserve)(SML_DSTACK_TNAME *me, size_t count)
{
    if (count <= me->capacity) { return; }

    me->data = (SML_DSTACK_T *)realloc(me->data, count * sizeof(*me->data));
    assert(me->data);
    me->capacity = count;
}

static void SML_DSTACK_IMPLNAME(pop)(SML_DSTACK_TNAME *me)
{
    if (SML_DSTACK_IMPLNAME(empty)(me)) { return; }
    --me->size;
}

static SML_DSTACK_T SML_DSTACK_IMPLNAME(back)(SML_DSTACK_TNAME *me)
{
    if (SML_DSTACK_IMPLNAME(empty)(me)) {
        /* should never happen*/
        return (SML_DSTACK_T){};
    }
    return me->data[me->size - 1];
}

static void SML_DSTACK_IMPLNAME(clear)(SML_DSTACK_TNAME *me)
{
    me->size = 0;
}

static inline bool SML_DSTACK_IMPLNAME(empty)(SML_DSTACK_TNAME *me)
{
    return me->size == 0;
}

#ifdef __cplusplus
}
#endif

#undef SML_DSTACK_IDENT