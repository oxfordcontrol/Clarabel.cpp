#ifndef CLARABEL_DEFAULT_SOLVER_H
#define CLARABEL_DEFAULT_SOLVER_H

#include "CscMatrix.h"
#include "DefaultInfo.h"
#include "DefaultSettings.h"
#include "DefaultSolution.h"
#include "SupportedConeT.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#ifdef CLARABEL_USE_FLOAT
typedef float ClarabelFloat;
#else
typedef double ClarabelFloat;
#endif

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

ClarabelDefaultSolution_f64 clarabel_DefaultSolver_f64_solution(ClarabelDefaultSolver_f64 *solver);

ClarabelDefaultSolution_f32 clarabel_DefaultSolver_f32_solution(ClarabelDefaultSolver_f32 *solver);

ClarabelDefaultInfo_f64 clarabel_DefaultSolver_f64_info(ClarabelDefaultSolver_f64 *solver);

ClarabelDefaultInfo_f32 clarabel_DefaultSolver_f32_info(ClarabelDefaultSolver_f32 *solver);

#ifdef CLARABEL_USE_FLOAT
typedef ClarabelDefaultSolver_f32 ClarabelDefaultSolver;
#define clarabel_DefaultSolver_new(...) clarabel_DefaultSolver_f32_new(__VA_ARGS__)
#define clarabel_DefaultSolver_solve(...) clarabel_DefaultSolver_f32_solve(__VA_ARGS__)
#define clarabel_DefaultSolver_free(...) clarabel_DefaultSolver_f32_free(__VA_ARGS__)
#define clarabel_DefaultSolver_solution(...) clarabel_DefaultSolver_f32_solution(__VA_ARGS__)
#define clarabel_DefaultSolver_info(...) clarabel_DefaultSolver_f32_info(__VA_ARGS__)
#else
typedef ClarabelDefaultSolver_f64 ClarabelDefaultSolver;
#define clarabel_DefaultSolver_new(...) clarabel_DefaultSolver_f64_new(__VA_ARGS__)
#define clarabel_DefaultSolver_solve(...) clarabel_DefaultSolver_f64_solve(__VA_ARGS__)
#define clarabel_DefaultSolver_free(...) clarabel_DefaultSolver_f64_free(__VA_ARGS__)
#define clarabel_DefaultSolver_solution(...) clarabel_DefaultSolver_f64_solution(__VA_ARGS__)
#define clarabel_DefaultSolver_info(...) clarabel_DefaultSolver_f64_info(__VA_ARGS__)
#endif /* CLARABEL_USE_FLOAT */

#endif /* CLARABEL_H */