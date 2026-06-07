#ifndef INCLUDE_SML_JSON_H
#define INCLUDE_SML_JSON_H

#include <stdint.h>
#include <stddef.h>

typedef struct SML_JsonNode SML_JsonNode;
typedef uint32_t SML_JsonNodeType;

/*
 * Implement hash map with key string view and value (JsonNode *)
 * for SML_JsonNodeObject 
 */
#ifndef SML_EHashMap_JsonNodep_IMPL
#define SML_EHashMap_JsonNodep_IMPL
#define SML_EHASH_T SML_JsonNode *
#define SML_EHASH_KEYT char *
#define SML_EHASH_ID JsonNodep
#define SML_EHASH_ISKEYSTRING true
#define SML_EHASH_ISKEYCSTRING false
#include "SML/sml_extendible_hash.h"
#undef SML_EHASH_ISKEYCSTRING
#undef SML_EHASH_ISKEYSTRING
#undef SML_EHASH_ID
#undef SML_EHASH_KEYT
#undef SML_EHASH_T
#endif /* SML_EHashMap_JsonNodep_IMPL */

/*
 * Implement dynamic vector of type (JsonNode *)
 * for SML_JsonNodeArray
 */
#ifndef SML_DVec_JsonNodep_IMPL
#define SML_DVec_JsonNodep_IMPL
#define SML_DVEC_T SML_JsonNode *
#define SML_DVEC_ID JsonNodep
#include "SML/sml_dyn_vector.h"
#undef SML_DVEC_ID
#undef SML_DVEC_T
#endif /* SML_DVec_JsonNodep_IMPL */ 

typedef enum SML_JsonNodeTypeEnum {
    SML_JSON_NODE_INVALID = 0,
    SML_JSON_NODE_OBJECT,
    SML_JSON_NODE_ARRAY,
    SML_JSON_NODE_STRING,
    SML_JSON_NODE_DOUBLE,
    SML_JSON_NODE_INT,
    SML_JSON_NODE_BOOL
} SML_JsonNodeTypeEnum;

typedef union SML_JsonValue {
    char *stringVal;
    double doubleVal;
    int64_t intVal;
    bool boolVal;
} SML_JsonValue;

struct SML_JsonNode {
    SML_JsonNodeType type;
};

typedef struct SML_JsonNodeObject {
    SML_JsonNode super;
    SML_EHashMap_JsonNodep children;
} SML_JsonNodeObject;

typedef struct SML_JsonNodeArray {
    SML_JsonNode super;
    SML_DVec_JsonNodep children;
} SML_JsonNodeArray;

typedef struct SML_JsonNodeValue {
    SML_JsonNode super;
    SML_JsonValue value;
} SML_JsonNodeValue;

/**
 * @brief Iterator for SML_JsonNodeObject
 * 
 */
typedef struct SML_JsonNodeObjectIter {
    SML_EHashMapIter_JsonNodep it;
} SML_JsonNodeObjectIter;

SML_JsonNode *SML_JsonNode_createObject(void);
SML_JsonNode *SML_JsonNode_createArray(void);
SML_JsonNode *SML_JsonNode_createString(const char *stringVal);
SML_JsonNode *SML_JsonNode_createStringFromView(const char *stringVal, unsigned int size);
SML_JsonNode *SML_JsonNode_createDouble(double doubleVal);
SML_JsonNode *SML_JsonNode_createInt(int64_t intVal);
SML_JsonNode *SML_JsonNode_createBool(bool boolVal);
void          SML_JsonNode_free(SML_JsonNode *me);
void          SML_JsonNode_print(SML_JsonNode *me);

unsigned int  SML_JsonNodeObject_size(SML_JsonNode *me);
SML_JsonNode *SML_JsonNodeObject_get(SML_JsonNode *me, const char *name);
SML_JsonNode *SML_JsonNodeObject_getFromView(SML_JsonNode *me, const char *name, unsigned int nameSize);
void          SML_JsonNodeObject_insert(SML_JsonNode *me, const char *name, unsigned int nameSize, SML_JsonNode *child);

SML_JsonNodeObjectIter SML_JsonNodeObject_begin(SML_JsonNode *me); 
bool SML_JsonNodeObjectIter_isEnd(const SML_JsonNodeObjectIter *me); 
void SML_JsonNodeObjectIter_next(SML_JsonNodeObjectIter *me);
// TODO: key/value getter for iterator

size_t        SML_JsonNodeArray_size(const SML_JsonNode *me);
SML_JsonNode *SML_JsonNodeArray_get(const SML_JsonNode *me, size_t idx);
void          SML_JsonNodeArray_append(SML_JsonNode *me, SML_JsonNode *child);

const char   *SML_JsonNodeValue_getString(SML_JsonNode *me);
double        SML_JsonNodeValue_getDouble(SML_JsonNode *me);
int64_t       SML_JsonNodeValue_getInt(SML_JsonNode *me);
bool          SML_JsonNodeValue_getBool(SML_JsonNode *me);

void          SML_JsonNodeValue_setString(SML_JsonNode *me, const char *stringVal);
void          SML_JsonNodeValue_setDouble(SML_JsonNode *me, double doubleVal);
void          SML_JsonNodeValue_setInt(SML_JsonNode *me, int64_t intVal);
void          SML_JsonNodeValue_setBool(SML_JsonNode *me, bool boolVal);

const char   *SML_JsonNodeType_toString(SML_JsonNodeType type);

SML_JsonNode *SML_Json_parse(const char *src, size_t len);

#endif /* INCLUDE_SML_JSON_H */
