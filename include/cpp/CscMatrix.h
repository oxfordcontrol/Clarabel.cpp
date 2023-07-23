#pragma once

#include <cstdint>
#include <type_traits>
#include <iostream>

namespace clarabel
{
    template<typename T>
    struct ClarabelCscMatrix
    {
        static_assert(std::is_same<T, float>::value || std::is_same<T, double>::value, "T must be float or double");

        uintptr_t m;
        uintptr_t n;
        const uintptr_t* colptr;
        const uintptr_t* rowval;
        const T* nzval;

        ClarabelCscMatrix(
            uintptr_t _m,
            uintptr_t _n,
            const uintptr_t *_colptr,
            const uintptr_t *_rowval,
            const T *_nzval)
            : m(_m), n(_n), colptr(_colptr), rowval(_rowval), nzval(_nzval) { }
    };
}