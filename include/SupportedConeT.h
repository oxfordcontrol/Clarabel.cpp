#ifndef SUPPORTED_CONE_T_H
#define SUPPORTED_CONE_T_H

#include <stdint.h>

typedef enum SupportedConeT_f64_Tag
{
    /**
     * The zero cone (used for equality constraints).
     *
     * The parameter indicates the cones dimension.
     */
    ZeroConeT_f64_Tag,
    /**
     * The nonnegative orthant.
     *
     * The parameter indicates the cones dimension.
     */
    NonnegativeConeT_f64_Tag,
    /**
     * The second order cone / Lorenz cone / ice-cream cone.
     *
     * The parameter indicates the cones dimension.
     */
    SecondOrderConeT_f64_Tag,
    /**
     * The exponential cone in R^3.
     *
     * This cone takes no parameters
     */
    ExponentialConeT_f64_Tag,
    /**
     * The power cone in R^3.
     *
     * The parameter indicates the power.
     */
    PowerConeT_f64_Tag,
    /**
     * The positive semidefinite cone in triangular form.
     *
     * The parameter indicates the matrix dimension, i.e. size = n
     * means that the variable is the upper triangle of an nxn matrix.
     */
    PSDTriangleConeT_f64_Tag,
} SupportedConeT_f64_Tag;

typedef struct ExponentialConeT_Body_f64
{

} ExponentialConeT_Body_f64;

typedef struct SupportedConeT_f64
{
    SupportedConeT_f64_Tag tag;
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
        ExponentialConeT_Body_f64 exponential_cone_t;
        struct
        {
            double power_cone_t;
        };
        struct
        {
            uintptr_t psd_triangle_cone_t;
        };
    };
} SupportedConeT_f64;

// Enum constructors
#define ZeroConeT_f64(size) \
    ((SupportedConeT_f64){.tag = ZeroConeT_f64_Tag, .zero_cone_t = (uintptr_t)(size)})

#define NonnegativeConeT_f64(size) \
    (SupportedConeT_f64){.tag = NonnegativeConeT_f64_Tag, .nonnegative_cone_t = (uintptr_t)(size)}

#define SecondOrderConeT_f64(size) \
    ((SupportedConeT_f64){.tag = SecondOrderConeT_f64_Tag, .second_order_cone_t = (uintptr_t)(size)})

#define ExponentialConeT_f64() \
    ((SupportedConeT_f64){.tag = ExponentialConeT_f64_Tag})

#define PowerConeT_f64(power) \
    ((SupportedConeT_f64){.tag = PowerConeT_f64_Tag, .power_cone_t = (double)(power)})

// TODO: PSDTriangleConeT

#endif /* SUPPORTED_CONE_T_H */