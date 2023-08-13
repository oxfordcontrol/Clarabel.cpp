#ifndef CLARABEL_DEFAULT_SOLVER_H
#define CLARABEL_DEFAULT_SOLVER_H

#include "ClarabelTypes.h"
#include "CscMatrix.h"
#include "DefaultInfo.h"
#include "DefaultSettings.h"
#include "DefaultSolution.h"
#include "SupportedConeT.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

// ClarabelDefaultSolver types
typedef void ClarabelDefaultSolver_f64;
typedef void ClarabelDefaultSolver_f32;

#ifdef CLARABEL_USE_FLOAT
typedef ClarabelDefaultSolver_f32 ClarabelDefaultSolver;
#else
typedef ClarabelDefaultSolver_f64 ClarabelDefaultSolver;
#endif

// Default solver APIs

// DefaultSolver::new
ClarabelDefaultSolver_f64 *clarabel_DefaultSolver_f64_new(const ClarabelCscMatrix_f64 *P,
                                                          const double *q,
                                                          const ClarabelCscMatrix_f64 *A,
                                                          const double *b,
                                                          uintptr_t n_cones,
                                                          const ClarabelSupportedConeT_f64 *cones,
                                                          const ClarabelDefaultSettings_f64 *settings);

ClarabelDefaultSolver_f32 *clarabel_DefaultSolver_f32_new(const ClarabelCscMatrix_f32 *P,
                                                          const float *q,
                                                          const ClarabelCscMatrix_f32 *A,
                                                          const float *b,
                                                          uintptr_t n_cones,
                                                          const ClarabelSupportedConeT_f32 *cones,
                                                          const ClarabelDefaultSettings_f32 *settings);

static inline ClarabelDefaultSolver *clarabel_DefaultSolver_new(const ClarabelCscMatrix *P,
                                                                const ClarabelFloat *q,
                                                                const ClarabelCscMatrix *A,
                                                                const ClarabelFloat *b,
                                                                uintptr_t n_cones,
                                                                const ClarabelSupportedConeT *cones,
                                                                const ClarabelDefaultSettings *settings)
{
#ifdef CLARABEL_USE_FLOAT
    return clarabel_DefaultSolver_f32_new(P, q, A, b, n_cones, cones, settings);
#else
    return clarabel_DefaultSolver_f64_new(P, q, A, b, n_cones, cones, settings);
#endif
}

// DefaultSolver::solve
void clarabel_DefaultSolver_f64_solve(ClarabelDefaultSolver_f64 *solver);

void clarabel_DefaultSolver_f32_solve(ClarabelDefaultSolver_f32 *solver);

static inline void clarabel_DefaultSolver_solve(ClarabelDefaultSolver *solver)
{
#ifdef CLARABEL_USE_FLOAT
    clarabel_DefaultSolver_f32_solve(solver);
#else
    clarabel_DefaultSolver_f64_solve(solver);
#endif
}

// DefaultSolver::free
void clarabel_DefaultSolver_f64_free(ClarabelDefaultSolver_f64 *solver);

void clarabel_DefaultSolver_f32_free(ClarabelDefaultSolver_f32 *solver);

static inline void clarabel_DefaultSolver_free(ClarabelDefaultSolver *solver)
{
#ifdef CLARABEL_USE_FLOAT
    clarabel_DefaultSolver_f32_free(solver);
#else
    clarabel_DefaultSolver_f64_free(solver);
#endif
}

// DefaultSolver::solution
ClarabelDefaultSolution_f64 clarabel_DefaultSolver_f64_solution(ClarabelDefaultSolver_f64 *solver);

ClarabelDefaultSolution_f32 clarabel_DefaultSolver_f32_solution(ClarabelDefaultSolver_f32 *solver);

static inline ClarabelDefaultSolution clarabel_DefaultSolver_solution(ClarabelDefaultSolver *solver)
{
#ifdef CLARABEL_USE_FLOAT
    return clarabel_DefaultSolver_f32_solution(solver);
#else
    return clarabel_DefaultSolver_f64_solution(solver);
#endif
}

// DefaultSolver::info
ClarabelDefaultInfo_f64 clarabel_DefaultSolver_f64_info(ClarabelDefaultSolver_f64 *solver);

ClarabelDefaultInfo_f32 clarabel_DefaultSolver_f32_info(ClarabelDefaultSolver_f32 *solver);

static inline ClarabelDefaultInfo clarabel_DefaultSolver_info(ClarabelDefaultSolver *solver)
{
#ifdef CLARABEL_USE_FLOAT
    return clarabel_DefaultSolver_f32_info(solver);
#else
    return clarabel_DefaultSolver_f64_info(solver);
#endif
}

#endif /* CLARABEL_H */