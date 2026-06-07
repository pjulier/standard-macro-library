#include "SML/sml_parsing_utils.h"
#include "SML/sml_lexer.h"


void SML_Lexer_init(SML_Lexer *me, const char *begin, const char *end)
{
    me->begin = begin;
    me->cursor = begin;
    me->end = end;
    me->nextTok.type = SML_TOK_UNSCANNED;
}

void SML_Lexer_rewind(SML_Lexer *me)
{
    me->cursor = me->begin;
    me->nextTok.type = SML_TOK_UNSCANNED;
}

SML_Token SML_Lexer_peekToken(SML_Lexer *me, bool skipInvisible)
{
    /* already scanned? */
    if (me->nextTok.type != SML_TOK_UNSCANNED) {
        return me->nextTok;
    }

    const char *c = me->cursor;

    /* reached the end? */
    if (c == me->end) {
        me->nextTok.data = me->cursor;
        me->nextTok.size = 1;
        me->nextTok.type = SML_TOK_END;
        return me->nextTok;
    }

    /* if requested, ignore newlines and whitespace */
    if (skipInvisible) {
        while (SML_ParsUtil_isLineEnd(*c) || SML_ParsUtil_isSpace(*c)) {
            ++c; 
        }
        me->cursor = c;
    }

    /* reached the end? */
    if (c == me->end) {
        me->nextTok.data = me->cursor;
        me->nextTok.size = 1;
        me->nextTok.type = SML_TOK_END;
        return me->nextTok;
    }

    /* if requested, scan invisible tokens */
    if (!skipInvisible) {
        if (SML_ParsUtil_isSpace(*c)) {
            ++c;
            while (SML_ParsUtil_isSpace(*c)) {
                ++c;
            }
            me->nextTok.type = SML_TOK_WHITESPACE;
            me->nextTok.data = me->cursor;
            me->nextTok.size = c - me->cursor;
            me->cursor = c;
            return me->nextTok;
        }
        // TODO: newline tokens
    }

    if (SML_ParsUtil_isLower(*c) || SML_ParsUtil_isUpper(*c) || *c == '_') {
        ++c;
        while (SML_ParsUtil_isLower(*c) || SML_ParsUtil_isUpper(*c) || SML_ParsUtil_isDigit(*c) || *c == '_') { 
            ++c;
        }
        me->nextTok.type = SML_TOK_IDENT;
        me->nextTok.data = me->cursor;
        me->nextTok.size = c - me->cursor;
        me->cursor = c;
    }
    else if (SML_ParsUtil_isDigit(*c) || (*c == '.' && SML_ParsUtil_isDigit(*(c+1)))) { // TODO: second check is ugly as hell
        ++c;
        /* assume an integer first */
        me->nextTok.type = SML_TOK_INTEGER;
        while (SML_ParsUtil_isDigit(*c)) { 
            ++c;
        }
        /* optional decimal point and digits */
        if (*c == '.') {
            ++c;
            while (SML_ParsUtil_isDigit(*c)) { 
                ++c;
            }
            me->nextTok.type = SML_TOK_REAL;
        }
        /* save the beginning of a possible exponent */
        const char *exponent = c;
        if (*c == 'e' || *c == 'E') {
            ++c;
            if (*c == '-' || *c == '+') {
                ++c;
            }
            if (SML_ParsUtil_isDigit(*c)) {
                ++c;
                while (SML_ParsUtil_isDigit(*c)) {
                    ++c;
                }
                me->nextTok.type = SML_TOK_REAL;
            } else {
                /* invalid exponent, might be part of another token */
                c = exponent;
            }
        }
        me->nextTok.data = me->cursor;
        me->nextTok.size = c - me->cursor;
        me->cursor = c;
    } else if (*c == '(') {
        ++c;
        me->nextTok.type = SML_TOK_LPAREN;
        me->nextTok.data = me->cursor;
        me->nextTok.size = 1;
        me->cursor = c;
    } 
    else if (*c == ')') {
        ++c;
        me->nextTok.type = SML_TOK_RPAREN;
        me->nextTok.data = me->cursor;
        me->nextTok.size = 1;
        me->cursor = c;
    }
    else if (*c == '{') {
        ++c;
        me->nextTok.type = SML_TOK_LBRACE;
        me->nextTok.data = me->cursor;
        me->nextTok.size = 1;
        me->cursor = c;
    }
    else if (*c == '}') {
        ++c;
        me->nextTok.type = SML_TOK_RBRACE;
        me->nextTok.data = me->cursor;
        me->nextTok.size = 1;
        me->cursor = c;
    }
    else if (*c == '[') {
        ++c;
        me->nextTok.type = SML_TOK_LBRACKET;
        me->nextTok.data = me->cursor;
        me->nextTok.size = 1;
        me->cursor = c;
    }
    else if (*c == ']') {
        ++c;
        me->nextTok.type = SML_TOK_RBRACKET;
        me->nextTok.data = me->cursor;
        me->nextTok.size = 1;
        me->cursor = c;
    }
    else if (*c == '.') {
        ++c;
        me->nextTok.type = SML_TOK_DOT;
        me->nextTok.data = me->cursor;
        me->nextTok.size = 1;
        me->cursor = c;
    }
    else if (*c == ',') {
        ++c;
        me->nextTok.type = SML_TOK_COMMA;
        me->nextTok.data = me->cursor;
        me->nextTok.size = 1;
        me->cursor = c;
    }
    else if (*c == ':') {
        ++c;
        me->nextTok.type = SML_TOK_COLON;
        me->nextTok.data = me->cursor;
        me->nextTok.size = 1;
        me->cursor = c;
    } 
    else if (*c == ';') {
        ++c;
        me->nextTok.type = SML_TOK_SEMICOL;
        me->nextTok.data = me->cursor;
        me->nextTok.size = 1;
        me->cursor = c;
    } 
    else if (*c == '"') {
        ++c;
        while (*c != '"') { // TODO: add escape sequence!!
            if (c == me->end) {
                me->nextTok.type = SML_TOK_UNKNOWN;
                me->nextTok.data = me->cursor;
                me->nextTok.size = c - me->cursor;
                me->cursor = c;
                return me->nextTok;
            }
            ++c;
        }
        /* consume the trailing quotes */
        ++c;
        /* token only contains the string without quotes */
        me->nextTok.type = SML_TOK_STRLIT_DQUOTE;
        me->nextTok.data = me->cursor + 1;
        me->nextTok.size = c - me->cursor - 2;
        me->cursor = c;
    } 
    else if (*c == '+') {
        ++c;
        me->nextTok.type = SML_TOK_PLUS;
        me->nextTok.data = me->cursor;
        me->nextTok.size = 1;
        me->cursor = c;
    } 
    else if (*c == '-') {
        ++c;
        me->nextTok.type = SML_TOK_MINUS;
        me->nextTok.data = me->cursor;
        me->nextTok.size = 1;
        me->cursor = c;
    } 
    else if (*c == '*') {
        ++c;
        me->nextTok.type = SML_TOK_MULT;
        me->nextTok.data = me->cursor;
        me->nextTok.size = 1;
        me->cursor = c;
    }
    else if (*c == '/') {
        ++c;
        me->nextTok.type = SML_TOK_DIV;
        me->nextTok.data = me->cursor;
        me->nextTok.size = 1;
        me->cursor = c;
    } else {
        ++c;
        me->nextTok.data = me->cursor;
        me->nextTok.size = 1;
        me->nextTok.type = SML_TOK_UNKNOWN;
        me->cursor = c;
    }
    return me->nextTok;
}

SML_Token SML_Lexer_nextToken(SML_Lexer *me, bool skipInvisible)
{
    SML_Token tok;
    tok = SML_Lexer_peekToken(me, skipInvisible);

    /* advance if we are not at the end */
    if (tok.type == SML_TOK_END) {
        return tok;
    }
    me->nextTok.type = SML_TOK_UNSCANNED; 
    return tok;
}
