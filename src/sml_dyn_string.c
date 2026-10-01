#include <stdlib.h>
#include <string.h>

#include "SML/sml_common.h"
#include "SML/sml_dyn_string.h"

#define SML_DSTRING_INITIAL_CAPACITY 1
#define SML_DSTRING_GROWTH_FACTOR_NUM 3
#define SML_DSTRING_GROWTH_FACTOR_DEN 2

static inline bool sML_DString_initWithCapacity(SML_DString *me, size_t capacity);
static inline bool sML_DString_growTo(SML_DString *me, size_t capacity);
static inline bool sML_DString_growToAtLeast(SML_DString *me, size_t capacity);
static inline bool sML_DString_grow(SML_DString *me);
static inline bool sML_DString_appendSpan(SML_DString *me, const char *str, size_t len);
static inline bool sML_DString_insertSpan(SML_DString *me, size_t pos, const char *str, size_t len);
static inline bool sML_DString_assignSpan(SML_DString *me, const char *str, size_t len);

static inline bool sML_DString_initWithCapacity(SML_DString *me, size_t capacity)
{
    me->begin = (char *)malloc((capacity + 1) * sizeof(*me->begin));
    if (!me->begin) {
        return false;
    }
    me->end = me->begin;
    *me->end = '\0';
    me->capacity = capacity;
    return true;
}

static inline bool sML_DString_growTo(SML_DString *me, size_t capacity)
{
    char *p = (char *)realloc(me->begin, (capacity + 1) * sizeof(*me->begin));
    if (!p) {
        return false;
    }
    me->end = p + (me->end - me->begin);
    me->begin = p;
    me->capacity = capacity;
    return true;
}

static inline bool sML_DString_growToAtLeast(SML_DString *me, size_t capacity)
{
    size_t newCap = me->end - me->begin;
    while (newCap < capacity) {
        newCap = SML_DSTRING_GROWTH_FACTOR_NUM * newCap / SML_DSTRING_GROWTH_FACTOR_DEN + (newCap < 2);
    }
    return sML_DString_growTo(me, newCap);
}

static inline bool sML_DString_grow(SML_DString *me)
{
    size_t newCap = SML_DSTRING_GROWTH_FACTOR_NUM * me->capacity / SML_DSTRING_GROWTH_FACTOR_DEN + (me->capacity < 2);
    return sML_DString_growTo(me, newCap);
}

static inline bool sML_DString_appendSpan(SML_DString *me, const char *str, size_t len)
{
    size_t reqCap = (me->end - me->begin) + len;
    if (reqCap > me->capacity) {
        if (!sML_DString_growToAtLeast(me, reqCap)) {
            return false;
        }
    }
    memcpy(me->end, str, len);
    me->end += len;
    *me->end = '\0';
    return true;
}

static inline bool sML_DString_insertSpan(SML_DString *me, size_t pos, const char *str, size_t len)
{
    size_t size = me->end - me->begin;
    if (pos > size) {
        return false;
    }
    size_t reqCap = size + len;
    if (reqCap > me->capacity) {
        if (!sML_DString_growToAtLeast(me, reqCap)) {
            return false;
        }
    }
    /* move trailing end of original string to the right */
    memmove(me->begin + pos + len, me->begin + pos, size - pos);
    /* insert new string */
    memcpy(me->begin + pos, str, len);
    me->end += len;
    *me->end = '\0';
    return true;
}

static inline bool sML_DString_assignSpan(SML_DString *me, const char *str, size_t len)
{
    if (len > me->capacity) {
        if (!sML_DString_growToAtLeast(me, len)) {
            return false;
        }
    }
    memcpy(me->begin, str, len);
    me->end = me->begin + len;
    *me->end = '\0';
    return true;
}

SML_DString* SML_DString_create(void)
{
    SML_DString *me;
    me = (SML_DString *)malloc(sizeof(*me));
    if (!me) {
        return NULL;
    }
    if (!sML_DString_initWithCapacity(me, SML_DSTRING_INITIAL_CAPACITY)) {
        free(me);
        return NULL;
    }
    return me;
}

