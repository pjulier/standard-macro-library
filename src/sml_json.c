
#include <stdlib.h>

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


static SML_JsonNode *sml_Json_parseObject(SML_Lexer *lex);
static SML_JsonNode *sml_Json_parseArray(SML_Lexer *lex);
static SML_JsonNode *sml_Json_parseValue(SML_Lexer *lex);

// TODO: test
#include <stdio.h>

SML_JsonNode *SML_Json_parse(const char *src, size_t len)
{
    if (!src || len == 0) {
        return NULL;
    }

    SML_Lexer lex;
    SML_Lexer_init(&lex, src, src + len);

    // SML_Token tok;
    // while (tok = SML_Lexer_nextToken(&lex, false), tok.type != SML_TOK_END) {
    //     printf("Detected: '%.*s' (%s, len: %i)\n", (int)tok.size, tok.data, SML_TokenType_toString(tok.type), (int)tok.size);
    // }
    // SML_Lexer_rewind(&lex);

    SML_JsonNode *node;

    /* check if the first token is brace or bracket */
    if (SML_Lexer_peekToken(&lex, true).type == SML_TOK_LBRACKET) {
        node = sml_Json_parseArray(&lex);
    } else {
        node = sml_Json_parseObject(&lex);
    }

    /* next token must not be valid */
    if (SML_TokenType_isValid(SML_Lexer_peekToken(&lex, true).type)) {
        // TODO: error, free the tree
        assert(false);
        return NULL;
    }

    return node;
}

static SML_JsonNode *sml_Json_parseObject(SML_Lexer *lex)
{
    SML_JsonNode *node;

    node = SML_JsonNode_createObject();
    if (!node) {
        return NULL;
    }

    /* consume the left brace */
    if (SML_Lexer_nextToken(lex, true).type != SML_TOK_LBRACE) {
        // TODO error
        assert(false);
        return NULL;
    }

    // TODO: handle empty case
    assert(SML_Lexer_peekToken(lex, true).type == SML_TOK_STRLIT_DQUOTE);

    while (true) {
        /* read the name */
        SML_Token name;
        name = SML_Lexer_nextToken(lex, true);
        if (name.type != SML_TOK_STRLIT_DQUOTE) {
            // TODO error
            assert(false);
            break;
        }

        /* consume colon */
        if (SML_Lexer_nextToken(lex, true).type != SML_TOK_COLON) {
            // TODO error
            assert(false);
            break;
        }

        SML_JsonNode *child;

        if (SML_Lexer_peekToken(lex, true).type == SML_TOK_LBRACKET) {
            /* found an array */
            child = sml_Json_parseArray(lex);
            if (!child) {
                // TODO error
                assert(false);
                break;
            }
        } else if (SML_Lexer_peekToken(lex, true).type == SML_TOK_LBRACE) {
            /* found an object */
            child = sml_Json_parseObject(lex);
            if (!child) {
                // TODO error
                assert(false);
                break;
            }
        } else {
            /* must be a value */
            child = sml_Json_parseValue(lex);
            if (!child) {
                // TODO error
                assert(false);
                break;
            }
        }

        /* add the child to the children */
        SML_JsonNodeObject_insert(node, name.data, name.size, child);

        /* check for exit condition */
        if (SML_Lexer_peekToken(lex, true).type != SML_TOK_COMMA) {
            if (SML_Lexer_peekToken(lex, true).type == SML_TOK_RBRACE) {
                /* valid exit, consume the right brace */
                SML_Lexer_nextToken(lex, true);
                break;
            } else {
                // TODO: error
                assert(false);
                break;
            }
        }

        /* consume the comma */
        SML_Lexer_nextToken(lex, true);
    }
    return node;
}

static SML_JsonNode *sml_Json_parseArray(SML_Lexer *lex)
{
    SML_JsonNode *node;
    
    node = SML_JsonNode_createArray();
    if (!node) {
        return NULL;
    }

    /* consume left bracket */
    if (SML_Lexer_nextToken(lex, true).type != SML_TOK_LBRACKET) {
        // TODO: error
        assert(false);
        return NULL;
    }

    // TODO: handle empty case

    while (true) {
        SML_JsonNode *child;
        SML_Token tok;
        tok = SML_Lexer_peekToken(lex, true);
        
        if (tok.type == SML_TOK_LBRACE) {
            /* is an object */
            child = sml_Json_parseObject(lex);
            if (!child) {
                // TODO: error
                assert(false);
                break;
            }
        } else {
            /* must be a value */
            child = sml_Json_parseValue(lex);
            if (!child) {
                // TODO error
                assert(false);
                break;
            }
        }

        /* add the child to the children */
        SML_JsonNodeArray_append(node, child);

        /* check for exit condition */
        if (SML_Lexer_peekToken(lex, true).type != SML_TOK_COMMA) {
            if (SML_Lexer_peekToken(lex, true).type == SML_TOK_RBRACKET) {
                /* valid exit, consume the right bracket */
                SML_Lexer_nextToken(lex, true);
                break;
            } else {
                // TODO: error
                assert(false);
                break;
            }
        }

        /* consume the comma */
        SML_Lexer_nextToken(lex, true);
    }
    return node;
}

static SML_JsonNode *sml_Json_parseValue(SML_Lexer *lex)
{
    SML_JsonNode *node;
    SML_Token tok;

    tok = SML_Lexer_nextToken(lex, true);

    if (!SML_TokenType_isValid(tok.type)) {
        // TODO error, e.g. end
        assert(false);
        return NULL;
    }

    /*
     * HACK: if minus sign is present make it part of the token 
     * so that the conversion function sees it
     * (important for value range of signed int)
     */
    if (tok.type == SML_TOK_MINUS) {
        const SML_Token minusTok = tok;
        SML_Token peek;
        peek = SML_Lexer_peekToken(lex, false);
        if (peek.type != SML_TOK_INTEGER && peek.type != SML_TOK_REAL) {
            // TODO: error
            assert(false);
            return NULL;
        }
        SML_Lexer_nextToken(lex, false);
        tok.type = peek.type;
        tok.data = minusTok.data;
        tok.size = minusTok.size + peek.size;
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
            // TODO error
            assert(false);
            node = NULL;
        }
        node = SML_JsonNode_createBool(true);
    } else if (tok.type == SML_TOK_INTEGER) {
        /* is integer */
        int64_t value;
        if (SML_charstoi64(tok.data, NULL, 10, &value) != SML_RET_OK) {
            // TODO error parsing int
            assert(false);
            return NULL;
        }
        node = SML_JsonNode_createInt(value);
    } else if (tok.type == SML_TOK_REAL) {
        /* is real number */
        double value;
        char *end;
        value = strtod(tok.data, &end);
        if (tok.data == end) {
            // TODO error parsing double
            assert(false);
            return NULL;
        }
        node = SML_JsonNode_createDouble(value);
    } else {
        // TODO: error, invalid token
        assert(false);
        node = NULL;
    }
    return node;
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
