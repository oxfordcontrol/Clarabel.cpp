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


////// P data updating 

// DefaultSolver::update_P (full rewrite of sparse nonzeros)
ClarabelDefaultInfo_f64 clarabel_DefaultSolver_f64_update_P(ClarabelDefaultSolver_f64 *solver, const double *Pnzval, uintptr_t nnzP);
ClarabelDefaultInfo_f32 clarabel_DefaultSolver_f32_update_P(ClarabelDefaultSolver_f32 *solver, const float  *Pnzval, uintptr_t nnzP);

static inline ClarabelDefaultInfo clarabel_DefaultSolver_update_P(ClarabelDefaultSolver *solver, const ClarabelFloat *Pnzval, uintptr_t nnzP)
{
#ifdef CLARABEL_USE_FLOAT
    return clarabel_DefaultSolver_f32_update_P(solver,Pnzval, nnzP);
#else
    return clarabel_DefaultSolver_f64_update_P(solver,Pnzval, nnzP);
#endif
}

// DefaultSolver::update_P_partial (partial rewrite of sparse nonzeros)
ClarabelDefaultInfo_f64 clarabel_DefaultSolver_f64_update_P_partial(ClarabelDefaultSolver_f64 *solver, const uintptr_t* index, const double *values, uintptr_t nvals);
ClarabelDefaultInfo_f32 clarabel_DefaultSolver_f32_update_P_partial(ClarabelDefaultSolver_f32 *solver, const uintptr_t* index, const float  *values, uintptr_t nvals);

static inline ClarabelDefaultInfo clarabel_DefaultSolver_update_P_partial(ClarabelDefaultSolver *solver, const uintptr_t* index, const ClarabelFloat *values, uintptr_t nvals)
{
#ifdef CLARABEL_USE_FLOAT
    return clarabel_DefaultSolver_f32_update_P_partial(solver,index, values, nvals);
#else
    return clarabel_DefaultSolver_f64_update_P_partial(solver,index, values, nvals);
#endif
}

// DefaultSolver::update_P (full rewrite of sparse matrix data using CSC formatted source)
ClarabelDefaultInfo_f64 clarabel_DefaultSolver_f64_update_P_csc(ClarabelDefaultSolver_f64 *solver, const ClarabelCscMatrix_f64 *P);
ClarabelDefaultInfo_f32 clarabel_DefaultSolver_f32_update_P_csc(ClarabelDefaultSolver_f32 *solver, const ClarabelCscMatrix_f32 *P);

static inline ClarabelDefaultInfo clarabel_DefaultSolver_update_P_csc(ClarabelDefaultSolver *solver, const ClarabelCscMatrix *P)
{
#ifdef CLARABEL_USE_FLOAT
    return clarabel_DefaultSolver_f32_update_P_csc(solver,P);
#else
    return clarabel_DefaultSolver_f64_update_P_csc(solver,P);
#endif
}


////// A data updating 

// DefaultSolver::update_A (full rewrite of sparse nonzeros)
ClarabelDefaultInfo_f64 clarabel_DefaultSolver_f64_update_A(ClarabelDefaultSolver_f64 *solver, const double *Anzval, uintptr_t nnzA);
ClarabelDefaultInfo_f32 clarabel_DefaultSolver_f32_update_A(ClarabelDefaultSolver_f32 *solver, const float  *Anzval, uintptr_t nnzA);

static inline ClarabelDefaultInfo clarabel_DefaultSolver_update_A(ClarabelDefaultSolver *solver, const ClarabelFloat *Anzval, uintptr_t nnzA)
{
#ifdef CLARABEL_USE_FLOAT
    return clarabel_DefaultSolver_f32_update_A(solver,Anzval, nnzA);
#else
    return clarabel_DefaultSolver_f64_update_A(solver,Anzval, nnzA);
#endif
}

// DefaultSolver::update_P_partial (partial rewrite of sparse nonzeros)
ClarabelDefaultInfo_f64 clarabel_DefaultSolver_f64_update_A_partial(ClarabelDefaultSolver_f64 *solver, const uintptr_t* index, const double *values, uintptr_t nvals);
ClarabelDefaultInfo_f32 clarabel_DefaultSolver_f32_update_A_partial(ClarabelDefaultSolver_f32 *solver, const uintptr_t* index, const float  *values, uintptr_t nvals);

static inline ClarabelDefaultInfo clarabel_DefaultSolver_update_A_partial(ClarabelDefaultSolver *solver, const uintptr_t* index, const ClarabelFloat *values, uintptr_t nvals)
{
#ifdef CLARABEL_USE_FLOAT
    return clarabel_DefaultSolver_f32_update_A_partial(solver,index, values, nvals);
#else
    return clarabel_DefaultSolver_f64_update_A_partial(solver,index, values, nvals);
#endif
}

