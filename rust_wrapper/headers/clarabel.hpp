#ifndef CLARABEL_H
#define CLARABEL_H

/* Generated with cbindgen:0.24.5 */

/* Warning, this file is autogenerated by cbindgen. Don't modify this manually. */

#include <cstdarg>
#include <cstdint>
#include <cstdlib>
#include <ostream>
#include <new>

namespace clarabel {

using DefaultSolver = void;

template<typename T>
struct CscMatrix {
  /**
   * number of rows
   */
  uintptr_t m;
  /**
   * number of columns
   */
  uintptr_t n;
  /**
   * CSC format column pointer.
   *
   * Ths field should have length `n+1`. The last entry corresponds
   * to the the number of nonzeros and should agree with the lengths
   * of the `rowval` and `nzval` fields.
   */
  const uintptr_t *colptr;
  /**
   * vector of row indices
   */
  const uintptr_t *rowval;
  /**
   * vector of non-zero matrix elements
   */
  const T *nzval;
};

extern "C" {

DefaultSolver *DefaultSolver_new(const CscMatrix<double> *P,
                                 const double *q,
                                 const CscMatrix<double> *A,
                                 const double *b,
                                 uintptr_t _n_cones,
                                 const void *_cones,
                                 const void *_settings);

} // extern "C"

} // namespace clarabel

#endif // CLARABEL_H
