#ifndef INCLUDE_SML_MATH_TYPES_H
#define INCLUDE_SML_MATH_TYPES_H

/**
 * @brief The main vec3 type
 * 
 */
typedef float SMLvec3[3];

/**
 * @brief The main vec4 type
 * 
 */
typedef float SMLvec4[4];

/**
 * @brief vec3 type wrapped in a union to enable struct syntax
 * 
 */
typedef union SMLvec3s {
    SMLvec3 raw;
    struct {
        float x;
        float y;
        float z;
    } v;
} SMLvec3s;

/**
 * @brief vec4 type wrapped in a union to enable struct syntax
 * 
 */
typedef union SMLvec4s {
    SMLvec4 raw;
    struct {
        float x;
        float y;
        float z;
        float w;
    } v;
} SMLvec4s;

#endif /* INCLUDE_SML_MATH_TYPES_H */