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

#endif /* CLARABEL_H */
