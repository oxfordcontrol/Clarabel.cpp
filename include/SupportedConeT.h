#ifndef SUPPORTED_CONE_T_H
#define SUPPORTED_CONE_T_H

#include <stdint.h>

typedef enum SupportedConeT_Tag
{
    /**
     * The zero cone (used for equality constraints).
     *
     * The parameter indicates the cones dimension.
     */
    ZeroConeT_Tag,
    /**
     * The nonnegative orthant.
     *
     * The parameter indicates the cones dimension.
     */
    NonnegativeConeT_Tag,
    /**
     * The second order cone / Lorenz cone / ice-cream cone.
     *
     * The parameter indicates the cones dimension.
     */
    SecondOrderConeT_Tag,
    /**
     * The exponential cone in R^3.
     *
     * This cone takes no parameters
     */
    ExponentialConeT_Tag,
    /**
     * The power cone in R^3.
     *
     * The parameter indicates the power.
     */
    PowerConeT_Tag,
    /**
     * The positive semidefinite cone in triangular form.
     *
     * The parameter indicates the matrix dimension, i.e. size = n
     * means that the variable is the upper triangle of an nxn matrix.
     */
#ifdef FEATURE_SDP
    PSDTriangleConeT_Tag,
#endif
} SupportedConeT_Tag;

typedef struct SupportedConeT
{
    SupportedConeT_Tag tag;
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
} SupportedConeT;

typedef enum SupportedConeT_f32_Tag
{
    /**
     * The zero cone (used for equality constraints).
     *
     * The parameter indicates the cones dimension.
     */
    ZeroConeT_f32_Tag,
    /**
     * The nonnegative orthant.
     *
     * The parameter indicates the cones dimension.
     */
    NonnegativeConeT_f32_Tag,
    /**
     * The second order cone / Lorenz cone / ice-cream cone.
     *
     * The parameter indicates the cones dimension.
     */
    SecondOrderConeT_f32_Tag,
    /**
     * The exponential cone in R^3.
     *
     * This cone takes no parameters
     */
    ExponentialConeT_f32_Tag,
    /**
     * The power cone in R^3.
     *
     * The parameter indicates the power.
     */
    PowerConeT_f32_Tag,
    /**
     * The positive semidefinite cone in triangular form.
     *
     * The parameter indicates the matrix dimension, i.e. size = n
     * means that the variable is the upper triangle of an nxn matrix.
     */
#ifdef FEATURE_SDP
    PSDTriangleConeT_f32_Tag,
#endif
} SupportedConeT_f32_Tag;

typedef struct SupportedConeT_f32
{
    SupportedConeT_f32_Tag tag;
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
} SupportedConeT_f32;

// Enum constructors

// f64
#define ZeroConeT(size) \
    ((SupportedConeT){.tag = ZeroConeT_Tag, .zero_cone_t = (uintptr_t)(size)})

#define NonnegativeConeT(size) \
    (SupportedConeT){.tag = NonnegativeConeT_Tag, .nonnegative_cone_t = (uintptr_t)(size)}

#define SecondOrderConeT(size) \
    ((SupportedConeT){.tag = SecondOrderConeT_Tag, .second_order_cone_t = (uintptr_t)(size)})

#define ExponentialConeT() \
    ((SupportedConeT){.tag = ExponentialConeT_Tag})

#define PowerConeT(power) \
    ((SupportedConeT){.tag = PowerConeT_Tag, .power_cone_t = (double)(power)})

#ifdef FEATURE_SDP
#define PSDTriangleConeT(size) \
    ((SupportedConeT){.tag = PSDTriangleConeT_Tag, .psd_triangle_cone_t = (uintptr_t)(size)})
#endif

// f32
#define ZeroConeT_f32(size) \
    ((SupportedConeT_f32){.tag = ZeroConeT_f32_Tag, .zero_cone_t = (uintptr_t)(size)})

#define NonnegativeConeT_f32(size) \
    (SupportedConeT_f32) { .tag = NonnegativeConeT_f32_Tag, .nonnegative_cone_t = (uintptr_t)(size) }

#define SecondOrderConeT_f32(size) \
    ((SupportedConeT_f32){.tag = SecondOrderConeT_f32_Tag, .second_order_cone_t = (uintptr_t)(size)})

#define ExponentialConeT_f32() \
    ((SupportedConeT_f32){.tag = ExponentialConeT_f32_Tag})

#define PowerConeT_f32(power) \
    ((SupportedConeT_f32){.tag = PowerConeT_f32_Tag, .power_cone_t = (double)(power)})

#ifdef FEATURE_SDP
#define PSDTriangleConeT_f32(size) \
    ((SupportedConeT_f32){.tag = PSDTriangleConeT_f32_Tag, .psd_triangle_cone_t = (uintptr_t)(size)})
#endif

#endif /* SUPPORTED_CONE_T_H */