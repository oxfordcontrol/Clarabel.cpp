#pragma once

#include <cstdint>
#include <type_traits>

namespace clarabel
{
    // Layout for SupportedConeT object in Rust

    template<typename T = double>
    struct SupportedConeT
    {
        static_assert(std::is_same<T, float>::value || std::is_same<T, double>::value, "T must be float or double");

        // Tag for the type of the cone
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
        Tag tag;

    private:
        struct ZeroConeT_Body { uintptr_t _0; };
        struct NonnegativeConeT_Body { uintptr_t _0; };
        struct SecondOrderConeT_Body { uintptr_t _0; };
        struct ExponentialConeT_Body {};
        struct PowerConeT_Body { T _0; };
#if defined(FEATURE_SDP)
        struct PSDTriangleConeT_Body { uintptr_t _0; };
#endif
        // Data
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

    // Concrete cone types
    template<typename T = double>
    struct ZeroConeT : public SupportedConeT<T>
    {
    public:
        ZeroConeT(uintptr_t dimension)
        {
            this->tag = SupportedConeT<T>::Tag::ZeroConeT;
            this->nonnegative_cone_t = { dimension };
        }

        uintptr_t dimension() { return this->nonnegative_cone_t._0; }
    };

    template<typename T = double>
    struct NonnegativeConeT : public SupportedConeT<T>
    {
    public:
        NonnegativeConeT(uintptr_t dimension)
        {
            this->tag = SupportedConeT<T>::Tag::NonnegativeConeT;
            this->zero_cone_t = { dimension };
        }

        uintptr_t dimension() { return this->zero_cone_t._0; }
    };

    template<typename T = double>
    struct PowerConeT : public SupportedConeT<T>
    {
    public:
        PowerConeT(T power)
        {
            this->tag = SupportedConeT<T>::Tag::PowerConeT;
            this->power_cone_t = { power };
        }

        T power() { return this->power_cone_t._0; }
    };
}