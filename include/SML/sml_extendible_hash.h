#ifndef INCLUDE_SML_EXTENDIBLE_HASH_H
#define INCLUDE_SML_EXTENDIBLE_HASH_H

#include <stddef.h>
#include <stdbool.h>
#include <stdint.h>
#include <limits.h>

#include "SML/sml_string.h"
#include "SML/sml_common.h"

/*
 * Implement a dynamic stack of type uint32_t - SML inception :)
 */
#ifndef SML_DStack_u32EHashMap_IMPL
#define SML_DStack_u32EHashMap_IMPL
#define SML_DSTACK_T uint32_t
#define SML_DSTACK_ID u32EHashMap
#include "SML/sml_dyn_stack.h"
#undef SML_DSTACK_ID
#undef SML_DSTACK_T
#endif /* SML_DStack_u32EHashMap_IMPL */ 

#define SML_EHASH_INITIAL_ITEM_CAPACITY 1
#define SML_EHASH_INITIAL_BUCKET_DEPTH  1
#define SML_EHASH_MAX_BUCKET_SIZE       3
#define SML_EHASH_MAX_ITEM_COUNT        (UINT32_MAX - 1)

/* -1 to not exceed 32bit item index with average of 2 items per bucket */
#define SML_EHASH_MAX_GLOBAL_DEPTH      ((8 * sizeof(uint32_t)) - 1)

#define SML_EHASH_KEYCLASS_FIXED      0
#define SML_EHASH_KEYCLASS_CSTRING    1
#define SML_EHASH_KEYCLASS_STRINGVIEW 2

/* type names */
#define SML_EHASH_GLUE_TNAME(type_, ident_) type_ ## _ ## ident_
#define SML_EHASH_TNAME(type_, ident_) SML_EHASH_GLUE_TNAME(type_, ident_)
#define SML_EHASH_T_TNAME SML_EHASH_TNAME(SML_EHashMap, SML_EHASH_IDENT)
#define SML_EHASH_ITER_TNAME SML_EHASH_TNAME(SML_EHashMapIter, SML_EHASH_IDENT)
#define SML_EHASH_ITEM_TNAME SML_EHASH_TNAME(SML_EHashMapItem, SML_EHASH_IDENT)
#define SML_EHASH_BUCKETENTRY_TNAME SML_EHASH_TNAME(SML_EHashMapBucketEntry, SML_EHASH_IDENT)

/* function names */
#define SML_EHASH_GLUE_FNAME(type_, ident_, name_) type_ ## _ ## ident_ ## _ ## name_
#define SML_EHASH_FNAME(type_, ident_, name_) SML_EHASH_GLUE_FNAME(type_, ident_, name_)
#define SML_EHASH_T_FNAME(name_) SML_EHASH_FNAME(SML_EHashMap, SML_EHASH_IDENT, name_)
#define SML_EHASH_ITER_FNAME(name_) SML_EHASH_FNAME(SML_EHashMapIter, SML_EHASH_IDENT, name_)

#endif /* INCLUDE_SML_EXTENDIBLE_HASH_H */

/* 
 * NOTE: The following is not protected by header 
 * guard and intended to be included multiple times
 */

#ifndef SML_EHASH_T
#error "SML_EHASH_T (typename) must be defined"
#endif

#ifndef SML_EHASH_KEYT
#error "SML_EHASH_KEYT (typename) must be defined"
#endif

#ifndef SML_EHASH_ISKEYSTRING
#error "SML_EHASH_ISKEYSTRING must be defined"
#endif

#if SML_EHASH_ISKEYSTRING
#ifndef SML_EHASH_ISKEYCSTRING
#error "SML_EHASH_ISKEYCSTRING must be defined if SML_EHASH_ISKEYSTRING is true"
#endif
#if SML_EHASH_ISKEYCSTRING
    #define SML_EHASH_KEYCLASS SML_EHASH_KEYCLASS_CSTRING
#else
    #define SML_EHASH_KEYCLASS SML_EHASH_KEYCLASS_STRINGVIEW
#endif
#else
    #define SML_EHASH_KEYCLASS SML_EHASH_KEYCLASS_FIXED
#endif

/* define unique template identifier */
#ifdef SML_EHASH_ID
#define SML_EHASH_IDENT SML_EHASH_ID
#else
#define SML_EHASH_IDENT SML_EHASH_TYPE(SML_EHASH_T, SML_EHASH_KEYT)
#endif

#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
typedef uint32_t (*SML_EHASH_T_FNAME(hash_fn))(const SML_EHASH_KEYT key, uint32_t size);
typedef bool (*SML_EHASH_T_FNAME(compare_fn))(const SML_EHASH_KEYT a, const SML_EHASH_KEYT b, uint32_t sizeA, uint32_t sizeB);
#else
typedef uint32_t (*SML_EHASH_T_FNAME(hash_fn))(const SML_EHASH_KEYT key);
typedef bool (*SML_EHASH_T_FNAME(compare_fn))(const SML_EHASH_KEYT a, const SML_EHASH_KEYT b);
#endif

