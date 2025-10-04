#ifndef INCLUDE_SML_EXTENDIBLE_HASH_H
#define INCLUDE_SML_EXTENDIBLE_HASH_H

#include <stdbool.h>
#include <stdint.h>

/*
 * Implement a dynamic queue of type unsigned int 
 */
#ifndef SML_DQueue_uint4EHashMap_IMPL
#define SML_DQueue_uint4EHashMap_IMPL
#define SML_DQUEUE_T unsigned int
#define SML_DQUEUE_ID uint4EHashMap
#include "SML/sml_dyn_queue.h"
#undef SML_DQUEUE_ID
#undef SML_DQUEUE_T
#endif /* SML_DQueue_uint4EHashMap_IMPL */ 

#define SML_EHASH_INITIAL_ITEM_CAPACITY 1
// #define SML_EHASH_INITIAL_GLOBAL_DEPTH 3
#define SML_EHASH_INITIAL_BUCKET_DEPTH  1
#define SML_EHASH_MAX_BUCKET_SIZE       2
#define SML_EHASH_MAX_GLOBAL_DEPTH      (8 * sizeof(uint32_t))

#define SML_EHASH_KEYCLASS_FIXED      0
#define SML_EHASH_KEYCLASS_CSTRING    1
#define SML_EHASH_KEYCLASS_STRINGVIEW 2

/* function names */
#define SML_EHASH_GLUE_IMPL(type, name) SML_EHashMap_ ## type ## _ ## name
#define SML_EHASH_IMPL(type, name) SML_EHASH_GLUE_IMPL(type, name)
#define SML_EHASH_IMPLNAME(name) SML_EHASH_IMPL(SML_EHASH_IDENT, name)
/* type names */
#define SML_EHASH_GLUE_TYPE(ehashtype, type) ehashtype ## _ ## type
#define SML_EHASH_TYPE(ehashtype, type) SML_EHASH_GLUE_TYPE(ehashtype, type)
/* final type names */
#define SML_EHASH_TNAME SML_EHASH_TYPE(SML_EHashMap, SML_EHASH_IDENT)
#define SML_EHASH_ITEMNAME SML_EHASH_TYPE(SML_EHashMapItem, SML_EHASH_IDENT)
#define SML_EHASH_BUCKETENTRYNAME SML_EHASH_TYPE(SML_EHashMapBucketEntry, SML_EHASH_IDENT)
#define SML_EHASH_ITERATORNAME SML_EHASH_TYPE(SML_EHashMapIterator, SML_EHASH_IDENT)

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
typedef uint32_t (*SML_EHASH_IMPLNAME(hash_fn))(const SML_EHASH_KEYT key, unsigned int size);
typedef bool (*SML_EHASH_IMPLNAME(compare_fn))(const SML_EHASH_KEYT a, const SML_EHASH_KEYT b, unsigned int size);
#else
typedef uint32_t (*SML_EHASH_IMPLNAME(hash_fn))(const SML_EHASH_KEYT key);
typedef bool (*SML_EHASH_IMPLNAME(compare_fn))(const SML_EHASH_KEYT a, const SML_EHASH_KEYT b);
#endif

/* 
 * Extendible hash map types declaration
 */

