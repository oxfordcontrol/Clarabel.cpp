#pragma once

#include <Eigen/Eigen>
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
class DefaultSolution
{
    static_assert(std::is_same<T, float>::value || std::is_same<T, double>::value, "T must be float or double");

  public:
    struct ClarabelDefaultSolution
    {
        T *x;
        uintptr_t x_length;
        T *z;
        uintptr_t z_length;
        T *s;
        uintptr_t s_length;
        SolverStatus status;
        T obj_val;
        T obj_val_dual;
        double solve_time;
        uint32_t iterations;
        T r_prim;
        T r_dual;
    };

    Eigen::Map<Eigen::VectorX<T>> x, z, s;
    SolverStatus status;
    T obj_val;
    T obj_val_dual;
    double solve_time;
    uint32_t iterations;
    T r_prim;
    T r_dual;

    DefaultSolution(ClarabelDefaultSolution &solution)
        : 
        x(solution.x, solution.x_length), 
        z(solution.z, solution.z_length), 
        s(solution.s, solution.s_length),
        status(solution.status), 
        obj_val(solution.obj_val),
        obj_val_dual(solution.obj_val_dual),
        solve_time(solution.solve_time), 
        iterations(solution.iterations),
        r_prim(solution.r_prim), 
        r_dual(solution.r_dual)
    {
    }
};

// Instantiate the templates
template struct DefaultSolution<double>;
template struct DefaultSolution<float>;

} // namespace clarabel