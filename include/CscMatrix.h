#ifndef CSC_MATRIX_H
#define CSC_MATRIX_H

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "clarabel.h"

typedef struct CscMatrix
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
} CscMatrix;

typedef struct CscMatrix_f32
{
    uintptr_t m;
    uintptr_t n;
    const uintptr_t *colptr;
    const uintptr_t *rowval;
    const float *nzval;
} CscMatrix_f32;

/// @brief Create a sparse matrix in Compressed Sparse Column format
/// @param m Number of rows
/// @param n Number of columns
/// @param colptr CSC format column pointer array (always have length n+1)
/// @param rowval Array of row indices (always have length colptr[n])
/// @param nzval Array of nonzero values (always have length colptr[n])
/// @return Pointer to a new CscMatrix_f64 object allocated on the heap
static inline CscMatrix *CscMatrix_new(
    uintptr_t m,
    uintptr_t n,
    const uintptr_t *colptr,
    const uintptr_t *rowval,
    const double *nzval)
{
    CscMatrix *ptr = malloc(sizeof(CscMatrix));

    if (ptr == NULL) // Failed to allocate memory
        return NULL;

    // Initialize CscMatrix_f64
    ptr->m = m;
    ptr->n = n;
    ptr->colptr = colptr;
    ptr->rowval = rowval;
    ptr->nzval = nzval;

    return ptr;
}

static inline CscMatrix_f32 *CscMatrix_f32_new(
    uintptr_t m,
    uintptr_t n,
    const uintptr_t *colptr,
    const uintptr_t *rowval,
    const float *nzval)
{
    CscMatrix_f32 *ptr = malloc(sizeof(CscMatrix_f32));

    if (ptr == NULL) // Failed to allocate memory
        return NULL;

    // Initialize CscMatrix_f32
    ptr->m = m;
    ptr->n = n;
    ptr->colptr = colptr;
    ptr->rowval = rowval;
    ptr->nzval = nzval;

    return ptr;
}

static inline void CscMatrix_free(CscMatrix *ptr)
{
    free(ptr);
}

static inline void CscMatrix_f32_free(CscMatrix_f32 *ptr)
{
    free(ptr);
}

#endif