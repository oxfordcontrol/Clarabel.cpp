#ifndef CSC_MATRIX_H
#define CSC_MATRIX_H

#include <stdint.h>
#include <stdbool.h>
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

    /**
     * @brief Indicates whether the memory of colptr, rowval and nzval is owned by this struct.
     *
     * Should never be changed by the user.
     */
    bool owns_matrix_data;

} CscMatrix_f64;

typedef struct CscMatrix_f32
{
    uintptr_t m;
    uintptr_t n;
    const uintptr_t *colptr;
    const uintptr_t *rowval;
    const float *nzval;
    bool owns_matrix_data;
} CscMatrix_f32;

/// @brief Create a sparse matrix in Compressed Sparse Column format
/// @param m Number of rows
/// @param n Number of columns
/// @param colptr CSC format column pointer array (always have length n+1)
/// @param rowval Array of row indices (always have length colptr[n])
/// @param nzval Array of nonzero values (always have length colptr[n])
/// @return Pointer to a new CscMatrix_f64 object allocated on the heap
static inline CscMatrix_f64 *CscMatrix_f64_new(
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
    ptr->owns_matrix_data = false;

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
    ptr->owns_matrix_data = false;

    return ptr;
}

/// @brief Create a sparse matrix in Compressed Sparse Column format from a dense matrix
/// @param m Number of rows
/// @param n Number of columns
/// @param matrix Dense matrix in the form of a contiguous array
/// @return A pointer to a new CscMatrix_f64 struct built from the dense matrix
CscMatrix_f64 *CscMatrix_f64_from(uintptr_t m, uintptr_t n, const double matrix[m][n]);

CscMatrix_f32 *CscMatrix_f32_from(uintptr_t m, uintptr_t n, const float matrix[m][n]);

/// @brief Create a sparse matrix in Compressed Sparse Column format of all zeros
/// @param rows Number of rows
/// @param cols Number of columns
/// @return A pointer to a new CscMatrix_f64 struct of all zeros
CscMatrix_f64 *CscMatrix_f64_zeros(uintptr_t rows, uintptr_t cols);

CscMatrix_f32 *CscMatrix_f32_zeros(uintptr_t rows, uintptr_t cols);

/// @brief Create an identity matrix in Compressed Sparse Column format
/// @param n Number of rows and columns of the identity matrix
/// @return A pointer to a new CscMatrix_f64 struct of the identity matrix
CscMatrix_f64 *CscMatrix_f64_identity(uintptr_t n);

CscMatrix_f32 *CscMatrix_f32_identity(uintptr_t n);

/// @brief Delete a CscMatrix_f64 object which has memory owned by Rust
/// @param matrix Pointer to the matrix to delete
void delete_CscMatrix_f64(CscMatrix_f64 *matrix);

void delete_CscMatrix_f32(CscMatrix_f32 *matrix);

/// @brief Free a CscMatrix_f64 object
/// @param matrix Pointer to the matrix to free
static inline void free_CscMatrix_f64(CscMatrix_f64 *matrix)
{
    if (matrix->owns_matrix_data)
        delete_CscMatrix_f64(matrix);
    else
        free(matrix);
}

static inline void free_CscMatrix_f32(CscMatrix_f32 *matrix)
{
    if (matrix->owns_matrix_data)
        delete_CscMatrix_f32(matrix);
    else
        free(matrix);
}

#endif