#pragma once

#include <cstdint>
#include <type_traits>

namespace clarabel
{
    template<typename T = double>
    struct ClarabelSupportedConeT
    {
        static_assert(std::is_same<T, float>::value || std::is_same<T, double>::value, "T must be float or double");

        enum class Tag
        {
            ZeroConeT,
            NonnegativeConeT,
            SecondOrderConeT,
            ExponentialConeT,
            PowerConeT,
#if defined(FEATURE_SDP)
            PSDTriangleConeT,
#endif
        };

        struct ZeroConeT_Body { uintptr_t _0; };
        struct NonnegativeConeT_Body { uintptr_t _0; };
        struct SecondOrderConeT_Body { uintptr_t _0; };
        struct ExponentialConeT_Body {};
        struct PowerConeT_Body { T _0; };
#if defined(FEATURE_SDP)
        struct PSDTriangleConeT_Body { uintptr_t _0; };
#endif

        Tag tag;
        union {
            ZeroConeT_Body zero_cone_t;
            NonnegativeConeT_Body nonnegative_cone_t;
            SecondOrderConeT_Body second_order_cone_t;
            ExponentialConeT_Body exponential_cone_t;
            PowerConeT_Body power_cone_t;
#if defined(FEATURE_SDP)
            PSDTriangleConeT_Body psd_triangle_cone_t;
#endif
            // TODO: constructors
        };
    };
}