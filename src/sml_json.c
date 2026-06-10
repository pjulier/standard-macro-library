
#include <stdlib.h>
#include <stdarg.h>  /* for va_list */
#include <stdio.h>   /* for vsnprintf */

#include "SML/sml_common.h"
#include "SML/sml_string.h"
#include "SML/sml_parsing_utils.h"
#include "SML/sml_lexer.h"
#include "SML/sml_json.h"

// TODO: temp
#include <assert.h>


SML_JsonNode *SML_JsonNode_createObject(void)
{
    SML_JsonNodeObject *p = (SML_JsonNodeObject *)malloc(sizeof(*p));
    if (!p) return NULL;
    p->super.type = SML_JSON_NODE_OBJECT;
    if (!SML_EHashMap_JsonNodep_init(&p->children, NULL, NULL)) {
        free(p);
        return NULL;
    }
    return (SML_JsonNode *)p;
}

SML_JsonNode *SML_JsonNode_createArray(void)
{
    SML_JsonNodeArray *p = (SML_JsonNodeArray *)malloc(sizeof(*p));
    if (!p) return NULL;
    p->super.type = SML_JSON_NODE_ARRAY;
    SML_DVec_JsonNodep_init(&p->children);
    return (SML_JsonNode *)p;
}

SML_JsonNode *SML_JsonNode_createString(const char *stringVal)
{
    SML_JsonNodeValue *p = (SML_JsonNodeValue *)malloc(sizeof(*p));
    if (!p) {
        return NULL;
    }
    p->super.type = SML_JSON_NODE_STRING;
    if (stringVal) {
        p->value.stringVal = SML_strdup(stringVal);
    } else {
        p->value.stringVal = NULL;
    }
    return (SML_JsonNode *)p;
}

SML_JsonNode *SML_JsonNode_createStringFromView(const char *stringVal, unsigned int size)
{
    SML_JsonNodeValue *p = (SML_JsonNodeValue *)malloc(sizeof(*p));
    if (!p) {
        return NULL;
    }
    p->super.type = SML_JSON_NODE_STRING;
    if (stringVal || size == 0) {
        p->value.stringVal = SML_strviewdup(stringVal, size);
    } else {
        p->value.stringVal = NULL;
    }
    return (SML_JsonNode *)p;
}

SML_JsonNode *SML_JsonNode_createDouble(double doubleVal)
{
    SML_JsonNodeValue *p = (SML_JsonNodeValue *)malloc(sizeof(*p));
    if (!p) {
        return NULL;
    }
    p->super.type = SML_JSON_NODE_DOUBLE;
    p->value.doubleVal = doubleVal;
    return (SML_JsonNode *)p;
}

SML_JsonNode *SML_JsonNode_createInt(int64_t intVal)
{
    SML_JsonNodeValue *p = (SML_JsonNodeValue *)malloc(sizeof(*p));
    if (!p) {
        return NULL;
    }
    p->super.type = SML_JSON_NODE_INT;
    p->value.intVal = intVal;
    return (SML_JsonNode *)p;
}

SML_JsonNode *SML_JsonNode_createBool(bool boolVal)
{
    SML_JsonNodeValue *p = (SML_JsonNodeValue *)malloc(sizeof(*p));
    if (!p) {
        return NULL;
    }
    p->super.type = SML_JSON_NODE_BOOL;
    p->value.boolVal = boolVal;
    return (SML_JsonNode *)p;
}

void SML_JsonNode_free(SML_JsonNode *me)
{
    if (!me || me->type == SML_JSON_NODE_INVALID) {
        return;
    }

    switch (me->type) {
        case SML_JSON_NODE_OBJECT: {
            SML_JsonNodeObject *p = (SML_JsonNodeObject *)me;
            /* free the children recursively */
            for (SML_EHashMapIter_JsonNodep it = SML_EHashMap_JsonNodep_begin(&p->children);
                 !SML_EHashMapIter_JsonNodep_isEnd(&it);
                 SML_EHashMapIter_JsonNodep_next(&it))
            {
                SML_JsonNode_free(it.item->value);
            }
            /* free the map */
            SML_EHashMap_JsonNodep_destroy(&p->children);
            break;
        }
        case SML_JSON_NODE_ARRAY: {
            SML_JsonNodeArray *p = (SML_JsonNodeArray *)me;
            /* free the children recursively */
            for (size_t i = 0; i < SML_DVec_JsonNodep_size(&p->children); ++i) {
                SML_JsonNode_free(SML_DVec_JsonNodep_get(&p->children, i));
            }
            /* free the vector */
            SML_DVec_JsonNodep_destroy(&p->children);
            break;
        }
        case SML_JSON_NODE_STRING: {
            SML_JsonNodeValue *p = (SML_JsonNodeValue *)me;
            /* free the string */
            free(p->value.stringVal);
        }
    }
    free(me);
}

