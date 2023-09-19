#ifndef CLARABEL_CSC_MATRIX_H
#define CLARABEL_CSC_MATRIX_H

#include "ClarabelTypes.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

// ClarabelCscMatrix types
typedef struct ClarabelCscMatrix_f64
{
    /// @brief Number of rows
    uintptr_t m;

    /// @brief number of columns
    uintptr_t n;

    /**
     * @brief CSC format column pointer.
     *
     * This field should have length `n+1`. The last entry corresponds
     * to the the number of nonzeros and should agree with the lengths
     * of the `rowval` and `nzval` fields.
     */
    const uintptr_t *colptr;

    /**
     * @brief Vector of row indices
     *
     * If this is a zero matrix, use `NULL` for this field.
     */
    const uintptr_t *rowval;

    /**
     * @brief Vector of non-zero matrix elements
     *
     * If this is a zero matrix, use `NULL` for this field.
     */
    const double *nzval;
} ClarabelCscMatrix_f64;

typedef struct ClarabelCscMatrix_f32
{
    uintptr_t m;
    uintptr_t n;
    const uintptr_t *colptr;
    const uintptr_t *rowval;
    const float *nzval;
} ClarabelCscMatrix_f32;

#ifdef CLARABEL_USE_FLOAT
typedef ClarabelCscMatrix_f32 ClarabelCscMatrix;
#else
typedef ClarabelCscMatrix_f64 ClarabelCscMatrix;
#endif

// ClarabelCscMatrix APIs

// ClarabelCscMatrix::init
void clarabel_CscMatrix_f32_init(ClarabelCscMatrix_f32 *ptr,
                                 uintptr_t m,
                                 uintptr_t n,
                                 const uintptr_t *colptr,
                                 const uintptr_t *rowval,
                                 const float *nzval);

void clarabel_CscMatrix_f64_init(ClarabelCscMatrix_f64 *ptr,
                                 uintptr_t m,
                                 uintptr_t n,
                                 const uintptr_t *colptr,
                                 const uintptr_t *rowval,
                                 const double *nzval);

/// @brief Initialize a sparse matrix in Compressed Sparse Column format
/// @param m Number of rows
/// @param n Number of columns
/// @param colptr CSC format column pointer array (always has length n+1)
/// @param rowval Array of row indices (always has length colptr[n])
/// @param nzval Array of nonzero values (always has length colptr[n])
static inline void clarabel_CscMatrix_init(ClarabelCscMatrix_f64 *ptr,
                                           uintptr_t m,
                                           uintptr_t n,
                                           const uintptr_t *colptr,
                                           const uintptr_t *rowval,
                                           const double *nzval)
{
#ifdef CLARABEL_USE_FLOAT
    clarabel_CscMatrix_f32_init(ptr, m, n, colptr, rowval, nzval);
#else
    clarabel_CscMatrix_f64_init(ptr, m, n, colptr, rowval, nzval);
#endif
}

#endif /* CLARABEL_CSC_MATRIX_H */