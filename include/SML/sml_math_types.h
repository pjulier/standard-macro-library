#ifndef INCLUDE_SML_MATH_TYPES_H
#define INCLUDE_SML_MATH_TYPES_H

/*
 * Constants
 */
#define SML_E         2.71828182845904523536028747135266250   /* e           */
#define SML_LOG2E     1.44269504088896340735992468100189214   /* log2(e)     */
#define SML_LOG10E    0.434294481903251827651128918916605082  /* log10(e)    */
#define SML_LN2       0.693147180559945309417232121458176568  /* loge(2)     */
#define SML_LN10      2.30258509299404568401799145468436421   /* loge(10)    */
#define SML_PI        3.14159265358979323846264338327950288   /* pi          */
#define SML_PI_2      1.57079632679489661923132169163975144   /* pi/2        */
#define SML_PI_4      0.785398163397448309615660845819875721  /* pi/4        */
#define SML_1_PI      0.318309886183790671537767526745028724  /* 1/pi        */
#define SML_2_PI      0.636619772367581343075535053490057448  /* 2/pi        */
#define SML_2_SQRTPI  1.12837916709551257389615890312154517   /* 2/sqrt(pi)  */
#define SML_SQRT2     1.41421356237309504880168872420969808   /* sqrt(2)     */
#define SML_SQRT1_2   0.707106781186547524400844362104849039  /* 1/sqrt(2)   */

#define SML_Ef        ((float)SML_E)
#define SML_LOG2Ef    ((float)SML_LOG2E)
#define SML_LOG10Ef   ((float)SML_LOG10E)
#define SML_LN2f      ((float)SML_LN2)
#define SML_LN10f     ((float)SML_LN10)
#define SML_PIf       ((float)SML_PI)
#define SML_PI_2f     ((float)SML_PI_2)
#define SML_PI_4f     ((float)SML_PI_4)
#define SML_1_PIf     ((float)SML_1_PI)
#define SML_2_PIf     ((float)SML_2_PI)
#define SML_2_SQRTPIf ((float)SML_2_SQRTPI)
#define SML_SQRT2f    ((float)SML_SQRT2)
#define SML_SQRT1_2f  ((float)SML_SQRT1_2)

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

/**
 * @brief The double precision vec3 type
 * 
 */
typedef double SMLdvec3[3];

/**
 * @brief The double precision vec4 type
 * 
 */
typedef double SMLdvec4[4];

/**
 * @brief SMLdvec3 type wrapped in a union to enable struct syntax
 * 
 */
typedef union SMLdvec3s {
    SMLdvec3 raw;
    struct {
        double x;
        double y;
        double z;
    } v;
} SMLdvec3s;

/**
 * @brief SMLdvec4 type wrapped in a union to enable struct syntax
 * 
 */
typedef union SMLdvec4s {
    SMLdvec4 raw;
    struct {
        double x;
        double y;
        double z;
        double w;
    } v;
} SMLdvec4s;

/**
 * @brief The 3x3 matrix type
 * 
 */
typedef SMLvec3 SMLmat3[3];

/**
 * @brief The double precision 3x3 matrix type
 * 
 */
typedef SMLdvec3 SMLdmat3[3];

#endif /* INCLUDE_SML_MATH_TYPES_H */