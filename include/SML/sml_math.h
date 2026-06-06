#ifndef INCLUDE_SML_MATH_H
#define INCLUDE_SML_MATH_H

#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

static inline float SML_flt_min(float a, float b);
static inline float SML_flt_max(float a, float b);
static inline float SML_flt_clamp(float val, float min, float max);

static inline double SML_dbl_min(double a, double b);
static inline double SML_dbl_max(double a, double b);
static inline double SML_dbl_clamp(double val, double min, double max);

static inline size_t SML_size_min(size_t a, size_t b);
static inline size_t SML_size_max(size_t a, size_t b);
static inline size_t SML_size_clamp(size_t val, size_t min, size_t max);


/**
 * @brief This does what you think it does
 * 
 * @param a value a
 * @param b value b
 * @return float lower value of a and b
 */
static inline float SML_flt_min(float a, float b)
{
    if (a < b) {
        return a;
    }
    return b;
}

/**
 * @brief This does what you think it does
 * 
 * @param a value a
 * @param b value b
 * @return float higher value of a and b
 */
static inline float SML_flt_max(float a, float b)
{
    if (a > b) {
        return a;
    }
    return b;
}

/**
 * @brief Clamp value to lower and upper limit
 * 
 * @param val value to be limited
 * @param min lower limit
 * @param max upper limit
 * @return float clamped value
 */
static inline float SML_flt_clamp(float val, float min, float max)
{
    if (val > max) {
        return max;
    } else if (val < min) {
        return min;
    }
    return val;
}

/**
 * @brief This does what you think it does
 * 
 * @param a value a
 * @param b value b
 * @return double lower value of a and b
 */
static inline double SML_dbl_min(double a, double b)
{
    if (a < b) {
        return a;
    }
    return b;
}

/**
 * @brief This does what you think it does
 * 
 * @param a value a
 * @param b value b
 * @return double higher value of a and b
 */
static inline double SML_dbl_max(double a, double b)
{
    if (a > b) {
        return a;
    }
    return b;
}

/**
 * @brief Clamp value to lower and upper limit
 * 
 * @param val value to be limited
 * @param min lower limit
 * @param max upper limit
 * @return double clamped value
 */
static inline double SML_dbl_clamp(double val, double min, double max)
{
    if (val > max) {
        return max;
    } else if (val < min) {
        return min;
    }
    return val;
}

/**
 * @brief This does what you think it does
 * 
 * @param a value a
 * @param b value b
 * @return size_t lower value of a and b
 */
static inline size_t SML_size_min(size_t a, size_t b)
{
    if (a < b) {
        return a;
    }
    return b;
}

/**
 * @brief This does what you think it does
 * 
 * @param a value a
 * @param b value b
 * @return size_t higher value of a and b
 */
static inline size_t SML_size_max(size_t a, size_t b)
{
    if (a > b) {
        return a;
    }
    return b;
}

/**
 * @brief Clamp value to lower and upper limit
 * 
 * @param val value to be limited
 * @param min lower limit
 * @param max upper limit
 * @return size_t clamped value
 */
static inline size_t SML_size_clamp(size_t val, size_t min, size_t max)
{
    if (val > max) {
        return max;
    } else if (val < min) {
        return min;
    }
    return val;
}

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_SML_MATH_H */
