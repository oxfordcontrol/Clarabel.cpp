#ifndef CLARABEL_H
#define CLARABEL_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "CscMatrix.h"
#include "SolverSettings.h"
#include "SupportedConeT.h"

typedef void DefaultSolver;

DefaultSolver *DefaultSolver_f64_new(const CscMatrix_f64 *P,
                                     const double *q,
                                     const CscMatrix_f64 *A,
                                     const double *b,
                                     uintptr_t n_cones,
                                     const SupportedConeT_f64 *cones,
                                     const DefaultSettings_f64 *settings);

void DefaultSolver_solve(void *solver);

void free_DefaultSolver(DefaultSolver *solver);

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

DefaultSolution_f64 DefaultSolver_f64_solution(DefaultSolver *solver);

#endif /* CLARABEL_H */