unsigned int SML_JsonNodeObject_size(SML_JsonNode *me)
{
    if (me->type != SML_JSON_NODE_OBJECT) {
        return 0;
    }
    SML_JsonNodeObject *const p = (SML_JsonNodeObject *)me;
    return SML_EHashMap_JsonNodep_size(&p->children);
}

SML_JsonNode *SML_JsonNodeObject_get(SML_JsonNode *me, const char *name)
{
    unsigned int nameSize = (unsigned int)strlen(name);
    return SML_JsonNodeObject_getFromView(me, name, nameSize);
}

SML_JsonNode *SML_JsonNodeObject_getFromView(SML_JsonNode *me, const char *name, unsigned int nameSize)
{
    if (me->type != SML_JSON_NODE_OBJECT) {
        return false;
    }
    SML_JsonNodeObject *const p = (SML_JsonNodeObject *)me;
    SML_JsonNode *child;
    if (!SML_EHashMap_JsonNodep_get(&p->children, name, nameSize, &child)) {
        return NULL;
    }
    return child;
}

void SML_JsonNodeObject_insert(SML_JsonNode *me, const char *name, unsigned int nameSize, SML_JsonNode *child)
{
    if (me->type != SML_JSON_NODE_OBJECT) {
        return;
    }
    SML_JsonNodeObject *const p = (SML_JsonNodeObject *)me;
    SML_EHashMap_JsonNodep_insert(&p->children, name, nameSize, child);
}

SML_JsonNodeObjectIter SML_JsonNodeObject_begin(SML_JsonNode *me)
{
    if (me->type != SML_JSON_NODE_OBJECT) { // TODO: this is not good...
#ifdef __cplusplus
        return (SML_JsonNodeObjectIter){};
#else
        return (SML_JsonNodeObjectIter){0};
#endif
    }
    SML_JsonNodeObject *const p = (SML_JsonNodeObject *)me;
    SML_JsonNodeObjectIter iter;
    iter.it = SML_EHashMap_JsonNodep_begin(&p->children);
    return iter;
}

bool SML_JsonNodeObjectIter_isEnd(const SML_JsonNodeObjectIter *me)
{
    return SML_EHashMapIter_JsonNodep_isEnd(&me->it);
}

void SML_JsonNodeObjectIter_next(SML_JsonNodeObjectIter *me)
{
    SML_EHashMapIter_JsonNodep_next(&me->it);
}

size_t SML_JsonNodeArray_size(const SML_JsonNode *me)
{
    if (me->type != SML_JSON_NODE_ARRAY) {
        return 0;
    }
    SML_JsonNodeArray *const p = (SML_JsonNodeArray *)me;
    return SML_DVec_JsonNodep_size(&p->children);
}

SML_JsonNode *SML_JsonNodeArray_get(const SML_JsonNode *me, size_t idx)
{
    if (me->type != SML_JSON_NODE_ARRAY) {
        return NULL;
    }
    SML_JsonNodeArray *const p = (SML_JsonNodeArray *)me;
    if (idx >= SML_DVec_JsonNodep_size(&p->children)) {
        return NULL;
    }
    return SML_DVec_JsonNodep_get(&p->children, idx);
}

void SML_JsonNodeArray_append(SML_JsonNode *me, SML_JsonNode *child)
{
    if (me->type != SML_JSON_NODE_ARRAY || !child) {
        return;
    }
    SML_JsonNodeArray *const p = (SML_JsonNodeArray *)me;
    SML_DVec_JsonNodep_push_back(&p->children, child);
}

void SML_JsonNodeValue_setString(SML_JsonNode *me, const char *stringVal)
{
    if (me->type != SML_JSON_NODE_STRING) {
        return;
    }
    SML_JsonNodeValue *const p = (SML_JsonNodeValue *)me;
    if (p->value.stringVal) {
        free(p->value.stringVal);
    }
    if (stringVal) {
        p->value.stringVal = SML_strdup(stringVal);
    } else {
        p->value.stringVal = NULL;
    }
}

