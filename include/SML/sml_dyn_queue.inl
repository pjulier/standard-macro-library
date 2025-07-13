#ifndef SML_DQUEUE_T
#error "SML_DQUEUE_T (typename) must be defined"
#endif

#ifdef SML_DQUEUE_ID
#define SML_DQUEUE_IDENT SML_DQUEUE_ID
#else
#define SML_DQUEUE_IDENT SML_DQUEUE_T
#endif

#define SML_DQUEUE_INITIAL_CAPACITY    2
#define SML_DQUEUE_GROWTH_FACTOR_NUM   3
#define SML_DQUEUE_GROWTH_FACTOR_DENUM 2

SML_DQUEUE_TNAME * SML_DQUEUE_IMPLNAME(create)(void)
{
    SML_DQUEUE_TNAME *me = (SML_DQUEUE_TNAME *)malloc(sizeof(*me));
    assert(me != NULL);
    SML_DQUEUE_IMPLNAME(initWithCapacity)(me, SML_DQUEUE_INITIAL_CAPACITY);
    return me;
}

void SML_DQUEUE_IMPLNAME(init)(SML_DQUEUE_TNAME *me)
{
    SML_DQUEUE_IMPLNAME(initWithCapacity)(me, SML_DQUEUE_INITIAL_CAPACITY);
}

void SML_DQUEUE_IMPLNAME(initWithCapacity)(SML_DQUEUE_TNAME *me, size_t initialCapacity)
{
    if (initialCapacity == 0)
        initialCapacity = 1;

    me->capacity = initialCapacity;
    me->front = 0;
    me->back = 0;
    me->mem = (SML_DQUEUE_T *)malloc(me->capacity * sizeof(*me->mem));
    assert(me->mem);
}

size_t SML_DQUEUE_IMPLNAME(size)(SML_DQUEUE_TNAME *me) {
    ssize_t size;
    size = (ssize_t)me->back - me->front;
    if (size < 0) {
        size += me->capacity;
    }
    return size;
}

void SML_DQUEUE_IMPLNAME(push)(SML_DQUEUE_TNAME *me, SML_DQUEUE_T val) {
    size_t next = (me->back + 1) % me->capacity;
    if (next == me->front) {
        /* buffer is full on next insert, realloc */
        size_t newCapacity = SML_DQUEUE_GROWTH_FACTOR_NUM * me->capacity / SML_DQUEUE_GROWTH_FACTOR_DENUM + 1;
        me->mem = (SML_DQUEUE_T *)realloc(me->mem, newCapacity * sizeof(*me->mem));
        assert(me->mem);

        /* remove discontinuity */
        if (me->front > me->back) {
            size_t shift = newCapacity - me->capacity;
            memmove(&me->mem[me->front + shift], &me->mem[me->front], (me->capacity - me->front) * sizeof(*me->mem));
            me->front = me->front + shift;
        }

        me->capacity = newCapacity;
        /* insert */
        me->mem[me->back] = val;
        me->back = (me->back + 1) % me->capacity;
    } else {
        me->mem[me->back] = val;
        me->back = next;
    }
}

void SML_DQUEUE_IMPLNAME(pop)(SML_DQUEUE_TNAME *me) {
    if (SML_DQUEUE_IMPLNAME(empty)(me)) {
        /* should never happen*/
        return;
    }
    me->front = (me->front + 1) % me->capacity;
}

SML_DQUEUE_T SML_DQUEUE_IMPLNAME(front)(SML_DQUEUE_TNAME *me) {
    if (SML_DQUEUE_IMPLNAME(empty)(me)) {
        /* should never happen*/
        return 0;
    }
    return me->mem[me->front];
}


void SML_DQUEUE_IMPLNAME(destroy)(SML_DQUEUE_TNAME *me) {
    free(me->mem);
}

void SML_DQUEUE_IMPLNAME(free)(SML_DQUEUE_TNAME *me) {
    if (me != NULL) {
        SML_DQUEUE_IMPLNAME(destroy)(me);
        free(me);
    }
}

#undef SML_DQUEUE_IDENT