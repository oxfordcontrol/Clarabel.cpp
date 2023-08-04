#pragma once

#include "CscMatrix.h"
#include "DefaultSettings.h"
#include "DefaultSolution.h"
#include "DefaultSolver.h"
#include "SupportedConeT.h"

#include <Eigen/Sparse>
#include <memory>
#include <vector>

namespace clarabel
{
    namespace eigen
    {
        template<typename T = double>
        struct ConvertedCscMatrix
        {
            static_assert(std::is_same<T, float>::value || std::is_same<T, double>::value, "T must be float or double");

            uintptr_t m;
            uintptr_t n;
            const std::vector<uintptr_t> colptr;
            const std::vector<uintptr_t> rowval;
            const T *nzval;

            // Use move semantics to avoid copying the data
            ConvertedCscMatrix(
                uintptr_t m,
                uintptr_t n,
                std::vector<uintptr_t> &colptr,
                std::vector<uintptr_t> &rowval,
                const T *nzval)
                :
                m(m), n(n),
                colptr(std::move(colptr)),
                rowval(std::move(rowval)),
                nzval(nzval)
            {
            }
        };

        template<typename T = double>
        class DefaultSolver : public clarabel::DefaultSolver<T>
        {
        private:
            // Holds the rowptr and colptr values converted from Eigen sparse matrix.
            // These intermediate values should be disposed when this DefaultSolver object is destroyed.
            std::unique_ptr<ConvertedCscMatrix<T>> matrix_P = nullptr, matrix_A = nullptr;

            // Helper function for converting a Eigen sparse matrix into a temporary object of type ConvertedCscMatrix<T>
            // The temporary object is used to keep the matrix data used by the solver alive, and will be used to init a clarabel::CscMatrix<T> object, which is then passed to the solver.
            static std::unique_ptr<ConvertedCscMatrix<T>> 
                eigen_sparse_to_clarabel(const Eigen::SparseMatrix<T, Eigen::ColMajor> &matrix);

        public:
            DefaultSolver(
                const Eigen::SparseMatrix<T, Eigen::ColMajor> &P,
                const T *q,
                const Eigen::SparseMatrix<T, Eigen::ColMajor> &A,
                const T *b,
                const std::vector<SupportedConeT<T>> &cones,
                const DefaultSettings<T> *settings);
        };
 
        template<typename T>
        std::unique_ptr<ConvertedCscMatrix<T>>
            DefaultSolver<T>::eigen_sparse_to_clarabel(const Eigen::SparseMatrix<T, Eigen::ColMajor> &matrix)
        {            
            // Make a copy of data in SparseMatrix to convert StorageIndex to uintptr_t
            std::vector<uintptr_t> row_indices(matrix.nonZeros());
            std::vector<uintptr_t> col_pointers(matrix.outerSize() + 1);

            // Convert to uintptr_t
            for (int k = 0; k < matrix.nonZeros(); ++k)
            {
                row_indices[k] = matrix.innerIndexPtr()[k];
            }
            for (int k = 0; k < matrix.outerSize(); ++k)
            {
                col_pointers[k] = matrix.outerIndexPtr()[k];
            }
            col_pointers[matrix.outerSize()] = matrix.nonZeros();

            // No conversion needed for nz values
            const T *nzval_ptr = matrix.nonZeros() == 0 ? nullptr : matrix.valuePtr();

            ConvertedCscMatrix<T> *csc_matrix = new ConvertedCscMatrix<T>(
                static_cast<uintptr_t>(matrix.rows()),
                static_cast<uintptr_t>(matrix.cols()),
                col_pointers,
                row_indices,
                nzval_ptr
            );

            return std::unique_ptr<ConvertedCscMatrix<T>>(csc_matrix);
        }

        
        // Convert unique_ptr P, A to CscMatrix objects, then init the solver
        // The CscMatrix objects are only used to pass the information needed to Rust.
        // The colptr and rowptr are stored in the ConvertedCscMatrix objects, which are kept alive by the unique_ptr.
        // No conversion is needed for nzval, so the Eigen sparse matrices must be kept alive until the solver is destroyed.
        template<>
        inline DefaultSolver<double>::DefaultSolver(
            const Eigen::SparseMatrix<double, Eigen::ColMajor> &P,
            const double *q,
            const Eigen::SparseMatrix<double, Eigen::ColMajor> &A,
            const double *b,
            const std::vector<SupportedConeT<double>> &cones,
            const DefaultSettings<double> *settings)
            : 
            matrix_P(DefaultSolver<double>::eigen_sparse_to_clarabel(P)),
            matrix_A(DefaultSolver<double>::eigen_sparse_to_clarabel(A))
        {
            CscMatrix<double> p(matrix_P->m, matrix_P->n, matrix_P->colptr.data(), matrix_P->rowval.data(), matrix_P->nzval);
            CscMatrix<double> a(matrix_A->m, matrix_A->n, matrix_A->colptr.data(), matrix_A->rowval.data(), matrix_A->nzval);

            // Cones are copied on the Rust side, so we don't need to worry about the lifetime of the cones.
            // The settings struct is also copied on the Rust side.
            this->handle = clarabel_DefaultSolver_f64_new(&p, q, &a, b, cones.size(), cones.data(), settings);
        }

        template<>
        inline DefaultSolver<float>::DefaultSolver(
            const Eigen::SparseMatrix<float, Eigen::ColMajor> &P,
            const float *q,
            const Eigen::SparseMatrix<float, Eigen::ColMajor> &A,
            const float *b,
            const std::vector<SupportedConeT<float>> &cones,
            const DefaultSettings<float> *settings)
            : 
            matrix_P(DefaultSolver<float>::eigen_sparse_to_clarabel(P)),
            matrix_A(DefaultSolver<float>::eigen_sparse_to_clarabel(A))
        {
            CscMatrix<float> p(matrix_P->m, matrix_P->n, matrix_P->colptr.data(), matrix_P->rowval.data(), matrix_P->nzval);
            CscMatrix<float> a(matrix_A->m, matrix_A->n, matrix_A->colptr.data(), matrix_A->rowval.data(), matrix_A->nzval);
            
            // Cones are copied on the Rust side, so we don't need to worry about the lifetime of the cones.
            // The settings struct is also copied on the Rust side.
            this->handle = clarabel_DefaultSolver_f32_new(&p, q, &a, b, cones.size(), cones.data(), settings);
        }

    } // namespace eigen
} // namespace clarabel
