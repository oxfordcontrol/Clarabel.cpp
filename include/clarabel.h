#ifndef CLARABEL_H
#define CLARABEL_H

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

typedef void CscMatrixF64;

CscMatrixF64 *CscMatrix_new(uintptr_t m,
                            uintptr_t n,
                            const uintptr_t *colptr,
                            const uintptr_t *rowval,
                            const double *nzval);

void CscMatrix_delete(CscMatrixF64 *ptr);

#endif /* CLARABEL_H */
