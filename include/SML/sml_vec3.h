#ifndef INCLUDE_SML_VEC3_H
#define INCLUDE_SML_VEC3_H

#include "sml_math.h"
#include "sml_math_types.h"

/*
 * Macros
 */
#define SML_VEC3_ZERO_INIT            {0.0f, 0.0f, 0.0f}
#define SML_VEC3_ONE_INIT             {1.0f, 1.0f, 1.0f}
#define SML_VEC3_INIT(v0_, v1_, v2_)  {v0_, v1_, v2_}

#define SML_VEC3S_ZERO_INIT           {SML_VEC3_ZERO_INIT}
#define SML_VEC3S_ONE_INIT            {SML_VEC3_ONE_INIT}
#define SML_VEC3S_INIT(v0_, v1_, v2_) {SML_VEC3_INIT(v0_, v1_, v2_)}

#define SML_VEC3_ZERO                 ((SMLvec3)SML_VEC3_ZERO_INIT)
#define SML_VEC3_ONE                  ((SMLvec3)SML_VEC3_ONE_INIT)
#define SML_VEC3(v0_, v1_, v2_)       ((SMLvec3)SML_VEC3_INIT(v0_, v1_, v2_))

#define SML_VEC3S_ZERO                ((SMLvec3s)SML_VEC3S_ZERO_INIT)
#define SML_VEC3S_ONE                 ((SMLvec3s)SML_VEC3S_ONE_INIT)
#define SML_VEC3S(v0_, v1_, v2_)      ((SMLvec3s)SML_VEC3S_INIT(v0_, v1_, v2_))


