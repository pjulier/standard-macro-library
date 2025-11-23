#ifndef INCLUDE_SML_DYN_QUEUE_H
#define INCLUDE_SML_DYN_QUEUE_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* function names */
#define SML_DQUEUE_GLUE_IMPL(type, name) SML_DQueue_ ## type ## _ ## name 
#define SML_DQUEUE_IMPL(type, name) SML_DQUEUE_GLUE_IMPL(type, name) 
#define SML_DQUEUE_IMPLNAME(name) SML_DQUEUE_IMPL(SML_DQUEUE_IDENT, name)
/* type names */
#define SML_DQUEUE_GLUE_TYPE(type) SML_DQueue_ ## type 
#define SML_DQUEUE_TYPE(type) SML_DQUEUE_GLUE_TYPE(type)
#define SML_DQUEUE_TNAME SML_DQUEUE_TYPE(SML_DQUEUE_IDENT)

#define SML_DQUEUE_INITIAL_CAPACITY    2
#define SML_DQUEUE_GROWTH_FACTOR_NUM   3
#define SML_DQUEUE_GROWTH_FACTOR_DENUM 2

#endif /* INCLUDE_SML_DYN_QUEUE */

/* 
 * NOTE: The following is not protected by header 
 * guard and intended to be included multiple times
 */
#ifndef SML_DQUEUE_T
#error "SML_DQUEUE_T (typename) must be defined"
#endif

#ifdef SML_DQUEUE_ID
#define SML_DQUEUE_IDENT SML_DQUEUE_ID
#else
#define SML_DQUEUE_IDENT SML_DQUEUE_T
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 * DQueue definition
 */
typedef struct SML_DQUEUE_TNAME {
    SML_DQUEUE_T *data;
    size_t capacity;
    size_t front;
    size_t back;
} SML_DQUEUE_TNAME;

static SML_DQUEUE_TNAME* SML_DQUEUE_IMPLNAME(create)(void);
static void SML_DQUEUE_IMPLNAME(free)(SML_DQUEUE_TNAME *me);
static void SML_DQUEUE_IMPLNAME(init)(SML_DQUEUE_TNAME *me);
static void SML_DQUEUE_IMPLNAME(initWithCapacity)(SML_DQUEUE_TNAME *me, size_t initialCapacity);
static void SML_DQUEUE_IMPLNAME(destroy)(SML_DQUEUE_TNAME *me);
static size_t SML_DQUEUE_IMPLNAME(size)(SML_DQUEUE_TNAME *me);
static void SML_DQUEUE_IMPLNAME(push)(SML_DQUEUE_TNAME *me, SML_DQUEUE_T val);
static SML_DQUEUE_T SML_DQUEUE_IMPLNAME(front)(SML_DQUEUE_TNAME *me);
static SML_DQUEUE_T SML_DQUEUE_IMPLNAME(back)(SML_DQUEUE_TNAME *me);
static void SML_DQUEUE_IMPLNAME(pop)(SML_DQUEUE_TNAME *me);
static void SML_DQUEUE_IMPLNAME(clear)(SML_DQUEUE_TNAME *me);
static inline bool SML_DQUEUE_IMPLNAME(empty)(SML_DQUEUE_TNAME *me);

static SML_DQUEUE_TNAME * SML_DQUEUE_IMPLNAME(create)(void)
{
    SML_DQUEUE_TNAME *me = (SML_DQUEUE_TNAME *)malloc(sizeof(*me));
    assert(me != NULL);
    SML_DQUEUE_IMPLNAME(initWithCapacity)(me, SML_DQUEUE_INITIAL_CAPACITY);
    return me;
}

static void SML_DQUEUE_IMPLNAME(free)(SML_DQUEUE_TNAME *me)
{
    if (me != NULL) {
        SML_DQUEUE_IMPLNAME(destroy)(me);
        free(me);
    }
}

static void SML_DQUEUE_IMPLNAME(init)(SML_DQUEUE_TNAME *me)
{
    SML_DQUEUE_IMPLNAME(initWithCapacity)(me, SML_DQUEUE_INITIAL_CAPACITY);
}

static void SML_DQUEUE_IMPLNAME(initWithCapacity)(SML_DQUEUE_TNAME *me, size_t initialCapacity)
{
    if (initialCapacity == 0)
        initialCapacity = 1;

    me->capacity = initialCapacity;
    me->front = 0;
    me->back = 0;
    me->data = (SML_DQUEUE_T *)malloc(me->capacity * sizeof(*me->data));
    assert(me->data);
}

static void SML_DQUEUE_IMPLNAME(destroy)(SML_DQUEUE_TNAME *me)
{
    free(me->data);
}

static size_t SML_DQUEUE_IMPLNAME(size)(SML_DQUEUE_TNAME *me) {
    ptrdiff_t size;
    size = (ptrdiff_t)me->back - me->front;
    if (size < 0) {
        size += me->capacity;
    }
    return size;
}

static void SML_DQUEUE_IMPLNAME(push)(SML_DQUEUE_TNAME *me, SML_DQUEUE_T val) {
    size_t next = (me->back + 1) % me->capacity;
    if (next == me->front) {
        /* buffer is full on next insert, realloc */
        size_t newCapacity = SML_DQUEUE_GROWTH_FACTOR_NUM * me->capacity / SML_DQUEUE_GROWTH_FACTOR_DENUM + (me->capacity == 1);
        me->data = (SML_DQUEUE_T *)realloc(me->data, newCapacity * sizeof(*me->data));
        assert(me->data);

        /* remove discontinuity */
        if (me->front > me->back) {
            size_t shift = newCapacity - me->capacity;
            memmove(&me->data[me->front + shift], &me->data[me->front], (me->capacity - me->front) * sizeof(*me->data));
            me->front = me->front + shift;
        }

        me->capacity = newCapacity;
        /* insert */
        me->data[me->back] = val;
        me->back = (me->back + 1) % me->capacity;
    } else {
        me->data[me->back] = val;
        me->back = next;
    }
}

static void SML_DQUEUE_IMPLNAME(pop)(SML_DQUEUE_TNAME *me) {
    if (SML_DQUEUE_IMPLNAME(empty)(me)) {
        /* should never happen*/
        return;
    }
    me->front = (me->front + 1) % me->capacity;
}

static SML_DQUEUE_T SML_DQUEUE_IMPLNAME(front)(SML_DQUEUE_TNAME *me) {
    if (SML_DQUEUE_IMPLNAME(empty)(me)) {
        /* should never happen*/
#ifdef __cplusplus
        return (SML_DQUEUE_T){};
#else
        return (SML_DQUEUE_T){0};
#endif
    }
    return me->data[me->front];
}

static SML_DQUEUE_T SML_DQUEUE_IMPLNAME(back)(SML_DQUEUE_TNAME *me)
{
    if (SML_DQUEUE_IMPLNAME(empty)(me)) {
        /* should never happen*/
#ifdef __cplusplus
        return (SML_DQUEUE_T){};
#else
        return (SML_DQUEUE_T){0};
#endif
    }
    return me->data[me->back];
}

static void SML_DQUEUE_IMPLNAME(clear)(SML_DQUEUE_TNAME *me)
{
    me->back = 0;
    me->front = 0;
}

static inline bool SML_DQUEUE_IMPLNAME(empty)(SML_DQUEUE_TNAME *me)
{
    return me->front == me->back;
}

#ifdef __cplusplus
}
#endif

#undef SML_DQUEUE_IDENT