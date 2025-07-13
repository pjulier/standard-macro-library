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
    SML_DQUEUE_T *mem;
    size_t capacity;
    size_t front;
    size_t back;
} SML_DQUEUE_TNAME;

SML_DQUEUE_TNAME* SML_DQUEUE_IMPLNAME(create)(void);
void SML_DQUEUE_IMPLNAME(free)(SML_DQUEUE_TNAME *me);
void SML_DQUEUE_IMPLNAME(init)(SML_DQUEUE_TNAME *me);
void SML_DQUEUE_IMPLNAME(initWithCapacity)(SML_DQUEUE_TNAME *me, size_t initialCapacity);
void SML_DQUEUE_IMPLNAME(destroy)(SML_DQUEUE_TNAME *me);
size_t SML_DQUEUE_IMPLNAME(size)(SML_DQUEUE_TNAME *me);
void SML_DQUEUE_IMPLNAME(push)(SML_DQUEUE_TNAME *me, SML_DQUEUE_T val);
SML_DQUEUE_T SML_DQUEUE_IMPLNAME(front)(SML_DQUEUE_TNAME *me);
void SML_DQUEUE_IMPLNAME(pop)(SML_DQUEUE_TNAME *me);

static inline bool SML_DQUEUE_IMPLNAME(empty)(SML_DQUEUE_TNAME *me)
{
    return me->front == me->back;
}

#ifdef __cplusplus
}
#endif

#undef SML_DQUEUE_IDENT