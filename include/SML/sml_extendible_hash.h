#ifndef INCLUDE_SML_EXTENDIBLE_HASH_H
#define INCLUDE_SML_EXTENDIBLE_HASH_H

#include <stdbool.h>
#include <stdint.h>

/*
 * Declare a dynamic queue of type unsigned int 
 */
#ifndef SML_DQUEUE_EHashMap_uint_DECL
#define SML_DQUEUE_EHashMap_uint_DECL
#define SML_DQUEUE_T unsigned int
#define SML_DQUEUE_ID EHashMap_uint
#include "SML/sml_dyn_queue.h"
#undef SML_DQUEUE_ID
#undef SML_DQUEUE_T
#endif /* SML_DVEC_EHashMap_uint_DECL */ 

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

typedef struct SML_EHASH_ITEMNAME SML_EHASH_ITEMNAME;

struct SML_EHASH_ITEMNAME {
    unsigned int next;
    SML_EHASH_KEYT key;
#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
    unsigned int keySize;
#endif
    SML_EHASH_T data;
    uint32_t hash;
};

typedef struct SML_EHASH_BUCKETENTRYNAME {
    unsigned int first;
    // SML_EHASH_ITEMNAME *first;
    uint16_t bucketSize;
    uint16_t bucketDepth;
}  SML_EHASH_BUCKETENTRYNAME;

typedef struct SML_EHASH_TNAME {                           
    SML_EHASH_IMPLNAME(hash_fn) hash_fn;        /**< hash function pointer */
    SML_EHASH_IMPLNAME(compare_fn) compare_fn;  /**< compare function pointer */
    SML_EHASH_BUCKETENTRYNAME *buckets;         /**< array of buckets */
    SML_EHASH_ITEMNAME *itemBuf;                /**< array of items */    
    SML_DQueue_EHashMap_uint itemFreeList;      /**< free list containing free indices into itemBuf */                     
    unsigned int *directory;                    /**< global directory, contains indices into "buckets" */
    unsigned int globalDepth;                   /**< bit depth of "directory" */                    
    unsigned int numEntries;                    /**< total number of items currently used */
    unsigned int capacityEntries;               /**< total number of items currently allocated */
    unsigned int numBuckets;                    /**< total number of buckets currently used */
    unsigned int capacityBuckets;               /**< total number of buckets currently allocated */              
} SML_EHASH_TNAME; 

SML_EHASH_TNAME * SML_EHASH_IMPLNAME(create)(SML_EHASH_IMPLNAME(hash_fn) hash_fn, SML_EHASH_IMPLNAME(compare_fn) compare_fn);
void          SML_EHASH_IMPLNAME(free)(SML_EHASH_TNAME *me);
void          SML_EHASH_IMPLNAME(destroy)(SML_EHASH_TNAME *me);
bool          SML_EHASH_IMPLNAME(init)(SML_EHASH_TNAME *me, SML_EHASH_IMPLNAME(hash_fn) hash_fn, SML_EHASH_IMPLNAME(compare_fn) compare_fn);
bool          SML_EHASH_IMPLNAME(initWithDepth)(SML_EHASH_TNAME *me, SML_EHASH_IMPLNAME(hash_fn) hash_fn, SML_EHASH_IMPLNAME(compare_fn) compare_fn, unsigned int bucketDepth);
void          SML_EHASH_IMPLNAME(clear)(SML_EHASH_TNAME *me);

#if SML_EHASH_KEYCLASS == SML_EHASH_KEYCLASS_STRINGVIEW
bool          SML_EHASH_IMPLNAME(insert)(SML_EHASH_TNAME *me, const SML_EHASH_KEYT key, unsigned int keySize, SML_EHASH_T data);
bool          SML_EHASH_IMPLNAME(get)(SML_EHASH_TNAME *me, const SML_EHASH_KEYT key, unsigned int keySize, SML_EHASH_T *data);
void          SML_EHASH_IMPLNAME(erase)(SML_EHASH_TNAME *me, const SML_EHASH_KEYT key, unsigned int keySize);

#else
bool          SML_EHASH_IMPLNAME(insert)(SML_EHASH_TNAME *me, const SML_EHASH_KEYT key, SML_EHASH_T data);
bool          SML_EHASH_IMPLNAME(get)(SML_EHASH_TNAME *me, const SML_EHASH_KEYT key, SML_EHASH_T *data);
void          SML_EHASH_IMPLNAME(erase)(SML_EHASH_TNAME *me, const SML_EHASH_KEYT key);

#endif

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