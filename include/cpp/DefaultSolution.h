#pragma once

#include <cstdint>
#include <type_traits>

namespace clarabel
{
    enum class ClarabelSolverStatus
    {
        ClarabelUnsolved,
        ClarabelSolved,
        ClarabelPrimalInfeasible,
        ClarabelDualInfeasible,
        ClarabelAlmostSolved,
        ClarabelAlmostPrimalInfeasible,
        ClarabelAlmostDualInfeasible,
        ClarabelMaxIterations,
        ClarabelMaxTime,
        ClarabelNumericalError,
        ClarabelInsufficientProgress,
    };

    template<typename T = double>
    struct DefaultSolution
    {
        static_assert(std::is_same<T, float>::value || std::is_same<T, double>::value, "T must be float or double");

        T* x;
        uintptr_t x_length;
        T* z;
        uintptr_t z_length;
        T* s;
        uintptr_t s_length;
        ClarabelSolverStatus status;
        T obj_val;
        double solve_time;
        uint32_t iterations;
        T r_prim;
        T r_dual;
    };
}