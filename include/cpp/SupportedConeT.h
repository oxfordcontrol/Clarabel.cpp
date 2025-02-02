#pragma once

#include <cstdint>
#include <stdexcept>
#include <type_traits>

namespace clarabel
{

// Provides the layout for SupportedConeT objects in Rust
// Base type of all cones
template<typename T = double>
struct SupportedConeT
{
    static_assert(std::is_same<T, float>::value || std::is_same<T, double>::value, "T must be float or double");

  public:
    // Tag for the type of the cone
    enum class Tag
    {
        ZeroConeT,
        NonnegativeConeT,
        SecondOrderConeT,
        ExponentialConeT,
        PowerConeT,
        GenPowerConeT,
#ifdef FEATURE_SDP
        PSDTriangleConeT,
#endif
    };
    Tag tag;

  protected:
    struct ZeroConeT_Body
    {
        uintptr_t _0;
    };
    struct NonnegativeConeT_Body
    {
        uintptr_t _0;
    };
    struct SecondOrderConeT_Body
    {
        uintptr_t _0;
    };
    struct ExponentialConeT_Body
    {
    };
    struct PowerConeT_Body
    {
        T _0;
    };
    struct GenPowerConeT_Body
    {
        const T* _0;
        uintptr_t _1;
        uintptr_t _2;
    };
#ifdef FEATURE_SDP
    struct PSDTriangleConeT_Body
    {
        uintptr_t _0;
    };
#endif
    // Data
    union
    {
        ZeroConeT_Body zero_cone_t;
        NonnegativeConeT_Body nonnegative_cone_t;
        SecondOrderConeT_Body second_order_cone_t;
        ExponentialConeT_Body exponential_cone_t;
        PowerConeT_Body power_cone_t;
        GenPowerConeT_Body genpower_cone_t;
#ifdef FEATURE_SDP
        PSDTriangleConeT_Body psd_triangle_cone_t;
#endif
    };

  public:
    unsigned int nvars() const
    {
        switch (this->tag)
        {
        case Tag::ZeroConeT:
            return this->zero_cone_t._0;
        case Tag::NonnegativeConeT:
            return this->nonnegative_cone_t._0;
        case Tag::SecondOrderConeT:
            return this->second_order_cone_t._0;
        case Tag::ExponentialConeT:
            return 3;
        case Tag::PowerConeT:
            return 3;
        case Tag::GenPowerConeT:
            return this->genpower_cone_t._1 + this->genpower_cone_t._2;
#ifdef FEATURE_SDP
        case Tag::PSDTriangleConeT: {
            unsigned int k = this->psd_triangle_cone_t._0;
            return (k * (k + 1)) >> 1;
        }
#endif
        default:
            throw std::invalid_argument("Invalid cone type");
        }
    }
};

template<typename T = double>
struct ZeroConeT : public SupportedConeT<T>
{
  public:
    ZeroConeT(uintptr_t dimension)
    {
        this->tag = SupportedConeT<T>::Tag::ZeroConeT;
        this->zero_cone_t = { dimension };
    }

    uintptr_t dimension() const { return this->nonnegative_cone_t._0; }
};

template<typename T = double>
struct NonnegativeConeT : public SupportedConeT<T>
{
  public:
    NonnegativeConeT(uintptr_t dimension)
    {
        this->tag = SupportedConeT<T>::Tag::NonnegativeConeT;
        this->nonnegative_cone_t = { dimension };
    }

    uintptr_t dimension() const { return this->zero_cone_t._0; }
};

template<typename T = double>
struct SecondOrderConeT : public SupportedConeT<T>
{
  public:
    SecondOrderConeT(uintptr_t dimension)
    {
        this->tag = SupportedConeT<T>::Tag::SecondOrderConeT;
        this->second_order_cone_t = { dimension };
    }

    T dimension() const { return this->second_order_cone_t._0; }
};

template<typename T = double>
struct ExponentialConeT : public SupportedConeT<T>
{
  public:
    ExponentialConeT() { this->tag = SupportedConeT<T>::Tag::ExponentialConeT; }
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

    T power() const { return this->power_cone_t._0; }
};

template<typename T = double>
struct GenPowerConeT : public SupportedConeT<T>
{
  public:
    GenPowerConeT(const Eigen::Ref<Eigen::VectorX<T>> &alpha, uintptr_t dim2)
    {
        this->tag = SupportedConeT<T>::Tag::GenPowerConeT;
        const T* alpha_data = alpha.data();
        uintptr_t dim1 = alpha.size();
        this->genpower_cone_t = {alpha_data,dim1,dim2 };
    }
};

#ifdef FEATURE_SDP
template<typename T = double>
struct PSDTriangleConeT : public SupportedConeT<T>
{
  public:
    PSDTriangleConeT(uintptr_t dimension)
    {
        this->tag = SupportedConeT<T>::Tag::PSDTriangleConeT;
        this->psd_triangle_cone_t = { dimension };
    }

    T dimension() const { return this->psd_triangle_cone_t._0; }
};
#endif

} // namespace clarabel