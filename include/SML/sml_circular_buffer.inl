/**
 * Fixed capacity circular buffer of arbitrary type and size
*/

#include <string.h> /* for memset */

#ifdef SML_CIRCBUF_ID
#define SML_CIRCBUF_IDENT SML_CIRCBUF_ID
#else
#define SML_CIRCBUF_IDENT SML_CIRCBUF_T
#endif

/**
 * Initialize the circular buffer
*/
void SML_CIRCBUF_IMPLNAME(init)(SML_CIRCBUF_TNAME *me)
{
    memset(me, 0, sizeof(*me));
}

/**
 * Push an element to the back of the circular buffer
*/
void SML_CIRCBUF_IMPLNAME(push)(SML_CIRCBUF_TNAME *me, SML_CIRCBUF_T val)
{
    me->buf[me->back] = val;
    me->back = (me->back + 1) % SML_CIRCBUF_CAPACITY;

    /* buffer is full (front == back)? */
    if (SML_CIRCBUF_IMPLNAME(empty)(me)) {
        /* discard front element */
        me->front = (me->front + 1) % SML_CIRCBUF_CAPACITY;
    }
}

/**
 * Pop an element from the front of the circular buffer
*/
void SML_CIRCBUF_IMPLNAME(pop)(SML_CIRCBUF_TNAME *me)
{
    me->front = (me->front + 1) % SML_CIRCBUF_CAPACITY;
}