#ifdef __cplusplus
extern "C" {
#endif


typedef struct SML_EHASH_ITEMNAME {
    unsigned int next;                          /**< index in itemBuf of the next item */
    SML_EHASH_KEYT key;                         /**< item key */
#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
    unsigned int keySize;                       /**< item key size */
#endif
    SML_EHASH_T data;                           /**< item value */
    uint32_t hash;                              /**< item key hash */
} SML_EHASH_ITEMNAME;

typedef struct SML_EHASH_BUCKETENTRYNAME {
    unsigned int first;                         /**< pointer to the first item in this bucket */
    uint16_t bucketSize;                        /**< number of itmes in this bucket */
    uint16_t bucketDepth;                       /**< bit depth of this bucket */
}  SML_EHASH_BUCKETENTRYNAME;

typedef struct SML_EHASH_TNAME {                           
    SML_EHASH_IMPLNAME(hash_fn) hash_fn;        /**< hash function pointer */
    SML_EHASH_IMPLNAME(compare_fn) compare_fn;  /**< compare function pointer */
    SML_EHASH_BUCKETENTRYNAME *buckets;         /**< array of buckets */
    SML_EHASH_ITEMNAME *itemBuf;                /**< array of items */    
    SML_DQueue_uint4EHashMap itemFreeList;      /**< free list containing free indices into itemBuf */                     
    unsigned int *directory;                    /**< global directory, contains indices into "buckets" */
    unsigned int globalDepth;                   /**< bit depth of "directory" */                    
    unsigned int numEntries;                    /**< total number of items currently used */
    unsigned int capacityEntries;               /**< total number of items currently allocated */
    unsigned int numBuckets;                    /**< total number of buckets currently used */
    unsigned int capacityBuckets;               /**< total number of buckets currently allocated */              
} SML_EHASH_TNAME; 

typedef struct SML_EHASH_ITERATORNAME {
    SML_EHASH_ITEMNAME *item;
    unsigned int curBucketIdx;
} SML_EHASH_ITERATORNAME;

static SML_EHASH_TNAME * SML_EHASH_IMPLNAME(create)(SML_EHASH_IMPLNAME(hash_fn) hash_fn, SML_EHASH_IMPLNAME(compare_fn) compare_fn);
static void          SML_EHASH_IMPLNAME(free)(SML_EHASH_TNAME *me);
static void          SML_EHASH_IMPLNAME(destroy)(SML_EHASH_TNAME *me);
static bool          SML_EHASH_IMPLNAME(init)(SML_EHASH_TNAME *me, SML_EHASH_IMPLNAME(hash_fn) hash_fn, SML_EHASH_IMPLNAME(compare_fn) compare_fn);
static bool          SML_EHASH_IMPLNAME(initWithDepth)(SML_EHASH_TNAME *me, SML_EHASH_IMPLNAME(hash_fn) hash_fn, SML_EHASH_IMPLNAME(compare_fn) compare_fn, unsigned int bucketDepth);
static void          SML_EHASH_IMPLNAME(clear)(SML_EHASH_TNAME *me);
static bool          SML_EHASH_IMPLNAME(expand)(SML_EHASH_TNAME *me, uint32_t hash);

#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
static bool          SML_EHASH_IMPLNAME(insert)(SML_EHASH_TNAME *me, const SML_EHASH_KEYT key, unsigned int keySize, SML_EHASH_T data);
static bool          SML_EHASH_IMPLNAME(get)(SML_EHASH_TNAME *me, const SML_EHASH_KEYT key, unsigned int keySize, SML_EHASH_T *data);
static void          SML_EHASH_IMPLNAME(erase)(SML_EHASH_TNAME *me, const SML_EHASH_KEYT key, unsigned int keySize);

#else
static bool          SML_EHASH_IMPLNAME(insert)(SML_EHASH_TNAME *me, const SML_EHASH_KEYT key, SML_EHASH_T data);
static bool          SML_EHASH_IMPLNAME(get)(SML_EHASH_TNAME *me, const SML_EHASH_KEYT key, SML_EHASH_T *data);
static void          SML_EHASH_IMPLNAME(erase)(SML_EHASH_TNAME *me, const SML_EHASH_KEYT key);

#endif

static inline unsigned int SML_EHASH_IMPLNAME(size)(const SML_EHASH_TNAME *me);
static inline bool SML_EHASH_IMPLNAME(empty)(const SML_EHASH_TNAME *me);

static SML_EHASH_ITERATORNAME SML_EHASH_IMPLNAME(firstIt)(SML_EHASH_TNAME *me);
static void                   SML_EHASH_IMPLNAME(nextIt)(SML_EHASH_TNAME *me, SML_EHASH_ITERATORNAME *it);
static bool                   SML_EHASH_IMPLNAME(isEndIt)(SML_EHASH_TNAME *me, SML_EHASH_ITERATORNAME *it);

static uint32_t std_hash_fn(const char *c);
static bool std_compare_fn(const char *a, const char *b);
static uint32_t std_view_hash_fn(const char *c, unsigned int size);
static bool std_view_compare_fn(const char *a, const char *b, unsigned int size);

// static void print_table(UintEHashMap *t);
#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
static SML_EHASH_ITEMNAME * SML_EHASH_IMPLNAME(createItemAndInsertFirst)(SML_EHASH_TNAME *me, const SML_EHASH_KEYT key, unsigned int keySize, SML_EHASH_T data, uint32_t hash, unsigned int bucketIdx);
#else
static SML_EHASH_ITEMNAME * SML_EHASH_IMPLNAME(createItemAndInsertFirst)(SML_EHASH_TNAME *me, const SML_EHASH_KEYT key, SML_EHASH_T data, uint32_t hash, unsigned int bucketIdx);
#endif


static SML_EHASH_TNAME * SML_EHASH_IMPLNAME(create)(SML_EHASH_IMPLNAME(hash_fn) hash_fn, SML_EHASH_IMPLNAME(compare_fn) compare_fn)
{
    SML_EHASH_TNAME *me = (SML_EHASH_TNAME *)malloc(sizeof(*me));
    if (!me) {
        return NULL;
    }
    if (!SML_EHASH_IMPLNAME(init)(me, hash_fn, compare_fn)) {
        free(me);
        return NULL;
    }
    return me;
}

static bool SML_EHASH_IMPLNAME(init)(SML_EHASH_TNAME *me, SML_EHASH_IMPLNAME(hash_fn) hash_fn, SML_EHASH_IMPLNAME(compare_fn) compare_fn)
{
    return SML_EHASH_IMPLNAME(initWithDepth)(me, hash_fn, compare_fn, SML_EHASH_INITIAL_BUCKET_DEPTH);
}

static bool SML_EHASH_IMPLNAME(initWithDepth)(SML_EHASH_TNAME *me, SML_EHASH_IMPLNAME(hash_fn) hash_fn, SML_EHASH_IMPLNAME(compare_fn) compare_fn, unsigned int bucketDepth)
{
    /* save hash function pointer, if key is string default can be used */
#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
    if (hash_fn) {
        me->hash_fn = hash_fn;
    } else {
        me->hash_fn = &std_view_hash_fn;
    }
#elif SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_CSTRING
    if (hash_fn) {
        me->hash_fn = hash_fn;
    } else {
        me->hash_fn = &std_hash_fn;
    }
#else
    if (!hash_fn) {
        return false;
    }
    me->hash_fn = hash_fn;
#endif /* SML_EHASH_ISKEYSTRING */

    /* save hash function pointer, if key is string, default (NULL) can be used */
#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
    if (compare_fn) {
        me->compare_fn = compare_fn;
    } else {
        me->compare_fn = &std_view_compare_fn;
    }
#elif SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_CSTRING
    if (compare_fn) {
        me->compare_fn = compare_fn;
    } else {
        me->compare_fn = &std_compare_fn;
    }
#else
    if (!compare_fn) {
        return false;
    }
    me->compare_fn = compare_fn;
#endif /* SML_EHASH_ISKEYSTRING */

    /* determine initial depth(s) */
    unsigned int globalDepth;
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
    const unsigned int numDir    = (1 << globalDepth);
    const unsigned int numBucket = (1 << bucketDepth);

    me->directory = (unsigned int *)malloc(numDir * sizeof(*me->directory));
    if (me->directory == NULL) {
        goto err1;
    }
    for (unsigned int i = 0; i < numDir; ++i) {
        /* map directories to buckets */
        me->directory[i] = i & (numBucket - 1);
    }

    me->globalDepth = globalDepth;
    me->numEntries = 0;

    /* create array of buckets */
    me->buckets = (SML_EHASH_BUCKETENTRYNAME *)malloc(numBucket * sizeof(*me->buckets));
    if (me->buckets == NULL) {
        goto err2;
    }
    me->numBuckets = numBucket;
    me->capacityBuckets = numBucket;
    for (unsigned int i = 0; i < numBucket; ++i) {
        me->buckets[i].first = UINT_MAX;
        me->buckets[i].bucketSize = 0;
        me->buckets[i].bucketDepth = bucketDepth;
    }

    /* create array of items */
    me->itemBuf = (SML_EHASH_ITEMNAME *)malloc(SML_EHASH_INITIAL_ITEM_CAPACITY * sizeof(*me->itemBuf));
    if (!me->itemBuf) {
        goto err3;
    }
    me->capacityEntries = SML_EHASH_INITIAL_ITEM_CAPACITY;

    /* init free list of items */
    SML_DQueue_uint4EHashMap_init(&me->itemFreeList);

    return true;

err3:
    free(me->itemBuf);
err2:
    free(me->buckets);
err1:
    free(me->directory);
    return false;
}

static void SML_EHASH_IMPLNAME(free)(SML_EHASH_TNAME *me)
{
    if (!me) {
        return;
    }
    SML_EHASH_IMPLNAME(destroy)(me);
    /* free the object itself */
    free(me);
}

static void SML_EHASH_IMPLNAME(destroy)(SML_EHASH_TNAME *me)
{
    /* if key was allocated, go through buckets and free each item key */
#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_CSTRING || SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
    for (unsigned int i = 0; i < me->numBuckets; ++i) {
        unsigned int itemIdx = me->buckets[i].first;
        while (itemIdx != UINT_MAX) {
            SML_EHASH_ITEMNAME *item = &me->itemBuf[itemIdx];
            itemIdx = item->next;
            free(item->key);
        }
    }
#endif /* SML_EHASH_ISKEYSTRING*/

    /* free the item free list */
    SML_DQueue_uint4EHashMap_destroy(&me->itemFreeList);
    /* free the itemBuf */
    free(me->itemBuf);
    /* free the buckets array */
    free(me->buckets);
    /* free the directory array */
    free(me->directory);
    /* zero everything */
    memset(me, 0, sizeof(*me));
}

#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
static bool SML_EHASH_IMPLNAME(insert)(SML_EHASH_TNAME *me, const SML_EHASH_KEYT key, unsigned int keySize, SML_EHASH_T data)
#else
static bool SML_EHASH_IMPLNAME(insert)(SML_EHASH_TNAME *me, const SML_EHASH_KEYT key, SML_EHASH_T data)
#endif
{
#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_CSTRING || SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
    if (key == NULL) {
        return false;
    }
#endif /* SML_EHASH_ISKEYSTRING */

    uint32_t hash, dirIdx;
    unsigned int bucketIdx;

#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
    hash = (*me->hash_fn)(key, keySize);
#else
    hash = (*me->hash_fn)(key);
#endif
    dirIdx = hash & ((1 << me->globalDepth) - 1);
    bucketIdx = me->directory[dirIdx];

    /* try direct inserting */
    unsigned int itemIdx = me->buckets[bucketIdx].first;
    unsigned int chainLevel = 0;

    while(itemIdx != UINT_MAX) {
        /* item already existing? -> overwrite */
        SML_EHASH_ITEMNAME *item = &me->itemBuf[itemIdx];
#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
        if (me->compare_fn(key, item->key, keySize))
#else
        if (me->compare_fn(key, item->key))
#endif
        {
            item->data = data;
            return true;
        }
        ++chainLevel;
        itemIdx = item->next;
    }

    /* still have more space in bucket? */
    if (chainLevel < SML_EHASH_MAX_BUCKET_SIZE) {
#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
        if (!SML_EHASH_IMPLNAME(createItemAndInsertFirst)(me, key, keySize, data, hash, bucketIdx))
#else
        if (!SML_EHASH_IMPLNAME(createItemAndInsertFirst)(me, key, data, hash, bucketIdx))
#endif
        {
            return false;
        }
        // print_table(me);
        return true;
    }

    /* bucket is full, expand */
    if (!SML_EHASH_IMPLNAME(expand)(me, hash)) {
        return false;
    }

    /* insert new item */
    dirIdx = hash & ((1U << me->globalDepth) - 1);
    bucketIdx = me->directory[dirIdx];

#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
    if (!SML_EHASH_IMPLNAME(createItemAndInsertFirst)(me, key, keySize, data, hash, bucketIdx))
#else
    if (!SML_EHASH_IMPLNAME(createItemAndInsertFirst)(me, key, data, hash, bucketIdx))
#endif
    {
        return false;
    }
    // print_table(me);
    return true;
}

#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
static bool SML_EHASH_IMPLNAME(get)(SML_EHASH_TNAME *me, const SML_EHASH_KEYT key, unsigned int keySize, SML_EHASH_T *data)
#else
static bool SML_EHASH_IMPLNAME(get)(SML_EHASH_TNAME *me, const SML_EHASH_KEYT key, SML_EHASH_T *data)
#endif
{
#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW || SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_CSTRING
    if (key == NULL) {
        return false;
    }
#endif /* SML_EHASH_KEYCLASS */

    uint32_t hash, dirIdx;
    unsigned int bucketIdx;

#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
    hash = (*me->hash_fn)(key, keySize);
#else
    hash = (*me->hash_fn)(key);
#endif
    dirIdx = hash & ((1U << me->globalDepth) - 1);
    bucketIdx = me->directory[dirIdx];

    unsigned int itemIdx = me->buckets[bucketIdx].first;

    while(itemIdx != UINT_MAX) {
        /* compare keys */
        SML_EHASH_ITEMNAME *item = &me->itemBuf[itemIdx];
#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
        if (me->compare_fn(key, item->key, keySize))
#else
        if (me->compare_fn(key, item->key))
#endif
        {
            *data = item->data;
            return true;
        }
        itemIdx = item->next;
    }

    /* item with this key does not exist */
    return false;
}

#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
static void SML_EHASH_IMPLNAME(erase)(SML_EHASH_TNAME *me, const SML_EHASH_KEYT key, unsigned int keySize)
#else
static void SML_EHASH_IMPLNAME(erase)(SML_EHASH_TNAME *me, const SML_EHASH_KEYT key)
#endif
{
#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW || SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_CSTRING
    if (key == NULL) {
        return;
    }
#endif /* SML_EHASH_KEYCLASS */

    uint32_t hash, dirIdx;
    unsigned int bucketIdx;

#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
    hash = (*me->hash_fn)(key, keySize);
#else
    hash = (*me->hash_fn)(key);
#endif
    dirIdx = hash & ((1U << me->globalDepth) - 1);
    bucketIdx = me->directory[dirIdx];

    unsigned int itemIdx = me->buckets[bucketIdx].first;
    unsigned int *prevIdxPtr = &me->buckets[bucketIdx].first;

    while (itemIdx != UINT_MAX) {
        SML_EHASH_ITEMNAME *item = &me->itemBuf[itemIdx];
#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
        if (me->compare_fn(key, item->key, keySize))
#else
        if (me->compare_fn(key, item->key))
#endif
        {
            /* item found, erase it */
            *prevIdxPtr = item->next;
            --me->buckets[bucketIdx].bucketSize;
            --me->numEntries;
#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW || SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_CSTRING
            free(item->key);
#endif
            /* push index onto free list */
            SML_DQueue_uint4EHashMap_push(&me->itemFreeList, itemIdx);
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
static void SML_EHASH_IMPLNAME(clear)(SML_EHASH_TNAME *me)
{
    /* loop through buckets and invalidate items */
    for (unsigned int i = 0; i < me->numBuckets; ++i) {
        me->buckets[i].first = UINT_MAX;
        me->buckets[i].bucketSize = 0;
        /* if keys are allocated, free those */
#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW || SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_CSTRING
        unsigned int itemIdx = me->buckets[i].first;
        while (itemIdx != UINT_MAX) {
            SML_EHASH_ITEMNAME *const item = &me->itemBuf[itemIdx];
            itemIdx = item->next;
            free(item->key);
        }
#endif
    }
    /* instead of pushing everything on the free list, only set numEntries to zero */
    me->numEntries = 0;
    SML_DQueue_uint4EHashMap_clear(&me->itemFreeList);
}

static bool SML_EHASH_IMPLNAME(expand)(SML_EHASH_TNAME *me, uint32_t hash) // TODO: return bucketIdx
{
    /* recursively split bucket or grow directory until item with "hash" can be inserted */
    while (me->globalDepth <= SML_EHASH_MAX_GLOBAL_DEPTH) {
        uint32_t dirIdx = hash & ((uint64_t)(1 << me->globalDepth) - 1);
        uint32_t bucketIdx = me->directory[dirIdx];

        if (!(me->globalDepth > me->buckets[bucketIdx].bucketDepth)) {
            /* grow the directory */
            unsigned int *p = (unsigned int *)realloc(me->directory, (1U << (me->globalDepth + 1)) * sizeof(*me->directory));
            if (p == NULL) {
                return false;
            }
            me->directory = p;

            ++me->globalDepth;

            /* duplicate first half of the directory to the second half */
            for (unsigned int i = 0; i < (1U << (me->globalDepth - 1)); ++i) {
                me->directory[i + (1U << (me->globalDepth - 1))] = me->directory[i];
            }
        }

        /* split the bucket */
        const unsigned int prevBucketDepth = me->buckets[bucketIdx].bucketDepth;

        /* increase bucket number by one, if necessary allocate larger array */
        if (me->numBuckets + 1 > me->capacityBuckets) {
            unsigned int newCap = me->numBuckets * 3 / 2 + 1;
            SML_EHASH_BUCKETENTRYNAME *p = (SML_EHASH_BUCKETENTRYNAME *)realloc(me->buckets, newCap * sizeof(*me->buckets));
            if (p == NULL) {
                return false;
            }
            me->buckets = p;
            me->capacityBuckets = newCap;
        }
        SML_EHASH_BUCKETENTRYNAME *const origBucket = &me->buckets[bucketIdx];
        ++me->numBuckets;
        ++origBucket->bucketDepth;
        me->buckets[me->numBuckets - 1].first = UINT_MAX;
        me->buckets[me->numBuckets - 1].bucketSize = 0;
        me->buckets[me->numBuckets - 1].bucketDepth = origBucket->bucketDepth;

        /* set the relevant directories to the new bucket (the ones with the "1" in the next higher bit) */
        const uint64_t stride = 1 << (prevBucketDepth + 1);
        const uint64_t offset = (stride >> 1) | (dirIdx & ((stride >> 1) - 1));

        for(uint64_t i = offset; i < (uint64_t)(1 << me->globalDepth); i += stride) {
            me->directory[i] = me->numBuckets - 1;
        }

        SML_EHASH_BUCKETENTRYNAME *const newBucket = &me->buckets[me->numBuckets - 1];

        // TODO: do this with less local variables
        unsigned int itemIdx = origBucket->first;
        unsigned int *prevIdxPtr = &origBucket->first;

        while(itemIdx != UINT_MAX) {
            SML_EHASH_ITEMNAME *item = &me->itemBuf[itemIdx];
            /* save for later */
            unsigned int nextIdx = item->next;

            /* test if item needs to be moved */
            if (item->hash & (1 << (uint64_t)(origBucket->bucketDepth - 1))) {
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

        // print_table(me);

        /* bucket index could have changed */
        bucketIdx = me->directory[dirIdx];

        if (me->buckets[bucketIdx].bucketSize < SML_EHASH_MAX_BUCKET_SIZE) {
            return true;
        }
    }
    /* directory depth is at its maximum */
    return false;
}

static SML_EHASH_ITERATORNAME SML_EHASH_IMPLNAME(firstIt)(SML_EHASH_TNAME *me)
{
    /* map is empty? -> return the end iterator */
    if (SML_EHASH_IMPLNAME(empty)(me)) {
        return (SML_EHASH_ITERATORNAME){ &me->itemBuf[me->numEntries], me->numBuckets };
    }

    /* serach the first item */
    unsigned int bucketIdx = 0;
    for ( ; bucketIdx < me->numBuckets; ++bucketIdx) {
        if (me->buckets[bucketIdx].first != UINT_MAX)
            break;
    }
    return (SML_EHASH_ITERATORNAME){ &me->itemBuf[me->buckets[bucketIdx].first], bucketIdx };
}

static void SML_EHASH_IMPLNAME(nextIt)(SML_EHASH_TNAME *me, SML_EHASH_ITERATORNAME *it)
{
    unsigned int bucketIdx   = it->curBucketIdx;
    SML_EHASH_ITEMNAME *item = &me->itemBuf[me->numEntries];

    /* try the next item in the same bucket */
    if (it->item->next != UINT_MAX) {
        it->item = &me->itemBuf[it->item->next];
        it->curBucketIdx = bucketIdx;
        return;
    }

    /* move to the next bucket(s) */
    ++bucketIdx;
    for ( ; bucketIdx < me->numBuckets; ++bucketIdx) {
        const unsigned int idx = me->buckets[bucketIdx].first;
        if (me->buckets[bucketIdx].first != UINT_MAX) {
            item = &me->itemBuf[idx];
            break;
        }
    }

    it->item = item;
    it->curBucketIdx = bucketIdx;
}

static bool SML_EHASH_IMPLNAME(isEndIt)(SML_EHASH_TNAME *me, SML_EHASH_ITERATORNAME *it)
{
    if (it->item == &me->itemBuf[me->numEntries]) {
        return true;
    } else {
        return false;
    }
}

#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
static SML_EHASH_ITEMNAME * SML_EHASH_IMPLNAME(createItemAndInsertFirst)(SML_EHASH_TNAME *me, const SML_EHASH_KEYT key, unsigned int keySize, SML_EHASH_T data, uint32_t hash, unsigned int bucketIdx)
#else
static SML_EHASH_ITEMNAME * SML_EHASH_IMPLNAME(createItemAndInsertFirst)(SML_EHASH_TNAME *me, const SML_EHASH_KEYT key, SML_EHASH_T data, uint32_t hash, unsigned int bucketIdx)
#endif
{
    unsigned int idx;
    bool fromFreeList;

    if (SML_DQueue_uint4EHashMap_empty(&me->itemFreeList)) {
        /* append new item */
        if (me->numEntries + 1 > me->capacityEntries) {
            /* reallocate buffer */
            unsigned int newCap = me->numEntries * 3 / 2 + 1;
            SML_EHASH_ITEMNAME *p = (SML_EHASH_ITEMNAME *)realloc(me->itemBuf, newCap * sizeof(*me->itemBuf));
            if (!p) {
                return NULL;
            }
            me->itemBuf = p;
            me->capacityEntries = newCap;
        }
        idx = me->numEntries;
        fromFreeList = false;
    } else {
        /* use existing free spot */
        idx = SML_DQueue_uint4EHashMap_front(&me->itemFreeList);
        fromFreeList = true;
    }

    /* get the item pointer */
    SML_EHASH_ITEMNAME *const item = &me->itemBuf[idx];

    /* set item data */
#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
    item->key = SML_strviewdup(key, keySize);
#elif SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_CSTRING
    item->key = SML_strdup(key);
#else 
    /* cast away const if key is passed as value */
    item->key = (SML_EHASH_KEYT)key;
#endif /* SML_EHASH_ISKEYSTRING*/

    item->data = data;
    item->hash = hash;

    unsigned int firstIdx = me->buckets[bucketIdx].first;
    item->next = firstIdx;
    me->buckets[bucketIdx].first = idx;
    ++me->buckets[bucketIdx].bucketSize;
    ++me->numEntries;

    if (fromFreeList) {
        SML_DQueue_uint4EHashMap_pop(&me->itemFreeList);
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

/*
 * Default hash and compare functions for (char *) keys
 */
static uint32_t std_hash_fn(const char *c)
{
    unsigned long ret = 0;
    long n;
    unsigned long v;
    int r;

    if ((c == NULL) || (*c == '\0'))
        return ret;

    n = 0x100;
    while (*c) {
        v = n | (*c);
        n += 0x100;
        r = (int)((v >> 2) ^ v) & 0x0f;
        /* cast to uint64_t to avoid 32 bit shift of 32 bit value */
        ret = (ret << r) | (unsigned long)((uint64_t)ret >> (32 - r));
        ret &= 0xFFFFFFFFL;
        ret ^= v * v;
        c++;
    }
    return (ret >> 16) ^ ret;
}

static bool std_compare_fn(const char *a, const char *b)
{
    return !strcmp(a, b);
}

static uint32_t std_view_hash_fn(const char *c, unsigned int size)
{
    unsigned long ret = 0;
    long n;
    unsigned long v;
    int r;

    if ((c == NULL) || (*c == '\0'))
        return ret;

    n = 0x100;
    while (size-- > 0) {
        v = n | (*c);
        n += 0x100;
        r = (int)((v >> 2) ^ v) & 0x0f;
        /* cast to uint64_t to avoid 32 bit shift of 32 bit value */
        ret = (ret << r) | (unsigned long)((uint64_t)ret >> (32 - r));
        ret &= 0xFFFFFFFFL;
        ret ^= v * v;
        c++;
    }
    return (ret >> 16) ^ ret;
}

static bool std_view_compare_fn(const char *a, const char *b, unsigned int size)
{
    /* use memcmp instead of strncmp since it should be faster */
    return !memcmp(a, b, size);
}

#endif /* SML_EHASH_LOCAL_ONCE */



/**
 * Returns the number of entries
*/
static inline unsigned int SML_EHASH_IMPLNAME(size)(const SML_EHASH_TNAME *me)
{
    return me->numEntries;
}

/**
 * Returns true if the hash map is empty
*/
static inline bool SML_EHASH_IMPLNAME(empty)(const SML_EHASH_TNAME *me)
{
    return me->numEntries == 0;
}

#ifdef __cplusplus
}
#endif

#undef SML_EHASH_KEYCLASS
#undef SML_EHASH_IDENT