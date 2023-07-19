#ifndef CLARABEL_H
#define CLARABEL_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "CscMatrix.h"
#include "SolverSettings.h"
#include "SupportedConeT.h"

typedef void ClarabelDefaultSolver;
typedef void ClarabelDefaultSolver_f32;

ClarabelDefaultSolver *DefaultSolver_new(const ClarabelCscMatrix *P,
                                 const double *q,
                                 const ClarabelCscMatrix *A,
                                 const double *b,
                                 uintptr_t n_cones,
                                 const ClarabelSupportedConeT *cones,
                                 const ClarabelDefaultSettings *settings);

ClarabelDefaultSolver_f32 *DefaultSolver_f32_new(const ClarabelCscMatrix_f32 *P,
                                         const float *q,
                                         const ClarabelCscMatrix_f32 *A,
                                         const float *b,
                                         uintptr_t n_cones,
                                         const ClarabelSupportedConeT_f32 *cones,
                                         const ClarabelDefaultSettings_f32 *settings);

void DefaultSolver_solve(ClarabelDefaultSolver *solver);

void DefaultSolver_f32_solve(ClarabelDefaultSolver_f32 *solver);

void DefaultSolver_free(ClarabelDefaultSolver *solver);

void DefaultSolver_f32_free(ClarabelDefaultSolver_f32 *solver);

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

typedef struct ClarabelDefaultSolution
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
} ClarabelDefaultSolution;

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

ClarabelDefaultSolution DefaultSolver_solution(ClarabelDefaultSolver *solver);

ClarabelDefaultSolution_f32 DefaultSolver_f32_solution(ClarabelDefaultSolver_f32 *solver);

#endif /* CLARABEL_H */
