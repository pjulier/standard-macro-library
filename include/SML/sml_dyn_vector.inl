
#ifdef SML_DVEC_ID
#define SML_DVEC_IDENT SML_DVEC_ID
#else
#define SML_DVEC_IDENT SML_DVEC_T
#endif

/*
 * Initialize the vector
 */
void SML_DVEC_IMPLNAME(init)(SML_DVEC_TNAME *me)
{
    SML_DVEC_IMPLNAME(initWithCapacity)(me, SML_DVEC_INITIAL_CAPACITY);
}

/**
 * Initialize the vector with a given capacity
*/
void SML_DVEC_IMPLNAME(initWithCapacity)(SML_DVEC_TNAME *me, size_t capacity)
{
    me->mem = (SML_DVEC_T *) malloc(sizeof(*me->mem) * capacity);
    assert(me->mem != NULL);
    me->capelems = capacity;
    me->elems = 0;
}

/**
 * Initialize the vector with a given size
*/
void SML_DVEC_IMPLNAME(initWithSize)(SML_DVEC_TNAME *me, size_t size)
{
    me->mem = (SML_DVEC_T *) malloc(sizeof(*me->mem) * size);
    assert(me->mem != NULL);
    me->capelems = size;
    me->elems = size;
}

/**
 * Free the underlying resources
*/
void SML_DVEC_IMPLNAME(destroy)(SML_DVEC_TNAME *me)
{
    free(me->mem);
    me->mem = NULL;
    me->elems = 0;
    me->capelems = 0;
}

/**
 * Create a vector dynamically
*/
SML_DVEC_TNAME *SML_DVEC_IMPLNAME(create)(void)
{
    SML_DVEC_TNAME *me = (SML_DVEC_TNAME *) malloc(sizeof(SML_DVEC_TNAME));
    assert(me != NULL);
    SML_DVEC_IMPLNAME(init)(me);
    return me;
}

/**
 * Free a dynamically created vector
*/
void SML_DVEC_IMPLNAME(free)(SML_DVEC_TNAME *me)
{
    if (me) {
        SML_DVEC_IMPLNAME(destroy)(me);
        free(me);
    }
}

/**
 * Clear a vector
*/
void SML_DVEC_IMPLNAME(clear)(SML_DVEC_TNAME *me)
{
    me->elems = 0;
}

/**
 * Grow the vector by the standard ratio
*/
void SML_DVEC_IMPLNAME(grow)(SML_DVEC_TNAME *me)
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
void SML_DVEC_IMPLNAME(resize)(SML_DVEC_TNAME *me, size_t cnt)
{
    me->mem = (SML_DVEC_T *)realloc(me->mem, cnt * sizeof(*me->mem));
    assert(me->mem);
    me->elems = cnt;
    me->capelems = cnt;
}

/**
 * Reserve enough size for cnt elements
 * Does nothing if cnt <= capacity
*/
void SML_DVEC_IMPLNAME(reserve)(SML_DVEC_TNAME *me, size_t cnt)
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
void SML_DVEC_IMPLNAME(push_back)(SML_DVEC_TNAME *me, SML_DVEC_T val)
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
void SML_DVEC_IMPLNAME(push_back_block)(SML_DVEC_TNAME *me, const SML_DVEC_T *data, size_t cnt)
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
void SML_DVEC_IMPLNAME(set_block)(SML_DVEC_TNAME *me, size_t idx, const SML_DVEC_T *data, size_t cnt)
{
    memcpy(me->mem + idx, data, cnt * sizeof(*me->mem));
}

/**
 * @brief Fill the vector up to its current size with elements val
 * 
 */
void SML_DVEC_IMPLNAME(fill)(SML_DVEC_TNAME *me, SML_DVEC_T val)
{
    for (size_t i = 0; i < me->elems; ++i) {
        me->mem[i] = val;
    }
}

/**
 * @brief Fill the vector up to its current size with raw bytes byteVal
 * 
 */
void SML_DVEC_IMPLNAME(fillBytes)(SML_DVEC_TNAME *me, unsigned char byteVal)
{
    memset(&me->mem, byteVal, sizeof(*me->mem) * me->elems);
}

#undef SML_DVEC_IDENT