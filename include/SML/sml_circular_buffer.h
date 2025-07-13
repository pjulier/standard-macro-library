/**
 * Fixed capacity circular buffer of arbitrary type and size
*/

#ifndef INCLUDE_SML_CIRCULAR_BUFFER
#define INCLUDE_SML_CIRCULAR_BUFFER

/* function names */
#define SML_CIRCBUF_GLUE_IMPL(type, name) SML_CircBuf_ ## type ## _ ## name 
#define SML_CIRCBUF_IMPL(type, name) SML_CIRCBUF_GLUE_IMPL(type, name) 
#define SML_CIRCBUF_IMPLNAME(name) SML_CIRCBUF_IMPL(SML_CIRCBUF_IDENT, name)
/* type names */
#define SML_CIRCBUF_GLUE_TYPE(type) SML_CircBuf_ ## type 
#define SML_CIRCBUF_TYPE(type) SML_CIRCBUF_GLUE_TYPE(type)
#define SML_CIRCBUF_TNAME SML_CIRCBUF_TYPE(SML_CIRCBUF_IDENT)

#endif /* INCLUDE_SML_CIRCULAR_BUFFER */

/* 
 * NOTE: The following is not protected by header 
 * guard and intended to be included multiple times
 */

#ifndef SML_CIRCBUF_T
#error "SML_CIRCBUF_T (typename) must be defined"
#endif

#ifndef SML_CIRCBUF_CAPACITY
#error "SML_CIRCBUF_CAPACITY (fixed capacity) must be defined"
#endif

#ifdef SML_CIRCBUF_ID
#define SML_CIRCBUF_IDENT SML_CIRCBUF_ID
#else
#define SML_CIRCBUF_IDENT SML_CIRCBUF_T
#endif

#ifdef __cplusplus
extern "C" {
#endif

/* 
 * Vector type definition
 */
typedef struct SML_CIRCBUF_TNAME {
    SML_CIRCBUF_T buf[SML_CIRCBUF_CAPACITY];
    unsigned long front;
    unsigned long back;
} SML_CIRCBUF_TNAME;

void SML_CIRCBUF_IMPLNAME(init)(SML_CIRCBUF_TNAME *me);
void SML_CIRCBUF_IMPLNAME(push)(SML_CIRCBUF_TNAME *me, SML_CIRCBUF_T val);
void SML_CIRCBUF_IMPLNAME(pop)(SML_CIRCBUF_TNAME *me);

/**
 * Returns true if the circular buffer is empty
*/
static inline bool SML_CIRCBUF_IMPLNAME(empty)(const SML_CIRCBUF_TNAME *me)
{
    return me->front == me->back;
}

/**
 * Return the front element
 * NOTE: undefined behaviour if size == 0
*/
static inline SML_CIRCBUF_T SML_CIRCBUF_IMPLNAME(front)(const SML_CIRCBUF_TNAME *me)
{
    return me->buf[me->front];
}

/**
 * Return the current number of elements
*/
static inline unsigned int SML_CIRCBUF_IMPLNAME(size)(const SML_CIRCBUF_TNAME *me)
{
    ssize_t size;
    size = (ssize_t)me->back - me->front;
    if (size < 0) {
        size += SML_CIRCBUF_CAPACITY;
    }
    return size;
}

#ifdef __cplusplus
}
#endif

#undef SML_CIRCBUF_IDENT