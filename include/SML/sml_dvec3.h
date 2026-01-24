#ifndef INCLUDE_SML_DVEC3_H
#define INCLUDE_SML_DVEC3_H

#include "sml_math.h"
#include "sml_math_types.h"

/*
 * Macros
 */
#define SML_DVEC3_ZERO_INIT            {0.0, 0.0, 0.0}
#define SML_DVEC3_ONE_INIT             {1.0, 1.0, 1.0}
#define SML_DVEC3_INIT(v0_, v1_, v2_)  {v0_, v1_, v2_}

#define SML_DVEC3S_ZERO_INIT           {SML_DVEC3_ZERO_INIT}
#define SML_DVEC3S_ONE_INIT            {SML_DVEC3_ONE_INIT}
#define SML_DVEC3S_INIT(v0_, v1_, v2_) {SML_DVEC3_INIT(v0_, v1_, v2_)}

#define SML_DVEC3_ZERO                 ((SMLdvec3)SML_DVEC3_ZERO_INIT)
#define SML_DVEC3_ONE                  ((SMLdvec3)SML_DVEC3_ONE_INIT)
#define SML_DVEC3(v0_, v1_, v2_)       ((SMLdvec3)SML_DVEC3_INIT(v0_, v1_, v2_))

#define SML_DVEC3S_ZERO                ((SMLdvec3s)SML_DVEC3S_ZERO_INIT)
#define SML_DVEC3S_ONE                 ((SMLdvec3s)SML_DVEC3S_ONE_INIT)
#define SML_DVEC3S(v0_, v1_, v2_)      ((SMLdvec3s)SML_DVEC3S_INIT(v0_, v1_, v2_))


