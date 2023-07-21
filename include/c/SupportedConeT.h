#ifndef CLARABEL_SUPPORTED_CONE_T_H
#define CLARABEL_SUPPORTED_CONE_T_H

#include <stdint.h>

typedef enum ClarabelSupportedConeT_Tag
{
    /**
     * The zero cone (used for equality constraints).
     *
     * The parameter indicates the cones dimension.
     */
    ClarabelZeroConeT_Tag,
    /**
     * The nonnegative orthant.
     *
     * The parameter indicates the cones dimension.
     */
    ClarabelNonnegativeConeT_Tag,
    /**
     * The second order cone / Lorenz cone / ice-cream cone.
     *
     * The parameter indicates the cones dimension.
     */
    ClarabelSecondOrderConeT_Tag,
    /**
     * The exponential cone in R^3.
     *
     * This cone takes no parameters
     */
    ClarabelExponentialConeT_Tag,
    /**
     * The power cone in R^3.
     *
     * The parameter indicates the power.
     */
    ClarabelPowerConeT_Tag,
    /**
     * The positive semidefinite cone in triangular form.
     *
     * The parameter indicates the matrix dimension, i.e. size = n
     * means that the variable is the upper triangle of an nxn matrix.
     */
#ifdef FEATURE_SDP
    ClarabelPSDTriangleConeT_Tag,
#endif
} ClarabelSupportedConeT_Tag;

typedef struct ClarabelSupportedConeT_f64
{
    ClarabelSupportedConeT_Tag tag;
    union
    {
        struct
        {
            uintptr_t zero_cone_t;
        };
        struct
        {
            uintptr_t nonnegative_cone_t;
        };
        struct
        {
            uintptr_t second_order_cone_t;
        };
        // exponential_cone_t (empty)
        struct
        {
            double power_cone_t;
        };
#ifdef FEATURE_SDP
        struct
        {
            uintptr_t psd_triangle_cone_t;
        };
#endif
    };
} ClarabelSupportedConeT_f64;

typedef struct ClarabelSupportedConeT_f32
{
    ClarabelSupportedConeT_Tag tag;
    union
    {
        struct
        {
            uintptr_t zero_cone_t;
        };
        struct
        {
            uintptr_t nonnegative_cone_t;
        };
        struct
        {
            uintptr_t second_order_cone_t;
        };
        // exponential_cone_t (empty)
        struct
        {
            float power_cone_t;
        };
    };
} ClarabelSupportedConeT_f32;

// Enum constructors

// f64
#define ClarabelZeroConeT_f64(size) \
    ((ClarabelSupportedConeT_f64){.tag = ClarabelZeroConeT_Tag, .zero_cone_t = (uintptr_t)(size)})

#define ClarabelNonnegativeConeT_f64(size) \
    (ClarabelSupportedConeT_f64) { .tag = ClarabelNonnegativeConeT_Tag, .nonnegative_cone_t = (uintptr_t)(size) }

#define ClarabelSecondOrderConeT_f64(size) \
    ((ClarabelSupportedConeT_f64){.tag = ClarabelSecondOrderConeT_Tag, .second_order_cone_t = (uintptr_t)(size)})

#define ClarabelExponentialConeT_f64() \
    ((ClarabelSupportedConeT_f64){.tag = ClarabelExponentialConeT_Tag})

#define ClarabelPowerConeT_f64(power) \
    ((ClarabelSupportedConeT_f64){.tag = ClarabelPowerConeT_Tag, .power_cone_t = (double)(power)})

#ifdef FEATURE_SDP
#define ClarabelPSDTriangleConeT_f64(size) \
    ((ClarabelSupportedConeT_f64){.tag = ClarabelPSDTriangleConeT_Tag, .psd_triangle_cone_t = (uintptr_t)(size)})
#endif

// f32
#define ClarabelZeroConeT_f32(size) \
    ((ClarabelSupportedConeT_f32){.tag = ClarabelZeroConeT_Tag, .zero_cone_t = (uintptr_t)(size)})

#define ClarabelNonnegativeConeT_f32(size) \
    (ClarabelSupportedConeT_f32) { .tag = ClarabelNonnegativeConeT_Tag, .nonnegative_cone_t = (uintptr_t)(size) }

#define ClarabelSecondOrderConeT_f32(size) \
    ((ClarabelSupportedConeT_f32){.tag = ClarabelSecondOrderConeT_Tag, .second_order_cone_t = (uintptr_t)(size)})

#define ClarabelExponentialConeT_f32() \
    ((ClarabelSupportedConeT_f32){.tag = ClarabelExponentialConeT_Tag})

#define ClarabelPowerConeT_f32(power) \
    ((ClarabelSupportedConeT_f32){.tag = ClarabelPowerConeT_Tag, .power_cone_t = (double)(power)})

#ifdef FEATURE_SDP
#define PSDTriangleConeT_f32(size) \
    ((ClarabelSupportedConeT_f32){.tag = ClarabelPSDTriangleConeT_Tag, .psd_triangle_cone_t = (uintptr_t)(size)})
#endif

// Choose float/double
#ifdef CLARABEL_USE_FLOAT
typedef ClarabelSupportedConeT_f32 ClarabelSupportedConeT;
#define ClarabelZeroConeT(...) ClarabelZeroConeT_f32(__VA_ARGS__)
#define ClarabelNonnegativeConeT(...) ClarabelNonnegativeConeT_f32(__VA_ARGS__)
#define ClarabelSecondOrderConeT(...) ClarabelSecondOrderConeT_f32(__VA_ARGS__)
#define ClarabelExponentialConeT(...) ClarabelExponentialConeT_f32(__VA_ARGS__)
#define ClarabelPowerConeT(...) ClarabelPowerConeT_f32(__VA_ARGS__)

#ifdef FEATURE_SDP
#define ClarabelPSDTriangleConeT(...) ClarabelPSDTriangleConeT_f32(__VA_ARGS__)
#endif

#else
typedef ClarabelSupportedConeT_f64 ClarabelSupportedConeT;
#define ClarabelZeroConeT(...) ClarabelZeroConeT_f64(__VA_ARGS__)
#define ClarabelNonnegativeConeT(...) ClarabelNonnegativeConeT_f64(__VA_ARGS__)
#define ClarabelSecondOrderConeT(...) ClarabelSecondOrderConeT_f64(__VA_ARGS__)
#define ClarabelExponentialConeT(...) ClarabelExponentialConeT_f64(__VA_ARGS__)
#define ClarabelPowerConeT(...) ClarabelPowerConeT_f64(__VA_ARGS__)

#ifdef FEATURE_SDP
#define ClarabelPSDTriangleConeT(...) ClarabelPSDTriangleConeT_f64(__VA_ARGS__)
#endif /* FEATURE_SDP */

#endif /* CLARABEL_USE_FLOAT */

#endif /* CLARABEL_SUPPORTED_CONE_T_H */