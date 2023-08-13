#ifndef CLARABEL_DEFAULT_SOLUTION_H
#define CLARABEL_DEFAULT_SOLUTION_H

#include "ClarabelTypes.h"

#include <stdint.h>

// ClarabelDefaultSolution types

typedef enum ClarabelSolverStatus
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
} ClarabelSolverStatus;

typedef struct ClarabelDefaultSolution_f64
{
    double *x;
    uintptr_t x_length;
    double *z;
    uintptr_t z_length;
    double *s;
    uintptr_t s_length;
    ClarabelSolverStatus status;
    double obj_val;
    double solve_time;
    uint32_t iterations;
    double r_prim;
    double r_dual;
} ClarabelDefaultSolution_f64;

typedef struct ClarabelDefaultSolution_f32
{
    float *x;
    uintptr_t x_length;
    float *z;
    uintptr_t z_length;
    float *s;
    uintptr_t s_length;
    ClarabelSolverStatus status;
    float obj_val;
    double solve_time;
    uint32_t iterations;
    float r_prim;
    float r_dual;
} ClarabelDefaultSolution_f32;

#ifdef CLARABEL_USE_FLOAT
typedef ClarabelDefaultSolution_f32 ClarabelDefaultSolution;
#else
typedef ClarabelDefaultSolution_f64 ClarabelDefaultSolution;
#endif /* CLARABEL_USE_FLOAT */

#endif /* CLARABEL_DEFAULT_SOLUTION_H */