#pragma once

#include "CscMatrix.h"
#include "DefaultSettings.h"
#include "DefaultSolution.h"
#include "SupportedConeT.h"
#include <vector>
#include <memory>

namespace clarabel
{
    using RustObjectHandle = void *;
    using RustDefaultSolverHandle_f64 = RustObjectHandle;
    using RustDefaultSolverHandle_f32 = RustObjectHandle;

    template<typename T = double>
    class DefaultSolver
    {
        static_assert(std::is_same<T, float>::value || std::is_same<T, double>::value, "T must be float or double");
    protected:
        RustObjectHandle handle;
        std::vector<SupportedConeT<T>> cones; // Copy the cones or initialize with the pointer?

        DefaultSolver() = default;

    public:
        DefaultSolver(
            const CscMatrix<T> *P,
            const T *q,
            const CscMatrix<T> *A,
            const T *b,
            const std::vector<SupportedConeT<T>> &cones,
            const DefaultSettings<T> *settings);

        ~DefaultSolver();

        void solve();
        DefaultSolution<T> solution() const;
    };

    extern "C"
    {
        RustDefaultSolverHandle_f64 clarabel_DefaultSolver_f64_new(
            const CscMatrix<double> *P,
            const double *q,
            const CscMatrix<double> *A,
            const double *b,
            uintptr_t n_cones,
            const SupportedConeT<double> *cones,
            const DefaultSettings<double> *settings);

        RustDefaultSolverHandle_f32 clarabel_DefaultSolver_f32_new(
            const CscMatrix<float> *P,
            const float *q,
            const CscMatrix<float> *A,
            const float *b,
            uintptr_t n_cones,
            const SupportedConeT<float> *cones,
            const DefaultSettings<float> *settings);

        void clarabel_DefaultSolver_f64_solve(RustDefaultSolverHandle_f64 solver);

        void clarabel_DefaultSolver_f32_solve(RustDefaultSolverHandle_f32 solver);

        void clarabel_DefaultSolver_f64_free(RustDefaultSolverHandle_f64 solver);

        void clarabel_DefaultSolver_f32_free(RustDefaultSolverHandle_f32 solver);

        template struct DefaultSolution<double>; // Instantiate the template
        DefaultSolution<double> clarabel_DefaultSolver_f64_solution(RustDefaultSolverHandle_f64 solver);

        template struct DefaultSolution<float>; // Instantiate the template
        DefaultSolution<float> clarabel_DefaultSolver_f32_solution(RustDefaultSolverHandle_f32 solver);
    }


    template<>
    inline DefaultSolver<double>::DefaultSolver(
        const CscMatrix<double> *P,
        const double *q,
        const CscMatrix<double> *A,
        const double *b,
        const std::vector<SupportedConeT<double>> &_cones,
        const DefaultSettings<double> *settings) : cones(_cones)
    {
        handle = clarabel_DefaultSolver_f64_new(P, q, A, b, cones.size(), cones.data(), settings);
    }

    template<>
    inline DefaultSolver<float>::DefaultSolver(
        const CscMatrix<float> *P,
        const float *q,
        const CscMatrix<float> *A,
        const float *b,
        const std::vector<SupportedConeT<float>> &_cones,
        const DefaultSettings<float> *settings) : cones(_cones)
    {
        handle = clarabel_DefaultSolver_f32_new(P, q, A, b, cones.size(), cones.data(), settings);
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

} // namespace clarabel