#pragma once

#include "CscMatrix.h"
#include "DefaultSettings.h"
#include "DefaultSolution.h"
#include "SupportedConeT.h"

namespace clarabel
{
    using RustObjectHandle = void *;
    using RustDefaultSolverHandle_f64 = RustObjectHandle;
    using RustDefaultSolverHandle_f32 = RustObjectHandle;

    template<typename T = double>
    class DefaultSolver
    {
        static_assert(std::is_same<T, float>::value || std::is_same<T, double>::value, "T must be float or double");
    private:
        RustObjectHandle handle;

    public:
        DefaultSolver(
            const ClarabelCscMatrix<T> *P,
            const T *q,
            const ClarabelCscMatrix<T> *A,
            const T *b,
            uintptr_t n_cones,
            const ClarabelSupportedConeT<T> *cones,
            const ClarabelDefaultSettings<T> *settings);

        ~DefaultSolver();

        void solve();
        DefaultSolution<T> solution() const;
    };

    extern "C"
    {
        RustDefaultSolverHandle_f64 clarabel_DefaultSolver_f64_new(
            const ClarabelCscMatrix<double> *P,
            const double *q,
            const ClarabelCscMatrix<double> *A,
            const double *b,
            uintptr_t n_cones,
            const ClarabelSupportedConeT<double> *cones,
            const ClarabelDefaultSettings<double> *settings);

        RustDefaultSolverHandle_f32 clarabel_DefaultSolver_f32_new(
            const ClarabelCscMatrix<float> *P,
            const float *q,
            const ClarabelCscMatrix<float> *A,
            const float *b,
            uintptr_t n_cones,
            const ClarabelSupportedConeT<float> *cones,
            const ClarabelDefaultSettings<float> *settings);

        void clarabel_DefaultSolver_f64_solve(RustDefaultSolverHandle_f64 solver);

        void clarabel_DefaultSolver_f32_solve(RustDefaultSolverHandle_f32 solver);

        void clarabel_DefaultSolver_f64_free(RustDefaultSolverHandle_f64 solver);

        void clarabel_DefaultSolver_f32_free(RustDefaultSolverHandle_f32 solver);

        DefaultSolution<double> clarabel_DefaultSolver_f64_solution(RustDefaultSolverHandle_f64 solver);

        DefaultSolution<float> clarabel_DefaultSolver_f32_solution(RustDefaultSolverHandle_f32 solver);
    }

    template<>
    inline DefaultSolver<double>::DefaultSolver(
        const ClarabelCscMatrix<double> *P,
        const double *q,
        const ClarabelCscMatrix<double> *A,
        const double *b,
        uintptr_t n_cones,
        const ClarabelSupportedConeT<double> *cones,
        const ClarabelDefaultSettings<double> *settings)
    {
        handle = clarabel_DefaultSolver_f64_new(P, q, A, b, n_cones, cones, settings);
    }

    template<>
    inline DefaultSolver<float>::DefaultSolver(
        const ClarabelCscMatrix<float> *P,
        const float *q,
        const ClarabelCscMatrix<float> *A,
        const float *b,
        uintptr_t n_cones,
        const ClarabelSupportedConeT<float> *cones,
        const ClarabelDefaultSettings<float> *settings)
    {
        handle = clarabel_DefaultSolver_f32_new(P, q, A, b, n_cones, cones, settings);
    }

    template<>
    inline DefaultSolver<double>::~DefaultSolver()
    {
        clarabel_DefaultSolver_f64_free(handle);
    }

    template<>
    inline DefaultSolver<float>::~DefaultSolver()
    {
        clarabel_DefaultSolver_f32_free(handle);
    }

    template<>
    inline void DefaultSolver<double>::solve()
    {
        clarabel_DefaultSolver_f64_solve(handle);
    }

    template<>
    inline void DefaultSolver<float>::solve()
    {
        clarabel_DefaultSolver_f32_solve(handle);
    }

    template<>
    inline DefaultSolution<double> DefaultSolver<double>::solution() const
    {
        return clarabel_DefaultSolver_f64_solution(handle);
    }

    template<>
    inline DefaultSolution<float> DefaultSolver<float>::solution() const
    {
        return clarabel_DefaultSolver_f32_solution(handle);
    }
}