#ifdef __cplusplus
extern "C" {
#endif

static inline void SML_vec3_assign(SMLvec3 a, float v0, float v1, float v2);
static inline void SML_vec3_copy(SMLvec3 a, const SMLvec3 b);
static inline void SML_vec3_negate(SMLvec3 a);
static inline void SML_vec3_negate_to(SMLvec3 a, const SMLvec3 b);
static inline void SML_vec3_zero(SMLvec3 a);
static inline void SML_vec3_one(SMLvec3 a);
static inline void SML_vec3_abs(SMLvec3 a, const SMLvec3 b);
static inline float SML_vec3_norm2(const SMLvec3 a);
static inline float SML_vec3_norm(const SMLvec3 a);
static inline void SML_vec3_scale(SMLvec3 a, float s);
static inline void SML_vec3_add(SMLvec3 a, const SMLvec3 b);
static inline void SML_vec3_sadd(SMLvec3 a, float s);
static inline void SML_vec3_smuladd(SMLvec3 a, const SMLvec3 b, float s);
static inline void SML_vec3_sub(SMLvec3 a, const SMLvec3 b);
static inline void SML_vec3_ssub(SMLvec3 a, float s);
static inline void SML_vec3_smulsub(SMLvec3 a, const SMLvec3 b, float s);
static inline void SML_vec3_sum(SMLvec3 c, const SMLvec3 a, const SMLvec3 b);
static inline void SML_vec3_smulsum(SMLvec3 c, const SMLvec3 a, const SMLvec3 b, float s);
static inline void SML_vec3_diff(SMLvec3 c, const SMLvec3 a, const SMLvec3 b);
static inline void SML_vec3_smuldiff(SMLvec3 c, const SMLvec3 a, const SMLvec3 b, float s);
static inline void SML_vec3_mul(SMLvec3 c, const SMLvec3 a, const SMLvec3 b);
static inline void SML_vec3_div(SMLvec3 c, const SMLvec3 a, const SMLvec3 b);
static inline void SML_vec3_smul(SMLvec3 b, const SMLvec3 a, float s);
static inline void SML_vec3_sdiv(SMLvec3 b, const SMLvec3 a, float s);
static inline float SML_vec3_dotp(const SMLvec3 a, const SMLvec3 b);
static inline void SML_vec3_cross(SMLvec3 c, const SMLvec3 a, const SMLvec3 b);

/**
 * @brief Assign a vec3
 * 
 * @param a vec3 to be assigned
 * @param v0 element 0
 * @param v1 element 1
 * @param v2 element 2
 */
static inline void SML_vec3_assign(SMLvec3 a, float v0, float v1, float v2)
{
    a[0] = v0;
    a[1] = v1;
    a[2] = v2;
}

/**
 * @brief Copy vec3 to vec3
 * 
 * a = b
 * 
 * @param a result
 * @param b vec3 which is copied
 */
static inline void SML_vec3_copy(SMLvec3 a, const SMLvec3 b)
{
    a[0] = b[0];    
    a[1] = b[1];
    a[2] = b[2];    
}

/**
 * @brief Negate a vec3
 * 
 * a = -a
 * 
 * @param a vec3 to negate 
 */
static inline void SML_vec3_negate(SMLvec3 a)
{
    SML_vec3_negate_to(a, a);
}

/**
 * @brief Negate vec3 b and store result in vec3 b
 * 
 * a = -b
 * 
 * @param a result
 * @param b vec3 to negate
 */
static inline void SML_vec3_negate_to(SMLvec3 a, const SMLvec3 b)
{
    a[0] = -b[0];
    a[1] = -b[1];
    a[2] = -b[2];
}

/**
 * @brief Assign zero to each element of vec3
 * 
 * @param a vec3 to be assigned
 */
static inline void SML_vec3_zero(SMLvec3 a)
{
    a[0] = 0.0f;    
    a[1] = 0.0f;
    a[2] = 0.0f;    
}

/**
 * @brief Assign one to each element of vec3
 * 
 * @param a vec3 to be assigned
 */
static inline void SML_vec3_one(SMLvec3 a)
{
    a[0] = 1.0f;    
    a[1] = 1.0f;
    a[2] = 1.0f;    
}

/**
 * @brief Take the absolute of each vec3 element
 * 
 * a = abs(b)
 * 
 * @param a result
 * @param b vec3 whose absolute is taken
 */
static inline void SML_vec3_abs(SMLvec3 a, const SMLvec3 b)
{
    a[0] = fabs(b[0]);
    a[1] = fabs(b[1]);
    a[2] = fabs(b[2]);
}

/**
 * @brief Squared L2 norm of vec3
 * 
 * @param a vec3 whose norm is calculated
 */
static inline float SML_vec3_norm2(const SMLvec3 a)
{
    return a[0] * a[0] + a[1] * a[1] + a[2] * a[2];   
}

/**
 * @brief L2 norm of vec3
 * 
 * @param a vec3 whose norm is calculated
 */
static inline float SML_vec3_norm(const SMLvec3 a)
{
    return sqrtf(SML_vec3_norm2(a));
}

/**
 * @brief Scale vec3 by s
 * 
 * @param a vec3 to be scaled
 * @param s scaling factor
 */
static inline void SML_vec3_scale(SMLvec3 a, float s)
{
    SML_vec3_smul(a, a, s);
}

/**
 * @brief Add vec3 to vec3
 * 
 * a = a + b
 * 
 * @param a vec3 to be added to
 * @param b vec3 which is added
 */
static inline void SML_vec3_add(SMLvec3 a, const SMLvec3 b)
{
    a[0] += b[0];    
    a[1] += b[1];
    a[2] += b[2];    
}

/**
 * @brief Add scalar to vec3
 * 
 * a = a + s
 * 
 * @param a vec3 to be added to
 * @param s scalar which is added
 */
static inline void SML_vec3_sadd(SMLvec3 a, float s)
{
    a[0] += s;    
    a[1] += s;
    a[2] += s;    
}

/**
 * @brief Add scaled vec3 to vec3
 * 
 * a = a + b * s
 * 
 * @param a vec3 to be added to
 * @param b vec3 which is scaled and added
 * @param s scaling factor
 */
static inline void SML_vec3_smuladd(SMLvec3 a, const SMLvec3 b, float s)
{
    a[0] += b[0] * s;
    a[1] += b[1] * s;
    a[2] += b[2] * s;
}


/**
 * @brief Subtract vec3 from vec3
 * 
 * a = a - b
 * 
 * @param a vec3 to be subtracted from
 * @param b vec3 which is subtracted
 */
static inline void SML_vec3_sub(SMLvec3 a, const SMLvec3 b)
{
    a[0] -= b[0];    
    a[1] -= b[1];
    a[2] -= b[2];  
}

/**
 * @brief Subtract scalar from vec3
 * 
 * a = a - s
 * 
 * @param a vec3 to be subtracted from
 * @param s scalar which is subtracted
 */
static inline void SML_vec3_ssub(SMLvec3 a, float s)
{
    a[0] -= s;    
    a[1] -= s;
    a[2] -= s;    
}

/**
 * @brief Subtract scaled vec3 from vec3
 * 
 * a = a - b * s
 * 
 * @param a vec3 to be subtracted from
 * @param b vec3 which is scaled and subtracted
 * @param s scaling factor
 */
static inline void SML_vec3_smulsub(SMLvec3 a, const SMLvec3 b, float s)
{
    a[0] -= b[0] * s;
    a[1] -= b[1] * s;
    a[2] -= b[2] * s;
}

/**
 * @brief Sum of two vec3's
 * 
 * c = a + b
 * 
 * @param c result
 * @param a vec3 a
 * @param b vec3 b
 */
static inline void SML_vec3_sum(SMLvec3 c, const SMLvec3 a, const SMLvec3 b)
{
    c[0] = a[0] + b[0];    
    c[1] = a[1] + b[1];
    c[2] = a[2] + b[2];    
}

/**
 * @brief Sum of vec3 a and scaled vec3 b
 * 
 * c = a + b * s
 * 
 * @param c result
 * @param a vec3 a
 * @param b vec3 b
 * @param s scalar applied to b
 */
static inline void SML_vec3_smulsum(SMLvec3 c, const SMLvec3 a, const SMLvec3 b, float s)
{
    SMLvec3 tmp;
    SML_vec3_smul(tmp, b, s);
    SML_vec3_sum(c, a, tmp);
}

/**
 * @brief Difference between two vec3's
 * 
 * c = a - b
 * 
 * @param c result
 * @param a vec3 a
 * @param b vec3 b
 */
static inline void SML_vec3_diff(SMLvec3 c, const SMLvec3 a, const SMLvec3 b)
{
    c[0] = a[0] - b[0];    
    c[1] = a[1] - b[1];
    c[2] = a[2] - b[2];  
}

/**
 * @brief Sum of vec3 a and scaled vec3 b
 * 
 * c = a - b * s
 * 
 * @param c result
 * @param a vec3 a
 * @param b vec3 b
 * @param s scalar applied to b
 */
static inline void SML_vec3_smuldiff(SMLvec3 c, const SMLvec3 a, const SMLvec3 b, float s)
{
    SMLvec3 tmp;
    SML_vec3_smul(tmp, b, s);
    SML_vec3_diff(c, a, tmp);
}

/**
 * @brief Multiply two vec3's element wise
 * 
 * c = a .* b
 * 
 * @param c result
 * @param a vec3 a
 * @param b vec3 b
 */
static inline void SML_vec3_mul(SMLvec3 c, const SMLvec3 a, const SMLvec3 b)
{
    c[0] = a[0] * b[0];    
    c[1] = a[1] * b[1];
    c[2] = a[2] * b[2];  
}

/**
 * @brief Divide vec3 by vec3 element wise
 * 
 * c = a ./ b
 * 
 * @param c result
 * @param a vec3 a
 * @param b vec3 b
 */
static inline void SML_vec3_div(SMLvec3 c, const SMLvec3 a, const SMLvec3 b)
{
    c[0] = a[0] / b[0];    
    c[1] = a[1] / b[1];
    c[2] = a[2] / b[2];  
}

/**
 * @brief Multiply vec3 by scalar
 * 
 * b = a * s
 * 
 * @param b result
 * @param a vec3
 * @param s scalar
 */
static inline void SML_vec3_smul(SMLvec3 b, const SMLvec3 a, float s)
{
    b[0] = a[0] * s;    
    b[1] = a[1] * s;
    b[2] = a[2] * s;    
}

/**
 * @brief Divide vec3 by scalar
 * 
 * b = a / s
 * 
 * @param c result
 * @param a vec3
 * @param b scalar
 */
static inline void SML_vec3_sdiv(SMLvec3 b, const SMLvec3 a, float s)
{
    b[0] = a[0] / s;    
    b[1] = a[1] / s;
    b[2] = a[2] / s;    
}

/**
 * @brief Dot product of two vec3's
 * 
 * result = a * b
 * 
 * @param a vec3 a
 * @param b vec3 b
 * @return float dot product
 */
static inline float SML_vec3_dotp(const SMLvec3 a, const SMLvec3 b)
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

/**
 * @brief Cross product of two vec3's
 * 
 * c = a x b
 * 
 * @param c result
 * @param a vec3 a
 * @param b vec3 b
 */
static inline void SML_vec3_cross(SMLvec3 c, const SMLvec3 a, const SMLvec3 b)
{
    c[0] = a[1] * b[2] - a[2] * b[1];
    c[1] = a[2] * b[0] - a[0] * b[2];
    c[2] = a[0] * b[1] - a[1] * b[0];
}

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_SML_VEC3_H */