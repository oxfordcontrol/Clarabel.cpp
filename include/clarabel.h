#ifndef CLARABEL_H
#define CLARABEL_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef void DefaultSolver;

DefaultSolver *DefaultSolver_new(const struct CscMatrix_f64 *P,
                                 const double *q,
                                 const struct CscMatrix_f64 *A,
                                 const double *b,
                                 uintptr_t n_cones,
                                 const void *cones,
                                 const void *settings);

#endif /* CLARABEL_H */
