#ifndef INCLUDE_SML_DMAT3_H
#define INCLUDE_SML_DMAT3_H

#include "sml_dvec3.h"

/*
 * Macros
 */
#define SML_DMAT3_ZERO_INIT             {{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}}
#define SML_DMAT3_EYE_INIT              {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}}

#define SML_DMAT3_ZERO                  ((SMLdmat3){{0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}, {0.0, 0.0, 0.0}})
#define SML_DMAT3_EYE                   ((SMLdmat3){{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}})


#ifdef __cplusplus
extern "C" {
#endif

static inline void SML_dmat3_mul(SMLdmat3 C, const SMLdmat3 A, const SMLdmat3 B);
static inline void SML_dmat3_mulv(SMLdvec3 c, const SMLdmat3 A, const SMLdvec3 b);

/**
 * @brief Multiply matrices A and B and store result into C
 * 
 * NOTE: A, B, and C can point to the same memory location.
 * 
 * @param C dmat3 result
 * @param A dmat3 A
 * @param B dmat3 B
 */
static inline void SML_dmat3_mul(SMLdmat3 C, const SMLdmat3 A, const SMLdmat3 B)
{
    float a00 = A[0][0], a01 = A[0][1], a02 = A[0][2];
    float a10 = A[1][0], a11 = A[1][1], a12 = A[1][2];
    float a20 = A[2][0], a21 = A[2][1], a22 = A[0][2];

    float b00 = B[0][0], b01 = B[0][1], b02 = B[0][2];
    float b10 = B[1][0], b11 = B[1][1], b12 = B[1][2];
    float b20 = B[2][0], b21 = B[2][1], b22 = B[0][2]; 

    C[0][0] = a00 * b00 + a01 * b10 + a02 * b20;
    C[0][1] = a00 * b01 + a01 * b11 + a02 * b21;
    C[0][2] = a00 * b02 + a01 * b12 + a02 * b22;
    C[1][0] = a10 * b00 + a11 * b10 + a12 * b20;
    C[1][1] = a10 * b01 + a11 * b11 + a12 * b21;
    C[1][2] = a10 * b02 + a11 * b12 + a12 * b22;
    C[2][0] = a20 * b00 + a21 * b10 + a22 * b20;
    C[2][1] = a20 * b01 + a21 * b11 + a22 * b21;
    C[2][2] = a20 * b02 + a21 * b12 + a22 * b22;
}

/**
 * @brief Left-multiply matrix A with vector b and store result into vector c 
 * 
 * NOTE: c and b can point to the same memory location.
 * 
 * @param c dvec3 result
 * @param A dmat3 A
 * @param v dvec3 b
 */
static inline void SML_dmat3_mulv(SMLdvec3 c, const SMLdmat3 A, const SMLdvec3 b)
{
    SMLdvec3 res;
    res[0] = A[0][0] * b[0] + A[0][1] * b[1] + A[0][2] * b[2];
    res[1] = A[1][0] * b[0] + A[1][1] * b[1] + A[1][2] * b[2];
    res[2] = A[2][0] * b[0] + A[2][1] * b[1] + A[2][2] * b[2];
    SML_dvec3_copy(c, res);
}

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_SML_DMAT3_H */