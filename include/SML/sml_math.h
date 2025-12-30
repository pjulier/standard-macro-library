#ifndef INCLUDE_SML_MATH_H
#define INCLUDE_SML_MATH_H

#include <math.h>

#ifdef __cplusplus
extern "C" {
#endif

static inline float SML_f32_min(float val, float min);
static inline float SML_f32_max(float val, float max);
static inline float SML_f32_clamp(float val, float min, float max);

static inline float SML_f32_min(float val, float min)
{
    if (val > min) {
        return min;
    }
    return val;
}

static inline float SML_f32_max(float val, float max)
{
    if (val < max) {
        return max;
    }
    return val;
}

static inline float SML_f32_clamp(float val, float min, float max)
{
    if(val > max){
        return max;
    } else if (val < min){
        return min;
    }
    return val;
}

#ifdef __cplusplus
}
#endif

#endif /* INCLUDE_SML_MATH_H */