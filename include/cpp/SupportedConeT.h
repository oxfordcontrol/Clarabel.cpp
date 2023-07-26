#pragma once

#include <cstdint>
#include <type_traits>

namespace clarabel
{
    // Layout for SupportedConeT object in Rust
    namespace rust_ffi
    {
        template<typename T = double>
        struct C_SupportedConeT
        {
            static_assert(std::is_same<T, float>::value || std::is_same<T, double>::value, "T must be float or double");

            // Types
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
            // Members
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
            };
        };
    }

    // Abstract base class for all cones
    template<typename T = double>
    class SupportedConeT
    {
        static_assert(std::is_same<T, float>::value || std::is_same<T, double>::value, "T must be float or double");

    protected:
        rust_ffi::C_SupportedConeT<T> cone;

    public:
        ~SupportedConeT() = default;
        rust_ffi::C_SupportedConeT<T> get_cone() const { return cone; }
    };

    // Concrete cone types
    template<typename T = double>
    class ZeroConeT : public SupportedConeT<T>
    {
    public:
        ZeroConeT(uintptr_t dimension)
        {
            this->cone = { .tag = rust_ffi::C_SupportedConeT<double>::Tag::ZeroConeT, .nonnegative_cone_t = {._0 = dimension } };
        }

        uintptr_t dimension() { return this->cone.nonnegative_cone_t._0; }
    };

    template<typename T = double>
    class NonnegativeConeT : public SupportedConeT<T>
    {
    public:
        NonnegativeConeT(uintptr_t dimension)
        {
            this->cone = { .tag = rust_ffi::C_SupportedConeT<double>::Tag::NonnegativeConeT, .zero_cone_t = {._0 = dimension } };
        }

        uintptr_t dimension() { return this->cone.zero_cone_t._0; }
    };
}