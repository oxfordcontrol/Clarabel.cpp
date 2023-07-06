#ifndef CSC_MATRIX_H
#define CSC_MATRIX_H

#include <stdint.h>
#include <stdlib.h>
#include "clarabel.h"

typedef struct CscMatrix_f64
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

    /// @brief Vector of row indices
    const uintptr_t *rowval;

    /// @brief Vector of non-zero matrix elements
    const double *nzval;

} CscMatrix_f64;

/// @brief Create a sparse matrix in Compressed Sparse Column format
/// @param m Number of rows
/// @param n Number of columns
/// @param colptr CSC format column pointer array (always have length n+1)
/// @param rowval Array of row indices (always have length colptr[n])
/// @param nzval Array of nonzero values (always have length colptr[n])
/// @return Pointer to a new CscMatrix_f64 object allocated on the heap
CscMatrix_f64 *CscMatrix_f64_new(
    uintptr_t m, 
    uintptr_t n, 
    const uintptr_t *colptr, 
    const uintptr_t *rowval,
    const double *nzval)
{
    CscMatrix_f64 *ptr = malloc(sizeof(CscMatrix_f64));
    
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

/// @brief Create a sparse matrix in Compressed Sparse Column format from a dense matrix
/// @param m Number of rows
/// @param n Number of columns
/// @param matrix Dense matrix in the form of a contiguous array
/// @return A new CscMatrix_f64 struct built from the dense matrix
struct CscMatrix_f64 CscMatrix_f64_from(uintptr_t m, uintptr_t n, const double *matrix);

/// @brief Create a sparse matrix in Compressed Sparse Column format of all zeros
/// @param rows Number of rows
/// @param cols Number of columns
/// @return A new CscMatrix_f64 struct of all zeros
struct CscMatrix_f64 CscMatrix_f64_zeros(uintptr_t rows, uintptr_t cols);

/// @brief Free a CscMatrix_f64 object
/// @param matrix Pointer to the matrix to free
void free_CscMatrix_f64(CscMatrix_f64 *matrix)
{
    free(matrix);
}

#endif