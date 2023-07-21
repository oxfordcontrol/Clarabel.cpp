#ifndef CLARABEL_H
#define CLARABEL_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "CscMatrix.h"
#include "DefaultSettings.h"
#include "DefaultSolution.h"
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

ClarabelDefaultSolution_f64 clarabel_DefaultSolver_f64_solution(ClarabelDefaultSolver_f64 *solver);

ClarabelDefaultSolution_f32 clarabel_DefaultSolver_f32_solution(ClarabelDefaultSolver_f32 *solver);

#endif /* CLARABEL_H */
