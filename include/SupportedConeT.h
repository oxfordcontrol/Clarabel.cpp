#ifndef SUPPORTED_CONE_T_H
#define SUPPORTED_CONE_T_H

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
    ClarabelPSDTriangleConeT_Tag,
} ClarabelSupportedConeT_Tag;

typedef struct ClarabelSupportedConeT
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
        struct
        {
            uintptr_t psd_triangle_cone_t;
        };
    };
} ClarabelSupportedConeT;

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
#define ClarabelZeroConeT(size) \
    ((ClarabelSupportedConeT){.tag = ClarabelZeroConeT_Tag, .zero_cone_t = (uintptr_t)(size)})

#define ClarabelNonnegativeConeT(size) \
    (ClarabelSupportedConeT){.tag = ClarabelNonnegativeConeT_Tag, .nonnegative_cone_t = (uintptr_t)(size)}

#define ClarabelSecondOrderConeT(size) \
    ((ClarabelSupportedConeT){.tag = ClarabelSecondOrderConeT_Tag, .second_order_cone_t = (uintptr_t)(size)})

#define ClarabelExponentialConeT() \
    ((ClarabelSupportedConeT){.tag = ClarabelExponentialConeT_Tag})

#define ClarabelPowerConeT(power) \
    ((ClarabelSupportedConeT){.tag = ClarabelPowerConeT_Tag, .power_cone_t = (double)(power)})

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

// TODO: PSDTriangleConeT

#endif /* SUPPORTED_CONE_T_H */