#ifndef CLARABEL_H
#define CLARABEL_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

#include "CscMatrix.h"

typedef void DefaultSolver;

DefaultSolver *DefaultSolver_new(const CscMatrix_f64 *P,
                                 const double *q,
                                 const CscMatrix_f64 *A,
                                 const double *b,
                                 uintptr_t n_cones,
                                 const void *cones,
                                 const void *settings);

void DefaultSolver_solve(void *solver);

void free_DefaultSolver(DefaultSolver *solver);

#endif /* CLARABEL_H */