void SML_JsonNodeValue_setDouble(SML_JsonNode *me, double doubleVal)
{
    if (me->type != SML_JSON_NODE_DOUBLE) {
        return;
    }
    SML_JsonNodeValue *const p = (SML_JsonNodeValue *)me;
    p->value.doubleVal = doubleVal;
}

void SML_JsonNodeValue_setInt(SML_JsonNode *me, int64_t intVal)
{
    if (me->type != SML_JSON_NODE_INT) {
        return;
    }
    SML_JsonNodeValue *const p = (SML_JsonNodeValue *)me;
    p->value.intVal = intVal;
}

const char *SML_JsonNodeValue_getString(SML_JsonNode *me)
{
    if (me->type != SML_JSON_NODE_STRING) {
        return NULL;
    }
    SML_JsonNodeValue *const p = (SML_JsonNodeValue *)me;
    return p->value.stringVal;
}

double SML_JsonNodeValue_getDouble(SML_JsonNode *me)
{
    if (me->type != SML_JSON_NODE_DOUBLE) {
        return 0.0;
    }
    SML_JsonNodeValue *const p = (SML_JsonNodeValue *)me;
    return p->value.doubleVal;
}

int64_t SML_JsonNodeValue_getInt(SML_JsonNode *me)
{
    if (me->type != SML_JSON_NODE_INT) {
        return 0;
    }
    SML_JsonNodeValue *const p = (SML_JsonNodeValue *)me;
    return p->value.intVal;
}

bool SML_JsonNodeValue_getBool(SML_JsonNode *me)
{
    if (me->type != SML_JSON_NODE_BOOL) {
        return false;
    }
    SML_JsonNodeValue *const p = (SML_JsonNodeValue *)me;
    return p->value.boolVal;
}

void SML_JsonNode_setBool(SML_JsonNode *me, bool boolVal)
{
    if (me->type != SML_JSON_NODE_BOOL) {
        return;
    }
    SML_JsonNodeValue *const p = (SML_JsonNodeValue *)me;
    p->value.boolVal = boolVal;
}

void SML_JsonParseResult_init(SML_JsonParseResult *me)
{
    me->root = NULL;
    me->errorStr = NULL;
}

void SML_JsonParseResult_setRoot(SML_JsonParseResult *me, SML_JsonNode *root)
{
    me->root = root;
}

void SML_JsonParseResult_setError(SML_JsonParseResult *me, const char *fmt, ...)
{
    va_list ap;
    va_list ap2;
    va_start(ap, fmt);
    va_copy(ap2, ap);

    /* get the required length first */
    const int count = vsnprintf(NULL, 0, fmt, ap);
    
    if (count < 0) {
        const char err[] = "Error while setting the error, invalid format";
        me->errorStr = realloc(me->errorStr, SML_STRLEN_LITERAL(err) + 1);
        strcpy(me->errorStr, err);
        va_end(ap2);
        va_end(ap);
        return;
    }

    me->errorStr = realloc(me->errorStr, count + 1); /* +1 for null character */
    if (me->errorStr) {
        vsnprintf(me->errorStr, count + 1, fmt, ap2);
    }

    va_end(ap2);
    va_end(ap);
}

void SML_JsonParseResult_destroy(SML_JsonParseResult *me)
{
    SML_JsonNode_free(me->root);
    free(me->errorStr);
}



static SMLReturn sml_Json_parseObject(SML_Lexer *lex, SML_JsonParseResult *res);
static SMLReturn sml_Json_parseArray(SML_Lexer *lex, SML_JsonParseResult *res);
static SMLReturn sml_Json_parseValue(SML_Lexer *lex, SML_JsonParseResult *res);

SML_JsonParseResult SML_Json_parse(const char *src, size_t len)
{
    SMLReturn ret;
    SML_JsonParseResult res;
    SML_JsonParseResult_init(&res);

    if (!src || len == 0) {
        SML_JsonParseResult_setError(&res, "Invalid input argument");
        return res;
    }

    SML_Lexer lex;
    SML_Lexer_init(&lex, src, src + len);

    // SML_Token tok;
    // while (tok = SML_Lexer_nextToken(&lex, false), tok.type != SML_TOK_END) {
    //     printf("Detected: '%.*s' (%s, len: %i)\n", (int)tok.size, tok.data, SML_TokenType_toString(tok.type), (int)tok.size);
    // }
    // SML_Lexer_rewind(&lex);

    /* check if the first token is brace or bracket */
    if (SML_Lexer_peekToken(&lex, true).type == SML_TOK_LBRACKET) {
        ret = sml_Json_parseArray(&lex, &res);
    } else {
        ret = sml_Json_parseObject(&lex, &res);
    }

    if (ret != SML_RET_OK) {
        return res;
    }

    /* next token must not be valid */
    SML_Token tok;
    tok = SML_Lexer_peekToken(&lex, true);
    if (SML_TokenType_isValid(tok.type)) {
        SML_JsonParseResult_setError(&res, "Extra token after successful parse: %.*s", tok.size, tok.data);
        SML_JsonNode_free(res.root);
        res.root = NULL;
    }

    return res;
}