SML_DString* SML_DString_createWithCapacity(size_t capacity)
{
    SML_DString *me;
    me = (SML_DString *)malloc(sizeof(*me));
    if (!me) {
        return NULL;
    }
    if (!sML_DString_initWithCapacity(me, capacity)) {
        free(me);
        return NULL;
    }
    return me;
}

SML_DString* SML_DString_createFrom(const char *src)
{
    SML_DString *me;
    me = (SML_DString *)malloc(sizeof(*me));
    if (!me) {
        return NULL;
    }
    if (!SML_DString_initFrom(me, src)) {
        free(me);
        return NULL;
    }
    return me;
}

SML_DString* SML_DString_createFromSpan(const char *src, size_t len)
{
    SML_DString *me;
    me = (SML_DString *)malloc(sizeof(*me));
    if (!me) {
        return NULL;
    }
    if (!SML_DString_initFromSpan(me, src, len)) {
        free(me);
        return NULL;
    }
    return me;
}

bool SML_DString_init(SML_DString *me)
{
    return sML_DString_initWithCapacity(me, SML_DSTRING_INITIAL_CAPACITY);
}

bool SML_DString_initWithCapacity(SML_DString *me, size_t capacity)
{
    return sML_DString_initWithCapacity(me, capacity);
}

bool SML_DString_initFrom(SML_DString *me, const char *src)
{
    size_t len = strlen(src);
    return SML_DString_initFromSpan(me, src, len);
}

bool SML_DString_initFromSpan(SML_DString *me, const char *src, size_t len)
{
    size_t cap = len;
    if (cap < SML_DSTRING_INITIAL_CAPACITY) {
        cap = SML_DSTRING_INITIAL_CAPACITY;
    }
    me->begin = (char *)malloc((cap + 1) * sizeof(*me->begin));
    if (!me->begin) {
        return false;
    }
    me->end = me->begin + len;
    memcpy(me->begin, src, len);
    *me->end = '\0';
    me->capacity = cap;
    return true;
}

void SML_DString_destroy(SML_DString *me)
{
    free(me->begin);
    SML_ZEROP(me);
}

void SML_DString_free(SML_DString *me)
{
    if (me) {
        SML_DString_destroy(me);
        free(me);
    }
}

void SML_DString_fill(SML_DString *me, char val)
{
    for (char *it = me->begin; it != me->end; ++it) {
        *it = val;
    }
}

bool SML_DString_push_back(SML_DString *me, char val)
{
    if (me->end - me->begin >= me->capacity) {
        if (!sML_DString_grow(me)) {
            return false;
        }
    }
    *me->end++ = val;
    *me->end = '\0';
    return true;
}

bool SML_DString_append(SML_DString *me, const char *str)
{
    size_t len = strlen(str);
    return sML_DString_appendSpan(me, str, len);
}

bool SML_DString_appendSpan(SML_DString *me, const char *str, size_t len)
{
    return sML_DString_appendSpan(me, str, len);
}

bool SML_DString_insert(SML_DString *me, size_t pos, const char *str)
{
    size_t len = strlen(str);
    return sML_DString_insertSpan(me, pos, str, len);
}

bool SML_DString_insertSpan(SML_DString *me, size_t pos, const char *str, size_t len)
{
    return sML_DString_insertSpan(me, pos, str, len);
}

void SML_DString_clear(SML_DString *me)
{
    me->end = me->begin;
    *me->end = '\0';
}

bool SML_DString_assign(SML_DString *me, const char *str)
{
    size_t len = strlen(str);
    return sML_DString_assignSpan(me, str, len);
}

bool SML_DString_assignSpan(SML_DString *me, const char *str, size_t len)
{
    return sML_DString_assignSpan(me, str, len);
}

bool SML_DString_resize(SML_DString *me, size_t count)
{
    if (count == me->capacity) {
        return true;
    }
    if (!sML_DString_growTo(me, count)) {
        return false;
    }
    /* resize and re-terminate */
    me->end = me->begin + me->capacity;
    *me->end = '\0';
    return true;
}

bool SML_DString_reserve(SML_DString *me, size_t capacity)
{
    if (capacity < me->capacity) {
        return true;
    }
    return sML_DString_growTo(me, capacity);
}