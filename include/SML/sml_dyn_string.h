#ifndef INCLUDE_SML_DYN_STRING_H
#define INCLUDE_SML_DYN_STRING_H

#include <stddef.h>
#include <stdbool.h>

/**
 * @brief The main SML_DString type
 * 
 */
typedef struct SML_DString {
    char *begin;
    char *end;
    size_t capacity;
} SML_DString;

/**
 * @brief Dynamically allocate and default initialize a SML_DString
 * 
 * @return SML_DString* newly created string or NULL on failure
 */
SML_DString* SML_DString_create(void);

/**
 * @brief Dynamically allocate and default initialize a SML_DString with a given capacity
 * 
 * @param capacity requested capacity
 * @return SML_DString* newly created string or NULL on failure
 */
SML_DString* SML_DString_createWithCapacity(size_t capacity);

/**
 * @brief Dynamically allocate a SML_DString and initialize from a C-style string
 * 
 * @param src C-style string which is copied
 * @return SML_DString* newly created string or NULL on failure
 */
SML_DString* SML_DString_createFrom(const char *src);

/**
 * @brief Dynamically allocate a SML_DString and initialize from a span of characters
 * 
 * @param src beginning of character span which is copied
 * @param len length of character span
 * @return SML_DString* newly created string or NULL on failure
 */
SML_DString* SML_DString_createFromSpan(const char *src, size_t len);

/**
 * @brief Default initialize a SML_DString
 * 
 * @param me pointer to self
 * @return true success
 * @return false failure
 */
bool SML_DString_init(SML_DString *me);

/**
 * @brief Initialize a SML_DString with a given capacity
 * 
 * @param me pointer to self
 * @param capacity requested capacity
 * @return true success
 * @return false failure
 */
bool SML_DString_initWithCapacity(SML_DString *me, size_t capacity);

/**
 * @brief Initialize a SML_DString from a C-style string
 * 
 * @param me pointer to self
 * @param src C-style string used to initialize
 * @return true success
 * @return false failure
 */
bool SML_DString_initFrom(SML_DString *me, const char *src);

/**
 * @brief Initialize a SML_DString from a character span
 * 
 * @param me pointer to self
 * @param src beginning of character span
 * @param len length of character span
 * @return true success
 * @return false failure
 */
bool SML_DString_initFromSpan(SML_DString *me, const char *src, size_t len);

/**
 * @brief Destroy a SML_DString
 * 
 * @param me pointer to self
 */
void SML_DString_destroy(SML_DString *me);

/**
 * @brief Free a dynamically allocated SML_DString
 * 
 * @param me pointer to self
 */
void SML_DString_free(SML_DString *me);

/**
 * @brief Fill SML_DString with character value up to current size
 * 
 * @param me pointer to self
 * @param val fill value
 */
void SML_DString_fill(SML_DString *me, char val);

/**
 * @brief Push back a single character
 * 
 * @param me pointer to self
 * @param val character value
 * @return true success
 * @return false failure
 */
bool SML_DString_push_back(SML_DString *me, char val);

/**
 * @brief Append another C-style string
 * 
 * @param me pointer to self
 * @param other C-string that is appended
 * @return true success
 * @return false failure
 */
bool SML_DString_append(SML_DString *me, const char *other);

/**
 * @brief Append a character span
 * 
 * @param me pointer to self
 * @param other beginning of character span
 * @param len length of character span
 * @return true success
 * @return false failure
 */
bool SML_DString_appendSpan(SML_DString *me, const char *other, size_t len);

/**
 * @brief Insert a C-style string at given position
 * 
 * @param me pointer to self
 * @param pos position index to insert at
 * @param str C-string to insert
 * @return true success
 * @return false failure
 */
bool SML_DString_insert(SML_DString *me, size_t pos, const char *str);

/**
 * @brief Insert a character span at given position
 * 
 * @param me pointer to self
 * @param pos position index to insert at
 * @param str beginning of character span
 * @param len length of character span
 * @return true success
 * @return false failure
 */
bool SML_DString_insertSpan(SML_DString *me, size_t pos, const char *str, size_t len);

/**
 * @brief Clear a SML_DString
 * 
 * @param me pointer to self
 */
void SML_DString_clear(SML_DString *me);

/**
 * @brief Re-assign to a C-string
 * 
 * @param me pointer to self
 * @param str C-string used for assignment
 * @return true success
 * @return false failure
 */
bool SML_DString_assign(SML_DString *me, const char *str);

/**
 * @brief Re-assign to a character span
 * 
 * @param me pointer to self
 * @param str beginning of character span
 * @param len length of character span
 * @return true success
 * @return false failure
 */
bool SML_DString_assignSpan(SML_DString *me, const char *str, size_t len);

/**
 * @brief Resize a SML_DString
 * 
 * Can also shrink the string.
 * 
 * @param me pointer to self
 * @param count new size
 * @return true success
 * @return false failure
 */
bool SML_DString_resize(SML_DString *me, size_t count);

/**
 * @brief Reserve capacity for count elements without NULL termination
 * 
 * @param me pointer to self
 * @param count number of elements to reserve
 * @return true success
 * @return false failure
 */
bool SML_DString_reserve(SML_DString *me, size_t count);

/**
 * @brief Return a pointer to the beginning of internal data 
 * 
 * @param me pointer to self
 * @return char* pointer to beginning
 */
static inline char* SML_DString_begin(const SML_DString *me)
{
    return me->begin;
}

/**
 * @brief Return a pointer to one past the last element
 * 
 * @param me pointer to self
 * @return char* pointer to end
 */
static inline char* SML_DString_end(const SML_DString *me)
{
    return me->end;
}

/**
 * @brief Return the first character
 * 
 * @param me pointer to self
 * @return char first character
 */
static inline char SML_DString_front(const SML_DString *me)
{
    return *me->begin;
}

/**
 * @brief Return the last character
 * 
 * @param me pointer to self
 * @return char last character
 */
static inline char SML_DString_back(const SML_DString *me)
{
    return *(me->end - 1);
}

/**
 * @brief Return the current size/length without NULL termination
 * 
 * @param me pointer to self
 * @return size_t 
 */
static inline size_t SML_DString_size(const SML_DString *me)
{
    return me->end - me->begin;
}

/**
 * @brief Check if a string is empty
 * 
 * @param me pointer to self
 * @return true success
 * @return false failure
 */
static inline bool SML_DString_empty(const SML_DString *me)
{
    return me->end == me->begin;
}

/**
 * @brief Get the character at given position
 * 
 * @param me pointer to self
 * @param pos position index
 * @return char character at pos
 */
static inline char SML_DString_get(const SML_DString *me, size_t pos)
{
    return me->begin[pos];
}

/**
 * @brief Get a pointer to character at given position
 * 
 * @param me pointer to self
 * @param pos position index
 * @return char* pointer to character at pos
 */
static inline char* SML_DString_get_p(const SML_DString *me, size_t pos)
{
    return &me->begin[pos];
}

#endif /* INCLUDE_SML_DYN_STRING_H */
