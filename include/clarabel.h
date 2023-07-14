#ifndef CLARABEL_H
#define CLARABEL_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "CscMatrix.h"
#include "SolverSettings.h"
#include "SupportedConeT.h"

typedef void DefaultSolver_f64;
typedef void DefaultSolver_f32;

DefaultSolver_f64 *DefaultSolver_f64_new(const CscMatrix_f64 *P,
                                         const double *q,
                                         const CscMatrix_f64 *A,
                                         const double *b,
                                         uintptr_t n_cones,
                                         const SupportedConeT_f64 *cones,
                                         const DefaultSettings_f64 *settings);

DefaultSolver_f32 *DefaultSolver_f32_new(const CscMatrix_f32 *P,
                                         const float *q,
                                         const CscMatrix_f32 *A,
                                         const float *b,
                                         uintptr_t n_cones,
                                         const SupportedConeT_f32 *cones,
                                         const DefaultSettings_f32 *settings);

void DefaultSolver_f64_solve(DefaultSolver_f64 *solver);

void DefaultSolver_f32_solve(DefaultSolver_f32 *solver);

void free_DefaultSolver_f64(DefaultSolver_f64 *solver);

void free_DefaultSolver_f32(DefaultSolver_f32 *solver);

typedef enum SolverStatus
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
} SolverStatus;

typedef struct DefaultSolution_f64
{
    double *x;
    uintptr_t x_length;
    double *z;
    uintptr_t z_length;
    double *s;
    uintptr_t s_length;
    SolverStatus status;
    double obj_val;
    double solve_time;
    uint32_t iterations;
    double r_prim;
    double r_dual;
} DefaultSolution_f64;

typedef struct DefaultSolution_f32
{
    float *x;
    uintptr_t x_length;
    float *z;
    uintptr_t z_length;
    float *s;
    uintptr_t s_length;
    enum SolverStatus status;
    float obj_val;
    double solve_time;
    uint32_t iterations;
    float r_prim;
    float r_dual;
} DefaultSolution_f32;

DefaultSolution_f64 DefaultSolver_f64_solution(DefaultSolver_f64 *solver);

DefaultSolution_f32 DefaultSolver_f32_solution(DefaultSolver_f32 *solver);

#endif /* CLARABEL_H */