#ifdef __cplusplus
extern "C" {
#endif

static inline void SML_dvec3_assign(SMLdvec3 a, double v0, double v1, double v2);
static inline void SML_dvec3_copy(SMLdvec3 a, const SMLdvec3 b);
static inline void SML_dvec3_negate(SMLdvec3 a);
static inline void SML_dvec3_negate_to(SMLdvec3 a, const SMLdvec3 b);
static inline void SML_dvec3_zero(SMLdvec3 a);
static inline void SML_dvec3_one(SMLdvec3 a);
static inline void SML_dvec3_abs(SMLdvec3 a, const SMLdvec3 b);
static inline double SML_dvec3_norm2(const SMLdvec3 a);
static inline double SML_dvec3_norm(const SMLdvec3 a);
static inline void SML_dvec3_scale(SMLdvec3 a, double s);
static inline void SML_dvec3_add(SMLdvec3 a, const SMLdvec3 b);
static inline void SML_dvec3_sadd(SMLdvec3 a, double s);
static inline void SML_dvec3_smuladd(SMLdvec3 a, const SMLdvec3 b, double s);
static inline void SML_dvec3_sub(SMLdvec3 a, const SMLdvec3 b);
static inline void SML_dvec3_ssub(SMLdvec3 a, double s);
static inline void SML_dvec3_smulsub(SMLdvec3 a, const SMLdvec3 b, double s);
static inline void SML_dvec3_sum(SMLdvec3 c, const SMLdvec3 a, const SMLdvec3 b);
static inline void SML_dvec3_smulsum(SMLdvec3 c, const SMLdvec3 a, const SMLdvec3 b, double s);
static inline void SML_dvec3_diff(SMLdvec3 c, const SMLdvec3 a, const SMLdvec3 b);
static inline void SML_dvec3_smuldiff(SMLdvec3 c, const SMLdvec3 a, const SMLdvec3 b, double s);
static inline void SML_dvec3_mul(SMLdvec3 c, const SMLdvec3 a, const SMLdvec3 b);
static inline void SML_dvec3_div(SMLdvec3 c, const SMLdvec3 a, const SMLdvec3 b);
static inline void SML_dvec3_smul(SMLdvec3 b, const SMLdvec3 a, double s);
static inline void SML_dvec3_sdiv(SMLdvec3 b, const SMLdvec3 a, double s);
static inline double SML_dvec3_dotp(const SMLdvec3 a, const SMLdvec3 b);
static inline void SML_dvec3_cross(SMLdvec3 c, const SMLdvec3 a, const SMLdvec3 b);

/**
 * @brief Assign a dvec3
 * 
 * @param a dvec3 to be assigned
 * @param v0 element 0
 * @param v1 element 1
 * @param v2 element 2
 */
static inline void SML_dvec3_assign(SMLdvec3 a, double v0, double v1, double v2)
{
    a[0] = v0;
    a[1] = v1;
    a[2] = v2;
}

/**
 * @brief Copy dvec3 to dvec3
 * 
 * a = b
 * 
 * @param a result
 * @param b dvec3 which is copied
 */
static inline void SML_dvec3_copy(SMLdvec3 a, const SMLdvec3 b)
{
    a[0] = b[0];    
    a[1] = b[1];
    a[2] = b[2];    
}

/**
 * @brief Negate a dvec3
 * 
 * a = -a
 * 
 * @param a dvec3 to negate 
 */
static inline void SML_dvec3_negate(SMLdvec3 a)
{
    SML_dvec3_negate_to(a, a);
}

/**
 * @brief Negate dvec3 b and store result in dvec3 b
 * 
 * a = -b
 * 
 * @param a result
 * @param b dvec3 to negate
 */
static inline void SML_dvec3_negate_to(SMLdvec3 a, const SMLdvec3 b)
{
    a[0] = -b[0];
    a[1] = -b[1];
    a[2] = -b[2];
}

/**
 * @brief Assign zero to each element of dvec3
 * 
 * @param a dvec3 to be assigned
 */
static inline void SML_dvec3_zero(SMLdvec3 a)
{
    a[0] = 0.0;    
    a[1] = 0.0;
    a[2] = 0.0;    
}

/**
 * @brief Assign one to each element of dvec3
 * 
 * @param a dvec3 to be assigned
 */
static inline void SML_dvec3_one(SMLdvec3 a)
{
    a[0] = 1.0;    
    a[1] = 1.0;
    a[2] = 1.0;    
}

/**
 * @brief Take the absolute of each dvec3 element
 * 
 * a = abs(b)
 * 
 * @param a result
 * @param b dvec3 whose absolute is taken
 */
static inline void SML_dvec3_abs(SMLdvec3 a, const SMLdvec3 b)
{
    a[0] = fabs(b[0]);
    a[1] = fabs(b[1]);
    a[2] = fabs(b[2]);
}

/**
 * @brief Squared L2 norm of dvec3
 * 
 * @param a dvec3 whose norm is calculated
 */
static inline double SML_dvec3_norm2(const SMLdvec3 a)
{
    return a[0] * a[0] + a[1] * a[1] + a[2] * a[2];   
}

/**
 * @brief L2 norm of dvec3
 * 
 * @param a dvec3 whose norm is calculated
 */
static inline double SML_dvec3_norm(const SMLdvec3 a)
{
    return sqrt(SML_dvec3_norm2(a));
}

/**
 * @brief Scale dvec3 by s
 * 
 * @param a dvec3 to be scaled
 * @param s scaling factor
 */
static inline void SML_dvec3_scale(SMLdvec3 a, double s)
{
    SML_dvec3_smul(a, a, s);
}

/**
 * @brief Add dvec3 to dvec3
 * 
 * a = a + b
 * 
 * @param a dvec3 to be added to
 * @param b dvec3 which is added
 */
static inline void SML_dvec3_add(SMLdvec3 a, const SMLdvec3 b)
{
    a[0] += b[0];    
    a[1] += b[1];
    a[2] += b[2];    
}

/**
 * @brief Add scalar to dvec3
 * 
 * a = a + s
 * 
 * @param a dvec3 to be added to
 * @param s scalar which is added
 */
static inline void SML_dvec3_sadd(SMLdvec3 a, double s)
{
    a[0] += s;    
    a[1] += s;
    a[2] += s;    
}

/**
 * @brief Add scaled dvec3 to dvec3
 * 
 * a = a + b * s
 * 
 * @param a dvec3 to be added to
 * @param b dvec3 which is scaled and added
 * @param s scaling factor
 */
static inline void SML_dvec3_smuladd(SMLdvec3 a, const SMLdvec3 b, double s)
{
    a[0] += b[0] * s;
    a[1] += b[1] * s;
    a[2] += b[2] * s;
}

/**
 * @brief Subtract dvec3 from dvec3
 * 
 * a = a - b
 * 
 * @param a dvec3 to be subtracted from
 * @param b dvec3 which is subtracted
 */
static inline void SML_dvec3_sub(SMLdvec3 a, const SMLdvec3 b)
{
    a[0] -= b[0];    
    a[1] -= b[1];
    a[2] -= b[2];  
}

/**
 * @brief Subtract scalar from dvec3
 * 
 * a = a - s
 * 
 * @param a dvec3 to be subtracted from
 * @param s scalar which is subtracted
 */
static inline void SML_dvec3_ssub(SMLdvec3 a, double s)
{
    a[0] -= s;    
    a[1] -= s;
    a[2] -= s;    
}

/**
 * @brief Subtract scaled dvec3 from dvec3
 * 
 * a = a - b * s
 * 
 * @param a dvec3 to be subtracted from
 * @param b dvec3 which is scaled and subtracted
 * @param s scaling factor
 */
static inline void SML_dvec3_smulsub(SMLdvec3 a, const SMLdvec3 b, double s)
{
    a[0] -= b[0] * s;
    a[1] -= b[1] * s;
    a[2] -= b[2] * s;
}

/**
 * @brief Sum of two dvec3's
 * 
 * c = a + b
 * 
 * @param c result
 * @param a dvec3 a
 * @param b dvec3 b
 */
static inline void SML_dvec3_sum(SMLdvec3 c, const SMLdvec3 a, const SMLdvec3 b)
{
    c[0] = a[0] + b[0];    
    c[1] = a[1] + b[1];
    c[2] = a[2] + b[2];    
}

/**
 * @brief Sum of dvec3 a and scaled dvec3 b
 * 
 * c = a + b * s
 * 
 * @param c result
 * @param a dvec3 a
 * @param b dvec3 b
 * @param s scalar applied to b
 */
static inline void SML_dvec3_smulsum(SMLdvec3 c, const SMLdvec3 a, const SMLdvec3 b, double s)
{
    SMLdvec3 tmp;
    SML_dvec3_smul(tmp, b, s);
    SML_dvec3_sum(c, a, tmp);
}

/**
 * @brief Difference between two dvec3's
 * 
 * c = a - b
 * 
 * @param c result
 * @param a dvec3 a
 * @param b dvec3 b
 */
static inline void SML_dvec3_diff(SMLdvec3 c, const SMLdvec3 a, const SMLdvec3 b)
{
    c[0] = a[0] - b[0];    
    c[1] = a[1] - b[1];
    c[2] = a[2] - b[2];  
}

/**
 * @brief Difference of dvec3 a and scaled dvec3 b
 * 
 * c = a - b * s
 * 
 * @param c result
 * @param a dvec3 a
 * @param b dvec3 b
 * @param s scalar applied to b
 */
static inline void SML_dvec3_smuldiff(SMLdvec3 c, const SMLdvec3 a, const SMLdvec3 b, double s)
{
    SMLdvec3 tmp;
    SML_dvec3_smul(tmp, b, s);
    SML_dvec3_diff(c, a, tmp);
}

/**
 * @brief Multiply two dvec3's element wise
 * 
 * c = a .* b
 * 
 * @param c result
 * @param a dvec3 a
 * @param b dvec3 b
 */
static inline void SML_dvec3_mul(SMLdvec3 c, const SMLdvec3 a, const SMLdvec3 b)
{
    c[0] = a[0] * b[0];    
    c[1] = a[1] * b[1];
    c[2] = a[2] * b[2];  
}

/**
 * @brief Divide dvec3 by dvec3 element wise
 * 
 * c = a ./ b
 * 
 * @param c result
 * @param a dvec3 a
 * @param b dvec3 b
 */
static inline void SML_dvec3_div(SMLdvec3 c, const SMLdvec3 a, const SMLdvec3 b)
{
    c[0] = a[0] / b[0];    
    c[1] = a[1] / b[1];
    c[2] = a[2] / b[2];  
}

/**
 * @brief Multiply dvec3 by scalar
 * 
 * b = a * s
 * 
 * @param b result
 * @param a dvec3
 * @param s scalar
 */
static inline void SML_dvec3_smul(SMLdvec3 b, const SMLdvec3 a, double s)
{
    b[0] = a[0] * s;    
    b[1] = a[1] * s;
    b[2] = a[2] * s;    
}

/**
 * @brief Divide dvec3 by scalar
 * 
 * b = a / s
 * 
 * @param c result
 * @param a dvec3
 * @param b scalar
 */
static inline void SML_dvec3_sdiv(SMLdvec3 b, const SMLdvec3 a, double s)
{
    b[0] = a[0] / s;    
    b[1] = a[1] / s;
    b[2] = a[2] / s;    
}

/**
 * @brief Dot product of two dvec3's
 * 
 * result = a * b
 * 
 * @param a dvec3 a
 * @param b dvec3 b
 * @return double dot product
 */
static inline double SML_dvec3_dotp(const SMLdvec3 a, const SMLdvec3 b)
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

/**
 * @brief Cross product of two dvec3's
 * 
 * c = a x b
 * 
 * @param c result
 * @param a dvec3 a
 * @param b dvec3 b
 */
static inline void SML_dvec3_cross(SMLdvec3 c, const SMLdvec3 a, const SMLdvec3 b)
{
    c[0] = a[1] * b[2] - a[2] * b[1];
    c[1] = a[2] * b[0] - a[0] * b[2];
    c[2] = a[0] * b[1] - a[1] * b[0];
}

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_SML_DVEC3_H */