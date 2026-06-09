#ifndef INCLUDE_SML_LEXER_H
#define INCLUDE_SML_LEXER_H

#include <stddef.h>
#include <stdbool.h>

#define FOR_LIST_OF_VALID_TOKEN_TYPES(DO) \
    DO(SML_TOK_WHITESPACE) \
    DO(SML_TOK_IDENT) \
    DO(SML_TOK_INTEGER) \
    DO(SML_TOK_REAL) \
    DO(SML_TOK_LPAREN) \
    DO(SML_TOK_RPAREN) \
    DO(SML_TOK_LBRACE) \
    DO(SML_TOK_RBRACE) \
    DO(SML_TOK_LBRACKET) \
    DO(SML_TOK_RBRACKET) \
    DO(SML_TOK_DOT) \
    DO(SML_TOK_COMMA) \
    DO(SML_TOK_COLON) \
    DO(SML_TOK_SEMICOL) \
    DO(SML_TOK_STRLIT_DQUOTE) \
    DO(SML_TOK_PLUS) \
    DO(SML_TOK_MINUS) \
    DO(SML_TOK_MULT) \
    DO(SML_TOK_DIV)

#define FOR_LIST_OF_INVALID_TOKEN_TYPES(DO) \
    DO(SML_TOK_UNSCANNED) \
    DO(SML_TOK_UNKNOWN) \
    DO(SML_TOK_END)

typedef enum SML_TokenType {
#define DO(token_) token_,
    FOR_LIST_OF_VALID_TOKEN_TYPES(DO)
    FOR_LIST_OF_INVALID_TOKEN_TYPES(DO)
#undef DO
} SML_TokenType;

static inline const char *SML_TokenType_toString(SML_TokenType type)
{
    switch (type) {
#define DO(token_) case token_: return #token_;
    FOR_LIST_OF_VALID_TOKEN_TYPES(DO)
    FOR_LIST_OF_INVALID_TOKEN_TYPES(DO)
#undef DO
        default:
            return "SML_TOK_REALLYUNKNOWN";
    }
}

static inline bool SML_TokenType_isValid(SML_TokenType type)
{
    switch (type) {
#define DO(token_) case token_: return false;
    FOR_LIST_OF_INVALID_TOKEN_TYPES(DO)
#undef DO
        default:
            return true;
    }
}

typedef struct SML_Token {
    const char *data;
    size_t size;
    SML_TokenType type;
} SML_Token;

typedef struct SML_Lexer {
    const char *begin;        /**< points to beginning of the input string */
    const char *end;          /**< points to first character after the string */
    const char *cursor;       /**< points to beginning of the next token to be analyzed, could be one after current */
    SML_Token nextTok;        /**< cached next token */
} SML_Lexer;

void        SML_Lexer_init(SML_Lexer *me, const char *begin, const char *end);
void        SML_Lexer_rewind(SML_Lexer *me);
SML_Token   SML_Lexer_nextToken(SML_Lexer *me, bool skipInvisible);
SML_Token   SML_Lexer_peekToken(SML_Lexer *me, bool skipInvisible);

#endif /* INCLUDE_SML_LEXER_H */