// DefaultSolver::update_P (full rewrite of sparse matrix data using CSC formatted source)
ClarabelDefaultInfo_f64 clarabel_DefaultSolver_f64_update_A_csc(ClarabelDefaultSolver_f64 *solver, const ClarabelCscMatrix_f64 *A);
ClarabelDefaultInfo_f32 clarabel_DefaultSolver_f32_update_A_csc(ClarabelDefaultSolver_f32 *solver, const ClarabelCscMatrix_f32 *A);

static inline ClarabelDefaultInfo clarabel_DefaultSolver_update_A_csc(ClarabelDefaultSolver *solver, const ClarabelCscMatrix *A)
{
#ifdef CLARABEL_USE_FLOAT
    return clarabel_DefaultSolver_f32_update_A_csc(solver,A);
#else
    return clarabel_DefaultSolver_f64_update_A_csc(solver,A);
#endif
}

////// q data updating 

// DefaultSolver::update_A (full rewrite of sparse nonzeros)
ClarabelDefaultInfo_f64 clarabel_DefaultSolver_f64_update_q(ClarabelDefaultSolver_f64 *solver, const double *values, uintptr_t n);
ClarabelDefaultInfo_f32 clarabel_DefaultSolver_f32_update_q(ClarabelDefaultSolver_f32 *solver, const float  *values, uintptr_t n);

static inline ClarabelDefaultInfo clarabel_DefaultSolver_update_q(ClarabelDefaultSolver *solver, const ClarabelFloat *values, uintptr_t n)
{
#ifdef CLARABEL_USE_FLOAT
    return clarabel_DefaultSolver_f32_update_q(solver, values, n);
#else
    return clarabel_DefaultSolver_f64_update_q(solver, values, n);
#endif
}

// DefaultSolver::update_P_partial (partial rewrite of sparse nonzeros)
ClarabelDefaultInfo_f64 clarabel_DefaultSolver_f64_update_q_partial(ClarabelDefaultSolver_f64 *solver, const uintptr_t* index, const double *values, uintptr_t nvals);
ClarabelDefaultInfo_f32 clarabel_DefaultSolver_f32_update_q_partial(ClarabelDefaultSolver_f32 *solver, const uintptr_t* index, const float  *values, uintptr_t nvals);

static inline ClarabelDefaultInfo clarabel_DefaultSolver_update_q_partial(ClarabelDefaultSolver *solver, const uintptr_t* index, const ClarabelFloat *values, uintptr_t nvals)
{
#ifdef CLARABEL_USE_FLOAT
    return clarabel_DefaultSolver_f32_update_q_partial(solver,index, values, nvals);
#else
    return clarabel_DefaultSolver_f64_update_q_partial(solver,index, values, nvals);
#endif
}

////// b data updating 

// DefaultSolver::update_A (full rewrite of sparse nonzeros)
ClarabelDefaultInfo_f64 clarabel_DefaultSolver_f64_update_b(ClarabelDefaultSolver_f64 *solver, const double *values, uintptr_t n);
ClarabelDefaultInfo_f32 clarabel_DefaultSolver_f32_update_b(ClarabelDefaultSolver_f32 *solver, const float  *values, uintptr_t n);

static inline ClarabelDefaultInfo clarabel_DefaultSolver_update_b(ClarabelDefaultSolver *solver, const ClarabelFloat *values, uintptr_t n)
{
#ifdef CLARABEL_USE_FLOAT
    return clarabel_DefaultSolver_f32_update_b(solver, values, n);
#else
    return clarabel_DefaultSolver_f64_update_b(solver, values, n);
#endif
}

// DefaultSolver::update_P_partial (partial rewrite of sparse nonzeros)
ClarabelDefaultInfo_f64 clarabel_DefaultSolver_f64_update_b_partial(ClarabelDefaultSolver_f64 *solver, const uintptr_t* index, const double *values, uintptr_t nvals);
ClarabelDefaultInfo_f32 clarabel_DefaultSolver_f32_update_b_partial(ClarabelDefaultSolver_f32 *solver, const uintptr_t* index, const float  *values, uintptr_t nvals);

static inline ClarabelDefaultInfo clarabel_DefaultSolver_update_b_partial(ClarabelDefaultSolver *solver, const uintptr_t* index, const ClarabelFloat *values, uintptr_t nvals)
{
#ifdef CLARABEL_USE_FLOAT
    return clarabel_DefaultSolver_f32_update_b_partial(solver,index, values, nvals);
#else
    return clarabel_DefaultSolver_f64_update_b_partial(solver,index, values, nvals);
#endif
}



#endif /* CLARABEL_H */