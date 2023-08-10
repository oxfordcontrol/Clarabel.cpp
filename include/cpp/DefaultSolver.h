#pragma once

#include "CscMatrix.h"
#include "DefaultInfo.h"
#include "DefaultSettings.h"
#include "DefaultSolution.h"
#include "SupportedConeT.h"

#include <Eigen/Eigen>
#include <memory>
#include <vector>

namespace clarabel
{
    using RustObjectHandle = void *;
    using RustDefaultSolverHandle_f64 = RustObjectHandle;
    using RustDefaultSolverHandle_f32 = RustObjectHandle;

    template<typename T = double>
    class DefaultSolver
    {
        static_assert(std::is_same<T, float>::value || std::is_same<T, double>::value, "T must be float or double");

    private:
        struct ConvertedCscMatrix;
        
        RustObjectHandle handle = nullptr;

        // Holds the rowptr and colptr values converted from Eigen sparse matrix.
        // These intermediate values should be disposed when this DefaultSolver object is destroyed.
        std::unique_ptr<ConvertedCscMatrix> matrix_P = nullptr, matrix_A = nullptr;

        // Helper function for converting a Eigen sparse matrix into a temporary object of type ConvertedCscMatrix<T>
        // The temporary object is used to keep the matrix data used by the solver alive, and will be used to init a clarabel::CscMatrix<T> object, which is then passed to the solver.
        static std::unique_ptr<ConvertedCscMatrix> eigen_sparse_to_clarabel(
            const Eigen::SparseMatrix<T, Eigen::ColMajor> &matrix)
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

            ConvertedCscMatrix *csc_matrix = new ConvertedCscMatrix(
                static_cast<uintptr_t>(matrix.rows()),
                static_cast<uintptr_t>(matrix.cols()),
                std::move(col_pointers),
                std::move(row_indices),
                nzval_ptr);

            return std::unique_ptr<ConvertedCscMatrix>(csc_matrix);
        }

        static void check_dimensions(
            const Eigen::SparseMatrix<T, Eigen::ColMajor> &P,
            const Eigen::Ref<const Eigen::VectorX<T>> &q,
            const Eigen::SparseMatrix<T, Eigen::ColMajor> &A,
            const Eigen::Ref<const Eigen::VectorX<T>> &b)
        {
            if (P.rows() != P.cols())
            {
                throw std::invalid_argument("P must be a square matrix");
            }

            int n = P.rows();
            int m = q.size();
            if (P.rows() != q.size())
            {
                throw std::invalid_argument("P and q must have the same number of rows");
            }

            if (A.cols() != P.cols())
            {
                throw std::invalid_argument("A and P must have the same number of columns");
            }

            if (A.rows() != b.size())
            {
                throw std::invalid_argument("A and b must have the same number of rows");
            }
        }

    public:
        // Lifetime of problem data:
        // - Matrices P, A are converted and the converted colptr and rowptr are kept alive by the unique_ptr, but matrix data is not copied, so the Eigen sparse matrices must be kept alive until the solver is destroyed.
        // - Vectors q, b, cones and the settings are copied when the DefaultSolver object is created in Rust.
        DefaultSolver(
            const Eigen::SparseMatrix<T, Eigen::ColMajor> &P,
            const Eigen::Ref<Eigen::VectorX<T>> &q,
            const Eigen::SparseMatrix<T, Eigen::ColMajor> &A,
            const Eigen::Ref<Eigen::VectorX<T>> &b,
            const std::vector<SupportedConeT<T>> &cones,
            const DefaultSettings<T> &settings);

        ~DefaultSolver();

