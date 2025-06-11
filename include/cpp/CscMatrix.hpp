#pragma once

#include <cstdint>
#include <type_traits>

namespace clarabel
{

template<typename T = double>
struct CscMatrix
{
    static_assert(std::is_same<T, float>::value || std::is_same<T, double>::value, "T must be float or double");

    uintptr_t m;
    uintptr_t n;
    const uintptr_t *colptr;
    const uintptr_t *rowval;
    const T *nzval;

    CscMatrix(uintptr_t _m, uintptr_t _n, const uintptr_t *_colptr, const uintptr_t *_rowval, const T *_nzval)
        : m(_m), n(_n), colptr(_colptr), rowval(_rowval), nzval(_nzval)
    {
    }
};

} // namespace clarabel