/* 
 * Extendible hash map types declaration
 */

#ifdef __cplusplus
extern "C" {
#endif


typedef struct SML_EHASH_ITEM_TNAME {
    uint32_t next;                              /**< index in itemBuf of the next item */
    SML_EHASH_KEYT key;                         /**< item key */
#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
    uint32_t keySize;                           /**< item key size */
#endif
    SML_EHASH_T value;                          /**< item value */
    uint32_t hash;                              /**< item key hash */
} SML_EHASH_ITEM_TNAME;

typedef struct SML_EHASH_BUCKETENTRY_TNAME {
    uint32_t first;                             /**< index in itemBuf of the first item in this bucket */
    uint16_t bucketSize;                        /**< number of itmes in this bucket */
    uint16_t bucketDepth;                       /**< bit depth of this bucket */
}  SML_EHASH_BUCKETENTRY_TNAME;

typedef struct SML_EHASH_T_TNAME {                           
    SML_EHASH_T_FNAME(hash_fn) hash_fn;        /**< hash function pointer */
    SML_EHASH_T_FNAME(compare_fn) compare_fn;  /**< compare function pointer */
    SML_EHASH_BUCKETENTRY_TNAME *buckets;      /**< array of buckets */
    SML_EHASH_ITEM_TNAME *itemBuf;             /**< array of items */    
    SML_DStack_u32EHashMap itemFreeList;       /**< free list containing free indices into itemBuf */                     
    uint32_t *directory;                       /**< global directory, contains indices into "buckets" */
    uint32_t globalDepth;                      /**< bit depth of "directory" */                    
    uint32_t itemCount;                        /**< total number of items currently used */
    uint32_t itemCapacity;                     /**< total number of items currently allocated */
    uint32_t bucketCount;                      /**< total number of buckets currently used */
    uint32_t bucketCapacity;                   /**< total number of buckets currently allocated */              
} SML_EHASH_T_TNAME; 

typedef struct SML_EHASH_ITER_TNAME {
    const SML_EHASH_T_TNAME *map;               /**< the map this iterator belongs to */
    SML_EHASH_ITEM_TNAME *item;                 /**< current item */
    uint32_t bucketIdx;                         /**< current bucket index */
} SML_EHASH_ITER_TNAME;

static SML_EHASH_T_TNAME * SML_EHASH_T_FNAME(create)(SML_EHASH_T_FNAME(hash_fn) hash_fn, SML_EHASH_T_FNAME(compare_fn) compare_fn);
static void          SML_EHASH_T_FNAME(free)(SML_EHASH_T_TNAME *me);
static void          SML_EHASH_T_FNAME(destroy)(SML_EHASH_T_TNAME *me);
static bool          SML_EHASH_T_FNAME(init)(SML_EHASH_T_TNAME *me, SML_EHASH_T_FNAME(hash_fn) hash_fn, SML_EHASH_T_FNAME(compare_fn) compare_fn);
static bool          SML_EHASH_T_FNAME(initWithDepth)(SML_EHASH_T_TNAME *me, SML_EHASH_T_FNAME(hash_fn) hash_fn, SML_EHASH_T_FNAME(compare_fn) compare_fn, uint32_t bucketDepth);
static void          SML_EHASH_T_FNAME(clear)(SML_EHASH_T_TNAME *me);
static bool          SML_EHASH_T_FNAME(expand)(SML_EHASH_T_TNAME *me, uint32_t hash, uint32_t dirIdxIn, uint32_t bucketIdxIn, uint32_t *bucketIdxOut);

#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
static bool          SML_EHASH_T_FNAME(insert)(SML_EHASH_T_TNAME *me, const SML_EHASH_KEYT key, uint32_t keySize, SML_EHASH_T value);
static bool          SML_EHASH_T_FNAME(get)(const SML_EHASH_T_TNAME *me, const SML_EHASH_KEYT key, uint32_t keySize, SML_EHASH_T *value);
static SML_EHASH_T*  SML_EHASH_T_FNAME(get_p)(const SML_EHASH_T_TNAME *me, const SML_EHASH_KEYT key, uint32_t keySize);
static void          SML_EHASH_T_FNAME(erase)(SML_EHASH_T_TNAME *me, const SML_EHASH_KEYT key, uint32_t keySize);

#else
static bool          SML_EHASH_T_FNAME(insert)(SML_EHASH_T_TNAME *me, const SML_EHASH_KEYT key, SML_EHASH_T value);
static bool          SML_EHASH_T_FNAME(get)(const SML_EHASH_T_TNAME *me, const SML_EHASH_KEYT key, SML_EHASH_T *value);
static SML_EHASH_T*  SML_EHASH_T_FNAME(get_p)(const SML_EHASH_T_TNAME *me, const SML_EHASH_KEYT key);
static void          SML_EHASH_T_FNAME(erase)(SML_EHASH_T_TNAME *me, const SML_EHASH_KEYT key);

#endif

static inline size_t SML_EHASH_T_FNAME(size)(const SML_EHASH_T_TNAME *me);
static inline bool SML_EHASH_T_FNAME(empty)(const SML_EHASH_T_TNAME *me);

static SML_EHASH_ITER_TNAME SML_EHASH_T_FNAME(begin)(const SML_EHASH_T_TNAME *me);
static void SML_EHASH_ITER_FNAME(next)(SML_EHASH_ITER_TNAME *me);
static bool SML_EHASH_ITER_FNAME(isEnd)(const SML_EHASH_ITER_TNAME *me);

static uint32_t sml_EHashMap_stdHash(const char *c);
static bool sml_EHashMap_stdCompare(const char *a, const char *b);
static uint32_t sml_EHashMap_stdViewHash(const char *c, uint32_t size);
static bool sml_EHashMap_stdViewCompare(const char *a, const char *b, uint32_t sizeA, uint32_t sizeB);

// static void print_table(UintEHashMap *t);
#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
static SML_EHASH_ITEM_TNAME * SML_EHASH_T_FNAME(createItemAndInsertFirst)(SML_EHASH_T_TNAME *me, const SML_EHASH_KEYT key, uint32_t keySize, SML_EHASH_T value, uint32_t hash, uint32_t bucketIdx);
#else
static SML_EHASH_ITEM_TNAME * SML_EHASH_T_FNAME(createItemAndInsertFirst)(SML_EHASH_T_TNAME *me, const SML_EHASH_KEYT key, SML_EHASH_T value, uint32_t hash, uint32_t bucketIdx);
#endif


static SML_EHASH_T_TNAME * SML_EHASH_T_FNAME(create)(SML_EHASH_T_FNAME(hash_fn) hash_fn, SML_EHASH_T_FNAME(compare_fn) compare_fn)
{
    SML_EHASH_T_TNAME *me = (SML_EHASH_T_TNAME *)malloc(sizeof(*me));
    if (!me) {
        return NULL;
    }
    if (!SML_EHASH_T_FNAME(init)(me, hash_fn, compare_fn)) {
        free(me);
        return NULL;
    }
    return me;
}

static bool SML_EHASH_T_FNAME(init)(SML_EHASH_T_TNAME *me, SML_EHASH_T_FNAME(hash_fn) hash_fn, SML_EHASH_T_FNAME(compare_fn) compare_fn)
{
    return SML_EHASH_T_FNAME(initWithDepth)(me, hash_fn, compare_fn, SML_EHASH_INITIAL_BUCKET_DEPTH);
}

static bool SML_EHASH_T_FNAME(initWithDepth)(SML_EHASH_T_TNAME *me, SML_EHASH_T_FNAME(hash_fn) hash_fn, SML_EHASH_T_FNAME(compare_fn) compare_fn, uint32_t bucketDepth)
{
    /* save hash function pointer, if key is string default can be used */
#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
    if (hash_fn) {
        me->hash_fn = hash_fn;
    } else {
        me->hash_fn = &sml_EHashMap_stdViewHash;
    }
#elif SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_CSTRING
    if (hash_fn) {
        me->hash_fn = hash_fn;
    } else {
        me->hash_fn = &sml_EHashMap_stdHash;
    }
#else
    if (!hash_fn) {
        return false;
    }
    me->hash_fn = hash_fn;
#endif /* SML_EHASH_ISKEYSTRING */

    /* save compare function pointer, if key is string, default (NULL) can be used */
#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
    if (compare_fn) {
        me->compare_fn = compare_fn;
    } else {
        me->compare_fn = &sml_EHashMap_stdViewCompare;
    }
#elif SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_CSTRING
    if (compare_fn) {
        me->compare_fn = compare_fn;
    } else {
        me->compare_fn = &sml_EHashMap_stdCompare;
    }
#else
    if (!compare_fn) {
        return false;
    }
    me->compare_fn = compare_fn;
#endif /* SML_EHASH_ISKEYSTRING */

    /* determine initial depth(s) */
    uint32_t globalDepth;
    if (bucketDepth < 1) {
        bucketDepth = 1;
        globalDepth = bucketDepth + 1;
    } else if (bucketDepth < SML_EHASH_MAX_GLOBAL_DEPTH) {
        globalDepth = bucketDepth + 1;
    } else if (bucketDepth == SML_EHASH_MAX_GLOBAL_DEPTH) {
        globalDepth = bucketDepth;
    } else {
        bucketDepth = SML_EHASH_MAX_GLOBAL_DEPTH;
        globalDepth = bucketDepth;
    }

    /* allocate initial directory */
    const uint32_t numDir    = (1U << globalDepth);
    const uint32_t numBucket = (1U << bucketDepth);

    me->directory = (uint32_t *)malloc(numDir * sizeof(*me->directory));
    if (me->directory == NULL) {
        goto err1;
    }
    for (uint32_t i = 0; i < numDir; ++i) {
        /* map directories to buckets */
        me->directory[i] = i & (numBucket - 1);
    }

    me->globalDepth = globalDepth;
    me->itemCount = 0;

    /* create array of buckets */
    me->buckets = (SML_EHASH_BUCKETENTRY_TNAME *)malloc(numBucket * sizeof(*me->buckets));
    if (me->buckets == NULL) {
        goto err2;
    }
    me->bucketCount = numBucket;
    me->bucketCapacity = numBucket;
    for (uint32_t i = 0; i < numBucket; ++i) {
        me->buckets[i].first = UINT32_MAX;
        me->buckets[i].bucketSize = 0;
        me->buckets[i].bucketDepth = bucketDepth;
    }

    /* create array of items */
    me->itemBuf = (SML_EHASH_ITEM_TNAME *)malloc(SML_EHASH_INITIAL_ITEM_CAPACITY * sizeof(*me->itemBuf));
    if (!me->itemBuf) {
        goto err3;
    }
    me->itemCapacity = SML_EHASH_INITIAL_ITEM_CAPACITY;

    /* init free list of items */
    SML_DStack_u32EHashMap_init(&me->itemFreeList);

    return true;

err3:
    free(me->itemBuf);
err2:
    free(me->buckets);
err1:
    free(me->directory);
    return false;
}

static void SML_EHASH_T_FNAME(free)(SML_EHASH_T_TNAME *me)
{
    if (!me) {
        return;
    }
    SML_EHASH_T_FNAME(destroy)(me);
    /* free the object itself */
    free(me);
}

static void SML_EHASH_T_FNAME(destroy)(SML_EHASH_T_TNAME *me)
{
    /* if key was allocated, go through buckets and free each item key */
#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_CSTRING || SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
    for (uint32_t i = 0; i < me->bucketCount; ++i) {
        uint32_t itemIdx = me->buckets[i].first;
        while (itemIdx != UINT32_MAX) {
            SML_EHASH_ITEM_TNAME *item = &me->itemBuf[itemIdx];
            itemIdx = item->next;
            free(item->key);
        }
    }
#endif /* SML_EHASH_ISKEYSTRING*/

    /* free the item free list */
    SML_DStack_u32EHashMap_destroy(&me->itemFreeList);
    /* free the itemBuf */
    free(me->itemBuf);
    /* free the buckets array */
    free(me->buckets);
    /* free the directory array */
    free(me->directory);
    /* zero everything */
    SML_ZEROP(me);
}

#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
static bool SML_EHASH_T_FNAME(insert)(SML_EHASH_T_TNAME *me, const SML_EHASH_KEYT key, uint32_t keySize, SML_EHASH_T value)
#else
static bool SML_EHASH_T_FNAME(insert)(SML_EHASH_T_TNAME *me, const SML_EHASH_KEYT key, SML_EHASH_T value)
#endif
{
#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
    if (key == NULL || keySize == 0) {
        return false;
    }
#endif /* SML_EHASH_KEYCLASS */
#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_CSTRING
    if (key == NULL) {
        return false;
    }
#endif /* SML_EHASH_KEYCLASS */

    uint32_t hash, dirIdx;
    uint32_t bucketIdx;

#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
    hash = (*me->hash_fn)(key, keySize);
#else
    hash = (*me->hash_fn)(key);
#endif
    dirIdx = hash & ((1U << me->globalDepth) - 1);
    bucketIdx = me->directory[dirIdx];

    /* try direct inserting */
    uint32_t itemIdx = me->buckets[bucketIdx].first;
    uint32_t chainLevel = 0;

    while(itemIdx != UINT32_MAX) {
        /* item already existing? -> overwrite */
        SML_EHASH_ITEM_TNAME *item = &me->itemBuf[itemIdx];
#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
        if (me->compare_fn(key, item->key, keySize, item->keySize))
#else
        if (me->compare_fn(key, item->key))
#endif
        {
            item->value = value;
            return true;
        }
        ++chainLevel;
        itemIdx = item->next;
    }

    /* still have more space in bucket? */
    if (chainLevel < SML_EHASH_MAX_BUCKET_SIZE) {
#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
        if (!SML_EHASH_T_FNAME(createItemAndInsertFirst)(me, key, keySize, value, hash, bucketIdx))
#else
        if (!SML_EHASH_T_FNAME(createItemAndInsertFirst)(me, key, value, hash, bucketIdx))
#endif
        {
            return false;
        }
        // print_table(me);
        return true;
    }

    /* bucket is full, expand */
    if (!SML_EHASH_T_FNAME(expand)(me, hash, dirIdx, bucketIdx, &bucketIdx)) {
        return false;
    }

    /* insert new item */
#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
    if (!SML_EHASH_T_FNAME(createItemAndInsertFirst)(me, key, keySize, value, hash, bucketIdx))
#else
    if (!SML_EHASH_T_FNAME(createItemAndInsertFirst)(me, key, value, hash, bucketIdx))
#endif
    {
        return false;
    }
    // print_table(me);
    return true;
}

#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
static bool SML_EHASH_T_FNAME(get)(const SML_EHASH_T_TNAME *me, const SML_EHASH_KEYT key, uint32_t keySize, SML_EHASH_T *value)
#else
static bool SML_EHASH_T_FNAME(get)(const SML_EHASH_T_TNAME *me, const SML_EHASH_KEYT key, SML_EHASH_T *value)
#endif
{
#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
    if (key == NULL || keySize == 0) {
        return false;
    }
#endif /* SML_EHASH_KEYCLASS */
#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_CSTRING
    if (key == NULL) {
        return false;
    }
#endif /* SML_EHASH_KEYCLASS */

    uint32_t hash, dirIdx;
    uint32_t bucketIdx;

#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
    hash = (*me->hash_fn)(key, keySize);
#else
    hash = (*me->hash_fn)(key);
#endif
    dirIdx = hash & ((1U << me->globalDepth) - 1);
    bucketIdx = me->directory[dirIdx];

    uint32_t itemIdx = me->buckets[bucketIdx].first;

    while(itemIdx != UINT32_MAX) {
        /* compare keys */
        SML_EHASH_ITEM_TNAME *item = &me->itemBuf[itemIdx];
#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
        if (me->compare_fn(key, item->key, keySize, item->keySize))
#else
        if (me->compare_fn(key, item->key))
#endif
        {
            *value = item->value;
            return true;
        }
        itemIdx = item->next;
    }

    /* item with this key does not exist */
    return false;
}

#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
static SML_EHASH_T*  SML_EHASH_T_FNAME(get_p)(const SML_EHASH_T_TNAME *me, const SML_EHASH_KEYT key, uint32_t keySize)
#else
static SML_EHASH_T* SML_EHASH_T_FNAME(get_p)(const SML_EHASH_T_TNAME *me, const SML_EHASH_KEYT key)
#endif
{
#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW || SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_CSTRING
    if (key == NULL) {
        return NULL;
    }
#endif /* SML_EHASH_KEYCLASS */

    uint32_t hash, dirIdx;
    uint32_t bucketIdx;

#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
    hash = (*me->hash_fn)(key, keySize);
#else
    hash = (*me->hash_fn)(key);
#endif
    dirIdx = hash & ((1U << me->globalDepth) - 1);
    bucketIdx = me->directory[dirIdx];

    uint32_t itemIdx = me->buckets[bucketIdx].first;

    while(itemIdx != UINT32_MAX) {
        /* compare keys */
        SML_EHASH_ITEM_TNAME *item = &me->itemBuf[itemIdx];
#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
        if (me->compare_fn(key, item->key, keySize, item->keySize))
#else
        if (me->compare_fn(key, item->key))
#endif
        {
            return &item->value;
        }
        itemIdx = item->next;
    }

    /* item with this key does not exist */
    return NULL;
}

#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
static void SML_EHASH_T_FNAME(erase)(SML_EHASH_T_TNAME *me, const SML_EHASH_KEYT key, uint32_t keySize)
#else
static void SML_EHASH_T_FNAME(erase)(SML_EHASH_T_TNAME *me, const SML_EHASH_KEYT key)
#endif
{
#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW || SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_CSTRING
    if (key == NULL) {
        return;
    }
#endif /* SML_EHASH_KEYCLASS */

    uint32_t hash, dirIdx;
    uint32_t bucketIdx;

#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
    hash = (*me->hash_fn)(key, keySize);
#else
    hash = (*me->hash_fn)(key);
#endif
    dirIdx = hash & ((1U << me->globalDepth) - 1);
    bucketIdx = me->directory[dirIdx];

    uint32_t itemIdx = me->buckets[bucketIdx].first;
    uint32_t *prevIdxPtr = &me->buckets[bucketIdx].first;

    while (itemIdx != UINT32_MAX) {
        SML_EHASH_ITEM_TNAME *item = &me->itemBuf[itemIdx];
#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
        if (me->compare_fn(key, item->key, keySize, item->keySize))
#else
        if (me->compare_fn(key, item->key))
#endif
        {
            /* item found, erase it */
            *prevIdxPtr = item->next;
            --me->buckets[bucketIdx].bucketSize;
            --me->itemCount;
#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW || SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_CSTRING
            free(item->key);
#endif
            /* push index onto free list */
            SML_DStack_u32EHashMap_push(&me->itemFreeList, itemIdx);
            return;
        }
        prevIdxPtr = &item->next;
        itemIdx = item->next;
    }
    /* arrive here, item not found */
}

/**
 * @brief Clear the EHashMap
 * 
 * Clears all items but keeps the rest of the internal structure
 */
static void SML_EHASH_T_FNAME(clear)(SML_EHASH_T_TNAME *me)
{
    /* loop through buckets and invalidate items */
    for (uint32_t i = 0; i < me->bucketCount; ++i) {
        /* if keys are allocated, free those */
#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW || SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_CSTRING
        uint32_t itemIdx = me->buckets[i].first;
        while (itemIdx != UINT32_MAX) {
            SML_EHASH_ITEM_TNAME *const item = &me->itemBuf[itemIdx];
            itemIdx = item->next;
            free(item->key);
        }
#endif
        me->buckets[i].first = UINT32_MAX;
        me->buckets[i].bucketSize = 0;
    }
    /* instead of pushing everything on the free list, only set itemCount to zero */
    me->itemCount = 0;
    SML_DStack_u32EHashMap_clear(&me->itemFreeList);
}

static bool SML_EHASH_T_FNAME(expand)(SML_EHASH_T_TNAME *me, uint32_t hash, uint32_t dirIdxIn, uint32_t bucketIdxIn, uint32_t *bucketIdxOut)
{
    /* starting values */
    uint32_t dirIdx = dirIdxIn;
    uint32_t bucketIdx = bucketIdxIn;
    /* recursively split bucket and/or grow directory until item with "hash" can be inserted */
    while (true) {
        while (true) {
            /* split the current bucket */
            if (!(me->buckets[bucketIdx].bucketDepth < me->globalDepth)) {
                /* before next bucket split we have to grow the global directory */
                break;
            }

            const uint32_t prevBucketDepth = me->buckets[bucketIdx].bucketDepth;

            /* increase bucket count by one, if necessary allocate larger array */
            if (me->bucketCount + 1 > me->bucketCapacity) {
                /* max capacity reached? */
                if (me->bucketCapacity == UINT32_MAX) {
                    return false;
                }
                uint64_t newCap = me->bucketCapacity * 3 / 2 + (me->bucketCapacity == 1);
                /* prevent overflow */
                if (newCap > UINT32_MAX) {
                    newCap = UINT32_MAX;
                }
                SML_EHASH_BUCKETENTRY_TNAME *p = (SML_EHASH_BUCKETENTRY_TNAME *)realloc(me->buckets, newCap * sizeof(*me->buckets));
                if (p == NULL) {
                    return false;
                }
                me->buckets = p;
                me->bucketCapacity = newCap;
            }
            SML_EHASH_BUCKETENTRY_TNAME *const origBucket = &me->buckets[bucketIdx];
            ++me->bucketCount;
            ++origBucket->bucketDepth;
            me->buckets[me->bucketCount - 1].first = UINT32_MAX;
            me->buckets[me->bucketCount - 1].bucketSize = 0;
            me->buckets[me->bucketCount - 1].bucketDepth = origBucket->bucketDepth;

            /* set the relevant directories to the new bucket (the ones with the "1" in the next higher bit) */
            const uint32_t stride = 1U << (prevBucketDepth + 1);
            const uint32_t offset = (stride >> 1) | (dirIdx & ((stride >> 1) - 1));

            for (uint32_t i = offset; i < (1U << me->globalDepth); i += stride) {
                me->directory[i] = me->bucketCount - 1;
            }

            SML_EHASH_BUCKETENTRY_TNAME *const newBucket = &me->buckets[me->bucketCount - 1];

            // TODO: do this with less local variables
            uint32_t itemIdx = origBucket->first;
            uint32_t *prevIdxPtr = &origBucket->first;

            /* loop through bucket linked list and move items if necessary */
            while(itemIdx != UINT32_MAX) {
                SML_EHASH_ITEM_TNAME *item = &me->itemBuf[itemIdx];
                /* save for later */
                uint32_t nextIdx = item->next;

                /* test if item needs to be moved */
                if (item->hash & (1 << ((uint32_t)origBucket->bucketDepth - 1))) {
                    *prevIdxPtr = item->next;
                    --origBucket->bucketSize;
                    item->next = newBucket->first;
                    newBucket->first = itemIdx;
                    ++newBucket->bucketSize;
                } else {
                    prevIdxPtr = &item->next;
                }
                itemIdx = nextIdx;
            }

            /* bucket index could have changed due to some of the directories pointing to new bucket */
            bucketIdx = me->directory[dirIdx];

            if (me->buckets[bucketIdx].bucketSize < SML_EHASH_MAX_BUCKET_SIZE) {
                /* success, enough room to insert item */
                *bucketIdxOut = bucketIdx;
                return true;
            }
            /* else: split bucket again */
        }

        /* check if the directory can be grown */
        if (me->globalDepth >= SML_EHASH_MAX_GLOBAL_DEPTH) {
            /* failure, directory depth is at its maximum */
            return false;
        }

        /* grow the directory */
        uint32_t *p = (uint32_t *)realloc(me->directory, (1UL << (me->globalDepth + 1)) * sizeof(*me->directory));
        if (!p) {
            return false;
        }
        me->directory = p;
        ++me->globalDepth;

        /* duplicate first half of the directory to the second half */
        for (uint32_t i = 0; i < (1U << (me->globalDepth - 1)); ++i) {
            me->directory[i + (1U << (me->globalDepth - 1))] = me->directory[i];
        }

        /* directory index could have changed due to leading 1 that is now being masked */
        dirIdx = hash & ((1U << me->globalDepth) - 1);
        /* bucketIdx stays the same since new half of directory is a duplicate */
    }
    /* should never be reached */
    return false;
}

static SML_EHASH_ITER_TNAME SML_EHASH_T_FNAME(begin)(const SML_EHASH_T_TNAME *me)
{
    /* map is empty? -> return the end iterator */
    if (SML_EHASH_T_FNAME(empty)(me)) {
        return (SML_EHASH_ITER_TNAME){ me, &me->itemBuf[me->itemCount], me->bucketCount };
    }

    /* serach the for the first item */
    uint32_t bucketIdx = 0;
    for ( ; bucketIdx < me->bucketCount; ++bucketIdx) {
        if (me->buckets[bucketIdx].first != UINT32_MAX)
            break;
    }
    return (SML_EHASH_ITER_TNAME){ me, &me->itemBuf[me->buckets[bucketIdx].first], bucketIdx };
}

static void SML_EHASH_ITER_FNAME(next)(SML_EHASH_ITER_TNAME *me)
{
    /* try the next item in the same bucket */
    if (me->item->next != UINT32_MAX) {
        me->item = &me->map->itemBuf[me->item->next];
        return;
    }

    /* move to the next bucket(s) */
    for (uint32_t bucketIdx = ++me->bucketIdx; bucketIdx < me->map->bucketCount; ++bucketIdx) {
        const uint32_t idx = me->map->buckets[bucketIdx].first;
        if (idx != UINT32_MAX) {
            me->bucketIdx = bucketIdx;
            me->item = &me->map->itemBuf[idx];
            return;
        }
    }

    /* arrive here, no more entries */
    me->item = &me->map->itemBuf[me->map->itemCount];
}

static bool SML_EHASH_ITER_FNAME(isEnd)(const SML_EHASH_ITER_TNAME *me)
{
    if (me->item == &me->map->itemBuf[me->map->itemCount]) {
        return true;
    } else {
        return false;
    }
}

#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
static SML_EHASH_ITEM_TNAME * SML_EHASH_T_FNAME(createItemAndInsertFirst)(SML_EHASH_T_TNAME *me, const SML_EHASH_KEYT key, uint32_t keySize, SML_EHASH_T value, uint32_t hash, uint32_t bucketIdx)
#else
static SML_EHASH_ITEM_TNAME * SML_EHASH_T_FNAME(createItemAndInsertFirst)(SML_EHASH_T_TNAME *me, const SML_EHASH_KEYT key, SML_EHASH_T value, uint32_t hash, uint32_t bucketIdx)
#endif
{
    uint32_t idx;
    bool fromFreeList;

    if (SML_DStack_u32EHashMap_empty(&me->itemFreeList)) {
        /* append new item */
        if (me->itemCount + 1 > me->itemCapacity) {
            /* max capacity reached? */
            if (me->itemCapacity == SML_EHASH_MAX_ITEM_COUNT) {
                return false;
            }
            /* reallocate buffer */
            uint64_t newCap = me->itemCapacity * 3 / 2 + (me->itemCapacity == 1);
            /* prevent overflow */
            if (newCap > SML_EHASH_MAX_ITEM_COUNT) {
                newCap = SML_EHASH_MAX_ITEM_COUNT;
            }
            SML_EHASH_ITEM_TNAME *p = (SML_EHASH_ITEM_TNAME *)realloc(me->itemBuf, newCap * sizeof(*me->itemBuf));
            if (!p) {
                return NULL;
            }
            me->itemBuf = p;
            me->itemCapacity = newCap;
        }
        idx = me->itemCount;
        fromFreeList = false;
    } else {
        /* use existing free spot */
        idx = SML_DStack_u32EHashMap_back(&me->itemFreeList);
        fromFreeList = true;
    }

    /* get the item pointer */
    SML_EHASH_ITEM_TNAME *const item = &me->itemBuf[idx];

    /* set item value */
#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
    item->key = SML_strviewdup(key, keySize);
    item->keySize = keySize;
#elif SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_CSTRING
    item->key = SML_strdup(key);
#else 
    /* cast away const if key is passed as value */
    item->key = (SML_EHASH_KEYT)key;
#endif /* SML_EHASH_ISKEYSTRING*/

    item->value = value;
    item->hash = hash;

    uint32_t firstIdx = me->buckets[bucketIdx].first;
    item->next = firstIdx;
    me->buckets[bucketIdx].first = idx;
    ++me->buckets[bucketIdx].bucketSize;
    ++me->itemCount;

    if (fromFreeList) {
        SML_DStack_u32EHashMap_pop(&me->itemFreeList);
    }

    return item;
}

// static void print_table(UintEHashMap *t) {
//     LOGINFO(">>>");
//     for (unsigned int i = 0; i < (1U << t->globalDepth); ++i) {
//         char dir[33], hash[33];
//         char itemsStr[1024]; // NOTE: this is unsafe
//         UintEHashMapBucketEntry *b = &t->buckets[t->directory[i]];
//         uinttobin(i, dir, t->globalDepth);

//         if (b->first) {
//             UintEHashMapItem *item = b->first;
//             char *to = itemsStr;

//             while (item != NULL) {
//                 uinttobin(item->hash, hash, b->bucketDepth);
//                 to = stringypcpy(to, "[");
//                 to = stringypcpy(to, item->key);
//                 to = stringypcpy(to, "...");
//                 to = stringypcpy(to, hash);
//                 to = stringypcpy(to, "] ");
//                 item = item->next;
//             }
//         } else {
//             strcpy(itemsStr, "NULL");
//         }
//         LOGINFO("%s -> [%u] %s", dir, t->directory[i], itemsStr);
//     }
//     LOGINFO("<<<");
// }

/* only define non-templated functions once */
#if !defined(SML_EHASH_LOCAL_ONCE) && SML_EHASH_ISKEYSTRING
#define SML_EHASH_LOCAL_ONCE

/**
 * @brief Default hash function for C-string keys in SML_EHashMap
 * 
 * Computes the 32bit FNV-1a (Fowler/Noll/Vo) hash
 * 
 * @param c Input string
 * @return uint32_t hash result
 */
static uint32_t sml_EHashMap_stdHash(const char *c)
{
    uint32_t hash = 2166136261U;
    while (*c) {
        hash ^= (uint32_t)*c++;
        hash *= 16777619U;
    }
    return hash;
}

/**
 * @brief Default compare function for C-string keys in SML_EHashMap
 * 
 * @param a Input string a
 * @param b Input string b
 * @return true Strings are the same
 * @return false Strings are not the same
 */
static bool sml_EHashMap_stdCompare(const char *a, const char *b)
{
    return !strcmp(a, b);
}

/**
 * @brief Default hash function for string view keys in SML_EHashMap
 * 
 * Computes the 32bit FNV-1a (Fowler/Noll/Vo) hash
 * 
 * @param c Input string pointer 
 * @param size Input string length
 * @return uint32_t hash result
 */
static uint32_t sml_EHashMap_stdViewHash(const char *c, uint32_t size)
{
    uint8_t *cur = (uint8_t *)c;
    uint8_t *end = (uint8_t *)c + size;
    uint32_t hash = 2166136261U;
    while (cur != end) {
        hash ^= (uint32_t)*cur++;
        hash *= 16777619U;
    }
    return hash;
}

/**
 * @brief Default compare functions for string view keys in SML_EHashMap
 * 
 * @param a Input string pointer a
 * @param b Input string pointer b
 * @param sizeA Length of string a
 * @param sizeB Length of string b
 * @return true Strings are the same
 * @return false Strings are not the same
 */
static bool sml_EHashMap_stdViewCompare(const char *a, const char *b, uint32_t sizeA, uint32_t sizeB)
{
    /* cannot be equal if sizes are different */
    if (sizeA != sizeB)
        return false;
    /* use memcmp instead of strncmp since it should be faster */
    return !memcmp(a, b, sizeA);
}

#endif /* SML_EHASH_LOCAL_ONCE */



/**
 * @brief Return the current number of items in the hash map
 * 
 */
static inline size_t SML_EHASH_T_FNAME(size)(const SML_EHASH_T_TNAME *me)
{
    return me->itemCount;
}

/**
 * @brief Return true if the hash map is empty
 * 
 */
static inline bool SML_EHASH_T_FNAME(empty)(const SML_EHASH_T_TNAME *me)
{
    return me->itemCount == 0;
}

#ifdef __cplusplus
}
#endif

#undef SML_EHASH_KEYCLASS
#undef SML_EHASH_IDENT