        void solve();
        DefaultSolution<T> solution() const;
        DefaultInfo<T> info() const;
    };

    template<typename T>
    struct DefaultSolver<T>::ConvertedCscMatrix
    {
        uintptr_t m;
        uintptr_t n;
        const std::vector<uintptr_t> colptr;
        const std::vector<uintptr_t> rowval;
        const T *nzval;

        // Use move semantics to avoid copying the data
        ConvertedCscMatrix(
            uintptr_t m,
            uintptr_t n,
            std::vector<uintptr_t> &&colptr,
            std::vector<uintptr_t> &&rowval,
            const T *nzval)
            : m(m), n(n),
              colptr(std::move(colptr)),
              rowval(std::move(rowval)),
              nzval(nzval)
        {
        }
    };

    extern "C"
    {
        RustDefaultSolverHandle_f64 clarabel_DefaultSolver_f64_new(
            const CscMatrix<double> *P,
            const double *q,
            const CscMatrix<double> *A,
            const double *b,
            uintptr_t n_cones,
            const SupportedConeT<double> *cones,
            const DefaultSettings<double> *settings);

        RustDefaultSolverHandle_f32 clarabel_DefaultSolver_f32_new(
            const CscMatrix<float> *P,
            const float *q,
            const CscMatrix<float> *A,
            const float *b,
            uintptr_t n_cones,
            const SupportedConeT<float> *cones,
            const DefaultSettings<float> *settings);

        void clarabel_DefaultSolver_f64_solve(RustDefaultSolverHandle_f64 solver);

        void clarabel_DefaultSolver_f32_solve(RustDefaultSolverHandle_f32 solver);

        void clarabel_DefaultSolver_f64_free(RustDefaultSolverHandle_f64 solver);

        void clarabel_DefaultSolver_f32_free(RustDefaultSolverHandle_f32 solver);

        DefaultSolution<double> clarabel_DefaultSolver_f64_solution(RustDefaultSolverHandle_f64 solver);

        DefaultSolution<float> clarabel_DefaultSolver_f32_solution(RustDefaultSolverHandle_f32 solver);

        DefaultInfo<double> clarabel_DefaultSolver_f64_info(RustDefaultSolverHandle_f64 solver);

        DefaultInfo<float> clarabel_DefaultSolver_f32_info(RustDefaultSolverHandle_f32 solver);
    }

    // Convert unique_ptr P, A to CscMatrix objects, then init the solver
    // The CscMatrix objects are only used to pass the information needed to Rust.
    // The colptr and rowptr are stored in the ConvertedCscMatrix objects, which are kept alive by the unique_ptr.
    // No conversion is needed for nzval, so the Eigen sparse matrices must be kept alive until the solver is destroyed.
    template<>
    inline DefaultSolver<double>::DefaultSolver(
        const Eigen::SparseMatrix<double, Eigen::ColMajor> &P,
        const Eigen::Ref<Eigen::VectorX<double>> &q,
        const Eigen::SparseMatrix<double, Eigen::ColMajor> &A,
        const Eigen::Ref<Eigen::VectorX<double>> &b,
        const std::vector<SupportedConeT<double>> &cones,
        const DefaultSettings<double> &settings)
    {
        check_dimensions(P, q, A, b); // Rust wrapper will assume the pointers represent matrices with the right dimensions.

        // segfault will occur if the dimensions are incorrect
        matrix_P = DefaultSolver<double>::eigen_sparse_to_clarabel(P);
        matrix_A = DefaultSolver<double>::eigen_sparse_to_clarabel(A);
        CscMatrix<double> p(matrix_P->m, matrix_P->n, matrix_P->colptr.data(), matrix_P->rowval.data(), matrix_P->nzval);
        CscMatrix<double> a(matrix_A->m, matrix_A->n, matrix_A->colptr.data(), matrix_A->rowval.data(), matrix_A->nzval);

        this->handle = clarabel_DefaultSolver_f64_new(&p, q.data(), &a, b.data(), cones.size(), cones.data(), &settings);
    }

    template<>
    inline DefaultSolver<float>::DefaultSolver(
        const Eigen::SparseMatrix<float, Eigen::ColMajor> &P,
        const Eigen::Ref<Eigen::VectorX<float>> &q,
        const Eigen::SparseMatrix<float, Eigen::ColMajor> &A,
        const Eigen::Ref<Eigen::VectorX<float>> &b,
        const std::vector<SupportedConeT<float>> &cones,
        const DefaultSettings<float> &settings)
        : matrix_P(DefaultSolver<float>::eigen_sparse_to_clarabel(P)),
          matrix_A(DefaultSolver<float>::eigen_sparse_to_clarabel(A))
    {
        check_dimensions(P, q, A, b); // Rust wrapper will assume the pointers represent matrices with the right dimensions.

        // segfault will occur if the dimensions are incorrect
        matrix_P = DefaultSolver<float>::eigen_sparse_to_clarabel(P);
        matrix_A = DefaultSolver<float>::eigen_sparse_to_clarabel(A);
        CscMatrix<float> p(matrix_P->m, matrix_P->n, matrix_P->colptr.data(), matrix_P->rowval.data(), matrix_P->nzval);
        CscMatrix<float> a(matrix_A->m, matrix_A->n, matrix_A->colptr.data(), matrix_A->rowval.data(), matrix_A->nzval);

        this->handle = clarabel_DefaultSolver_f32_new(&p, q.data(), &a, b.data(), cones.size(), cones.data(), &settings);
    }

    template<>
    inline DefaultSolver<double>::~DefaultSolver()
    {
        if (handle != nullptr) // handle is null when an exception is thrown in the constructor
            clarabel_DefaultSolver_f64_free(handle);
    }

    template<>
    inline DefaultSolver<float>::~DefaultSolver()
    {
        if (handle != nullptr)
            clarabel_DefaultSolver_f32_free(handle);
    }

    
    template<>
    inline void DefaultSolver<double>::solve()
    {
        clarabel_DefaultSolver_f64_solve(handle);
    }

    template<>
    inline void DefaultSolver<float>::solve()
    {
        clarabel_DefaultSolver_f32_solve(handle);
    }


    template<>
    inline DefaultSolution<double> DefaultSolver<double>::solution() const
    {
        return clarabel_DefaultSolver_f64_solution(handle);
    }

    template<>
    inline DefaultSolution<float> DefaultSolver<float>::solution() const
    {
        return clarabel_DefaultSolver_f32_solution(handle);
    }

    
    template<>
    inline DefaultInfo<double> DefaultSolver<double>::info() const
    {
        return clarabel_DefaultSolver_f64_info(handle);
    }

    template<>
    inline DefaultInfo<float> DefaultSolver<float>::info() const
    {
        return clarabel_DefaultSolver_f32_info(handle);
    }

} // namespace clarabel