static SMLReturn sml_Json_parseObject(SML_Lexer *lex, SML_JsonParseResult *res)
{
    SML_Token tok;
    SMLReturn ret;
    SML_JsonNode *node = NULL;

    node = SML_JsonNode_createObject();
    if (!node) {
        ret = SML_RET_ENOMEM;
        goto failed;
    }

    /* consume the left brace */
    tok = SML_Lexer_nextToken(lex, true);
    if (tok.type != SML_TOK_LBRACE) {
        SML_JsonParseResult_setError(res, "Expected opening brace in object definition but got '%.*s'", tok.size, tok.data);
        ret = SML_RET_EINVAL;
        goto failed;
    }

    // TODO: handle empty case
    assert(SML_Lexer_peekToken(lex, true).type == SML_TOK_STRLIT_DQUOTE);

    while (true) {
        /* read the name */
        SML_Token name;
        name = SML_Lexer_nextToken(lex, true);
        if (name.type != SML_TOK_STRLIT_DQUOTE) {
            SML_JsonParseResult_setError(res, "Expected key name in object definition but got '%.*s'. Forgotten opening bracket for array definition?", name.size, name.data);
            ret = SML_RET_EINVAL;
            goto failed;
        }

        /* consume colon */
        tok = SML_Lexer_nextToken(lex, true);
        if (tok.type != SML_TOK_COLON) {
            SML_JsonParseResult_setError(res, "Expected colon after key name in object definition but got '%.*s'", tok.size, tok.data);
            ret = SML_RET_EINVAL;
            goto failed;
        }

        if (SML_Lexer_peekToken(lex, true).type == SML_TOK_LBRACKET) {
            /* found an array */
            ret = sml_Json_parseArray(lex, res);
            if (ret != SML_RET_OK) {
                goto failed;
            }
        } else if (SML_Lexer_peekToken(lex, true).type == SML_TOK_LBRACE) {
            /* found an object */
            ret = sml_Json_parseObject(lex, res);
            if (ret != SML_RET_OK) {
                goto failed;
            }
        } else {
            /* must be a value */
            ret = sml_Json_parseValue(lex, res);
            if (ret != SML_RET_OK) {
                goto failed;
            }
        }

        /* add the child to the children */
        SML_JsonNodeObject_insert(node, name.data, name.size, res->root);


        /* check for exit condition */
        tok = SML_Lexer_nextToken(lex, true);
        if (tok.type == SML_TOK_RBRACE) {
            /* was last element, exit the loop */
            break;
        } else if (tok.type != SML_TOK_COMMA) {
            SML_JsonParseResult_setError(res, "Expected comma or closing brace in object definition but got '%.*s'", tok.size, tok.data);
            ret = SML_RET_EINVAL;
            goto failed;
        }
    }

    res->root = node;
    return SML_RET_OK;

failed:
    if (node) {
        SML_JsonNode_free(node);
    }
    res->root = NULL;
    return ret;
}

