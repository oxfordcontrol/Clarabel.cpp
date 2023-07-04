#ifndef CLARABEL_H
#define CLARABEL_H

#include <cstdarg>
#include <cstdint>
#include <cstdlib>
#include <ostream>
#include <new>

namespace clarabel {

using CscMatrixF64 = void;

extern "C" {

CscMatrixF64 *CscMatrix_new(uintptr_t m,
                            uintptr_t n,
                            const uintptr_t *colptr,
                            const uintptr_t *rowval,
                            const double *nzval);

void CscMatrix_delete(CscMatrixF64 *ptr);

} // extern "C"

} // namespace clarabel

#endif // CLARABEL_H
