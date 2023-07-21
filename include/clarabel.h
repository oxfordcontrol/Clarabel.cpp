#ifndef CLARABEL_H
#define CLARABEL_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "CscMatrix.h"
#include "SolverSettings.h"
#include "SupportedConeT.h"

typedef void ClarabelDefaultSolver_f64;
typedef void ClarabelDefaultSolver_f32;

ClarabelDefaultSolver_f64 *clarabel_DefaultSolver_f64_new(
    const ClarabelCscMatrix_f64 *P,
    const double *q,
    const ClarabelCscMatrix_f64 *A,
    const double *b,
    uintptr_t n_cones,
    const ClarabelSupportedConeT_f64 *cones,
    const ClarabelDefaultSettings_f64 *settings);

ClarabelDefaultSolver_f32 *clarabel_DefaultSolver_f32_new(
    const ClarabelCscMatrix_f32 *P,
    const float *q,
    const ClarabelCscMatrix_f32 *A,
    const float *b,
    uintptr_t n_cones,
    const ClarabelSupportedConeT_f32 *cones,
    const ClarabelDefaultSettings_f32 *settings);

void clarabel_DefaultSolver_f64_solve(ClarabelDefaultSolver_f64 *solver);

void clarabel_DefaultSolver_f32_solve(ClarabelDefaultSolver_f32 *solver);

void clarabel_DefaultSolver_f64_free(ClarabelDefaultSolver_f64 *solver);

void clarabel_DefaultSolver_f32_free(ClarabelDefaultSolver_f32 *solver);

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

ClarabelDefaultSolution_f64 clarabel_DefaultSolver_f64_solution(ClarabelDefaultSolver_f64 *solver);

ClarabelDefaultSolution_f32 clarabel_DefaultSolver_f32_solution(ClarabelDefaultSolver_f32 *solver);

#endif /* CLARABEL_H */