static SMLReturn sml_Json_parseArray(SML_Lexer *lex, SML_JsonParseResult *res)
{
    SML_Token tok;
    SMLReturn ret;
    SML_JsonNode *node = NULL;
    
    node = SML_JsonNode_createArray();
    if (!node) {
        ret = SML_RET_ENOMEM;
        goto failed;
    }

    /* consume left bracket */
    tok = SML_Lexer_nextToken(lex, true);
    if (tok.type != SML_TOK_LBRACKET) {
        SML_JsonParseResult_setError(res, "Expected opening bracket in array definition but got: '%.*s'", tok.size, tok.data);
        ret = SML_RET_EINVAL;
        goto failed;
    }

    // TODO: handle empty case

    while (true) {
        tok = SML_Lexer_peekToken(lex, true);        
        if (tok.type == SML_TOK_LBRACE) {
            /* is an object */
            ret = sml_Json_parseObject(lex, res);
            if (ret != SML_RET_OK) {
                goto failed;
            }
        } else {
            /* must be a value */
            ret = sml_Json_parseValue(lex, res);
            if (ret != SML_RET_OK) {
                goto failed;
            }
        }

        /* add the child to the children */
        SML_JsonNodeArray_append(node, res->root);

        /* check for exit condition */
        tok = SML_Lexer_nextToken(lex, true);
        if (tok.type == SML_TOK_RBRACKET) {
            /* was last element, exit the loop */
            break;
        } else if (tok.type != SML_TOK_COMMA) {
            SML_JsonParseResult_setError(res, "Expected comma or closing bracket in array definition but got '%.*s'", tok.size, tok.data);
            ret = SML_RET_EINVAL;
            goto failed;
        }
    }

    res->root = node;
    return SML_RET_OK;

failed:
    if (node) {
        SML_JsonNode_free(node);
    }
    res->root = NULL;
    return ret;
}

static SMLReturn sml_Json_parseValue(SML_Lexer *lex, SML_JsonParseResult *res)
{
    SMLReturn ret;
    SML_Token tok;
    SML_JsonNode *node = NULL;

    tok = SML_Lexer_nextToken(lex, true);

    if (!SML_TokenType_isValid(tok.type)) {
        SML_JsonParseResult_setError(res, "Invalid token: '%.*s'", tok.size, tok.data);
        ret = SML_RET_EINVAL;
        goto failed;
    }

    /*
     * HACK: if minus sign is present make it part of the token 
     * so that the conversion function sees it
     * (important for value range of signed int)
     */
    if (tok.type == SML_TOK_MINUS) {
        /* save the minus token */
        const SML_Token minusTok = tok;

        tok = SML_Lexer_nextToken(lex, false);
        if (!(tok.type == SML_TOK_INTEGER || tok.type == SML_TOK_REAL)) {
            SML_JsonParseResult_setError(res, "Value after minus sign must be numeric but got: '%.*s' (%s)", tok.size, tok.data, SML_TokenType_toString(tok.type));
            ret = SML_RET_EINVAL;
            goto failed;
        }
        tok.data = minusTok.data;
        tok.size += minusTok.size;
    }

    if (tok.type == SML_TOK_STRLIT_DQUOTE) {
        /* is string literal */
        node = SML_JsonNode_createStringFromView(tok.data, tok.size);
    } else if (tok.type == SML_TOK_IDENT) {
        /* is boolean */
        if (!strncmp(tok.data, "true", tok.size)) {
            node = SML_JsonNode_createBool(true);
        } else if (!strncmp(tok.data, "false", tok.size)) {
            node = SML_JsonNode_createBool(false);
        } else {
            SML_JsonParseResult_setError(res, "Expected true or false for unquoted value but got: '%.*s'", tok.size, tok.data);
            ret = SML_RET_EINVAL;
            goto failed;
        }
    } else if (tok.type == SML_TOK_INTEGER) {
        /* is integer */
        int64_t value;
        SMLReturn ret = SML_charstoi64(tok.data, NULL, 10, &value);
        if (ret != SML_RET_OK) {
            if (ret == SML_RET_ERANGE) {
                SML_JsonParseResult_setError(res, "Integer value '%.*s' is too large to store in an int64_t", tok.size, tok.data);
            } else {
                SML_JsonParseResult_setError(res, "Error parsing integer value: '%.*s'", tok.size, tok.data);
            }
            ret = SML_RET_EINVAL;
            goto failed;
        }
        node = SML_JsonNode_createInt(value);
    } else if (tok.type == SML_TOK_REAL) {
        /* is real number */
        double value;
        char *end;
        value = strtod(tok.data, &end);
        if (tok.data == end) {
            SML_JsonParseResult_setError(res, "Error parsing real value: '%.*s'", tok.size, tok.data);
            ret = SML_RET_EINVAL;
            goto failed;
        }
        node = SML_JsonNode_createDouble(value);
    } else {
        SML_JsonParseResult_setError(res, "Expected json value but got: '%.*s'", tok.size, tok.data);
        ret = SML_RET_EINVAL;
        goto failed;
    }

    /* NULL at this point can only mean OOM */
    if (!node) {
        ret = SML_RET_ENOMEM;
        goto failed;
    }

    res->root = node;
    return SML_RET_OK;

failed:
    res->root = NULL;
    return ret;
}

static void sml_JsonNode_print(SML_JsonNode *me, unsigned int level);


