#pragma once

#include <cstdint>
#include <type_traits>

namespace clarabel
{
    enum class SolverStatus
    {
        Unsolved,
        Solved,
        PrimalInfeasible,
        DualInfeasible,
        AlmostSolved,
        AlmostPrimalInfeasible,
        AlmostDualInfeasible,
        MaxIterations,
        MaxTime,
        NumericalError,
        InsufficientProgress,
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
        SolverStatus status;
        T obj_val;
        double solve_time;
        uint32_t iterations;
        T r_prim;
        T r_dual;
    };

    // Instantiate the templates
    template struct DefaultSolution<double>;
    template struct DefaultSolution<float>;
}