void SML_JsonNode_print(SML_JsonNode *me)
{
    sml_JsonNode_print(me, 0);
    printf("\n");
}

static inline void indent(unsigned int level);

static void sml_JsonNode_print(SML_JsonNode *me, unsigned int level)
{
    // printf("Print node of type %s\n", SML_JsonNodeType_toString(me->type));

    if (me->type == SML_JSON_NODE_OBJECT) {
        size_t childCount = SML_JsonNodeObject_size(me);
        size_t iiChild = 0;
        indent(level);
        printf("{\n");

        ++level;

        for (SML_JsonNodeObjectIter it = SML_JsonNodeObject_begin(me);
            !SML_JsonNodeObjectIter_isEnd(&it);
            SML_JsonNodeObjectIter_next(&it))
        {
            const char *name = it.it.item->key;
            SML_JsonNode *child = it.it.item->value;

            indent(level);
            printf("\"%s\": ", name);

            switch (child->type) {
                case SML_JSON_NODE_OBJECT: {
                    printf("\n");
                    sml_JsonNode_print(child, level + 1);
                    break;
                }
                case SML_JSON_NODE_ARRAY: {
                    printf("\n");
                    sml_JsonNode_print(child, level + 1);
                    break;
                }
                case SML_JSON_NODE_STRING: {
                    printf("\"%s\"", SML_JsonNodeValue_getString(child));
                    break;
                }
                case SML_JSON_NODE_DOUBLE: {
                    printf("%f", SML_JsonNodeValue_getDouble(child));
                    break;
                }
                case SML_JSON_NODE_INT: {
                    printf("%i", (int)SML_JsonNodeValue_getInt(child));
                    break;
                }
                case SML_JSON_NODE_BOOL: {
                    printf("%s", SML_JsonNodeValue_getBool(child) ? "true" : "false");
                    break;
                }
            }
            if (iiChild++ < childCount - 1) {
                printf(",");
            }
            printf("\n");
        }

        --level;
        indent(level);
        printf("}");
    } else if (me->type == SML_JSON_NODE_ARRAY) {
        indent(level);
        printf("[\n");

        ++level;

        for (size_t iiChild = 0; iiChild < SML_JsonNodeArray_size(me); ++iiChild) {
            SML_JsonNode *child = SML_JsonNodeArray_get(me, iiChild);

            switch (child->type) {
                case SML_JSON_NODE_OBJECT: {
                    sml_JsonNode_print(child, level);
                    break;
                }
                case SML_JSON_NODE_ARRAY: {
                    // TODO: error
                    assert(false);
                    break;
                }
                case SML_JSON_NODE_STRING: {
                    indent(level);
                    printf("\"%s\"", SML_JsonNodeValue_getString(child));
                    break;
                }
                case SML_JSON_NODE_DOUBLE: {
                    indent(level);
                    printf("%f", SML_JsonNodeValue_getDouble(child));
                    break;
                }
                case SML_JSON_NODE_INT: {
                    indent(level);
                    printf("%i", (int)SML_JsonNodeValue_getInt(child));
                    break;
                }
                case SML_JSON_NODE_BOOL: {
                    indent(level);
                    printf("%s", SML_JsonNodeValue_getBool(child) ? "true" : "false");
                    break;
                }
            }
            if (iiChild < SML_JsonNodeArray_size(me) - 1) {
                printf(",");
            }
            printf("\n");
        }
        --level;
        indent(level);
        printf("]");
    } else {
        printf("only a value\n");
    }
}

static inline void indent(unsigned int level)
{
    while (level-- > 0) {
        printf("    ");
    }
}

const char *SML_JsonNodeType_toString(SML_JsonNodeType type)
{
    switch (type) {
        case SML_JSON_NODE_OBJECT: {
            return "SML_JSON_NODE_OBJECT";
        }
        case SML_JSON_NODE_ARRAY: {
            return "SML_JSON_NODE_OBJECT";
        }
        case SML_JSON_NODE_STRING: {
            return "SML_JSON_NODE_STRING";
        }
        case SML_JSON_NODE_DOUBLE: {
            return "SML_JSON_NODE_DOUBLE";
        }
        case SML_JSON_NODE_INT: {
            return "SML_JSON_NODE_INT";
        }
        case SML_JSON_NODE_BOOL: {
            return "SML_JSON_NODE_BOOL";
        }
        default: {
            return "SML_JSON_NODE_INVALID";
        }
    }
}
