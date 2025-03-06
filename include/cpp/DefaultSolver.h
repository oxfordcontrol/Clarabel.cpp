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

    // Helper function for converting a Eigen sparse matrix into a temporary object of type ConvertedCscMatrix<T>
    // The temporary object is used to provide the problem data for constructing the solver.
    static ConvertedCscMatrix eigen_sparse_to_clarabel(const Eigen::SparseMatrix<T, Eigen::ColMajor> &matrix)
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

        ConvertedCscMatrix csc_matrix(static_cast<uintptr_t>(matrix.rows()), static_cast<uintptr_t>(matrix.cols()),
                                      std::move(col_pointers), std::move(row_indices), nzval_ptr);

        return std::move(csc_matrix);
    }

    static void check_dimensions(const Eigen::SparseMatrix<T, Eigen::ColMajor> &P,
                                 const Eigen::Ref<const Eigen::VectorX<T>> &q,
                                 const Eigen::SparseMatrix<T, Eigen::ColMajor> &A,
                                 const Eigen::Ref<const Eigen::VectorX<T>> &b,
                                 const std::vector<SupportedConeT<T>> &cones)
    {
        if (P.rows() != P.cols())
        {
            throw std::invalid_argument("P must be a square matrix");
        }

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

        unsigned int p = 0;
        for (const auto &cone : cones)
        {
            p += cone.nvars();
        }
        if (p != b.size())
        {
            throw std::invalid_argument("Constraint dimensions inconsistent with size of cones");
        }
    }

  public:
    // Lifetime of problem data: matrices P, A, vectors q, b, cones and the settings are copied when the DefaultSolver
    // object is created in Rust. Eigen::SparseMatrix objects need to be converted to the format supported by Clarabel.
    // Since the problem data is copied, the converted data exists as local variables in the constructor and is not
    // stored as part of this class.
    DefaultSolver(const Eigen::SparseMatrix<T, Eigen::ColMajor> &P,
                  const Eigen::Ref<Eigen::VectorX<T>> &q,
                  const Eigen::SparseMatrix<T, Eigen::ColMajor> &A,
                  const Eigen::Ref<Eigen::VectorX<T>> &b,
                  const std::vector<SupportedConeT<T>> &cones,
                  const DefaultSettings<T> &settings);

    DefaultSolver(void* handle);
    ~DefaultSolver();

    DefaultSolver(const DefaultSolver &) = delete;
    DefaultSolver(DefaultSolver &&other);
    DefaultSolver &operator=(const DefaultSolver &) = delete;
    DefaultSolver &operator=(DefaultSolver &&other);

    void solve();

    // The solution can only be obtained when the solver is in the Solved state, and the DefaultSolution object is only
    // valid when the solver is alive.
    DefaultSolution<T> solution() const;
    DefaultInfo<T> info() const;

    // problem data updating functions 
    // ------------------------------- 

    // update P 
    void update_P(const Eigen::SparseMatrix<T, Eigen::ColMajor> &P);
    void update_P(const Eigen::Ref<Eigen::VectorX<T>> &Pnzval);
    void update_P(const T* Pnzval, uintptr_t nnzP);
    void update_P(const Eigen::Ref<Eigen::VectorX<uintptr_t>> &index, const Eigen::Ref<Eigen::VectorX<T>> &values);
    void update_P(const uintptr_t* index, const T* values, uintptr_t nvals);

    // update A
    void update_A(const Eigen::SparseMatrix<T, Eigen::ColMajor> &A);
    void update_A(const Eigen::Ref<Eigen::VectorX<T>> &Anzval);
    void update_A(const T* Pnzval, uintptr_t nnzA);
    void update_A(const Eigen::Ref<Eigen::VectorX<uintptr_t>> &index, const Eigen::Ref<Eigen::VectorX<T>> &values);
    void update_A(const uintptr_t* index, const T* values, uintptr_t nvals);

    // update q
    void update_q(const Eigen::Ref<Eigen::VectorX<T>> &Anzval);
    void update_q(const T* values, uintptr_t nvals);
    void update_q(const Eigen::Ref<Eigen::VectorX<uintptr_t>> &index, const Eigen::Ref<Eigen::VectorX<T>> &values);
    void update_q(const uintptr_t* index, const T* values, uintptr_t nvals);

    // update b
    void update_b(const Eigen::Ref<Eigen::VectorX<T>> &Anzval);
    void update_b(const T* values, uintptr_t nvals);
    void update_b(const Eigen::Ref<Eigen::VectorX<uintptr_t>> &index, const Eigen::Ref<Eigen::VectorX<T>> &values);
    void update_b(const uintptr_t* index, const T* values, uintptr_t nvals);

    // Read / write to JSON file 
    #ifdef FEATURE_SERDE
    void save_to_file(const std::string &filename);
    static DefaultSolver<T> load_from_file(const std::string &filename);
    #endif

    // print stream configurations 
    void print_to_stdout();
    void print_to_file(const std::string &filename);
    void print_to_buffer();
    std::string get_print_buffer();

};

template<typename T>
DefaultSolver<T>::DefaultSolver(DefaultSolver &&other) : handle(other.handle)
{
    other.handle = nullptr;
}

template<typename T>
DefaultSolver<T> &DefaultSolver<T>::operator=(DefaultSolver &&other)
{
    if (this != &other){
        handle = other.handle;
        other.handle = nullptr;
    }
    return *this;
}

template<typename T>
struct DefaultSolver<T>::ConvertedCscMatrix
{
    uintptr_t m;
    uintptr_t n;
    const std::vector<uintptr_t> colptr;
    const std::vector<uintptr_t> rowval;
    const T *nzval;

    ConvertedCscMatrix(
        uintptr_t m, uintptr_t n, std::vector<uintptr_t> &&colptr, std::vector<uintptr_t> &&rowval, const T *nzval)
        : m(m), n(n), colptr(std::move(colptr)), rowval(std::move(rowval)), nzval(nzval)
    {
    }
};

extern "C" {
RustDefaultSolverHandle_f64 clarabel_DefaultSolver_f64_new(const CscMatrix<double> *P,
                                                           const double *q,
                                                           const CscMatrix<double> *A,
                                                           const double *b,
                                                           uintptr_t n_cones,
                                                           const SupportedConeT<double> *cones,
                                                           const DefaultSettings<double> *settings);

RustDefaultSolverHandle_f32 clarabel_DefaultSolver_f32_new(const CscMatrix<float> *P,
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

DefaultSolution<double>::ClarabelDefaultSolution
clarabel_DefaultSolver_f64_solution(RustDefaultSolverHandle_f64 solver);
DefaultSolution<float>::ClarabelDefaultSolution clarabel_DefaultSolver_f32_solution(RustDefaultSolverHandle_f32 solver);

DefaultInfo<double> clarabel_DefaultSolver_f64_info(RustDefaultSolverHandle_f64 solver);

DefaultInfo<float> clarabel_DefaultSolver_f32_info(RustDefaultSolverHandle_f32 solver);

void clarabel_DefaultSolver_f64_update_P_csc(RustDefaultSolverHandle_f64 solver, const CscMatrix<double> *P);
void clarabel_DefaultSolver_f32_update_P_csc(RustDefaultSolverHandle_f32 solver, const CscMatrix<float> *P);
void clarabel_DefaultSolver_f64_update_P(RustDefaultSolverHandle_f64 solver, const double *Pnzval, uintptr_t nnzP);
void clarabel_DefaultSolver_f32_update_P(RustDefaultSolverHandle_f32 solver, const float  *Pnzval, uintptr_t nnzP);
void clarabel_DefaultSolver_f64_update_P_partial(RustDefaultSolverHandle_f64 solver, const uintptr_t* index, const double *values, uintptr_t nvals);
void clarabel_DefaultSolver_f32_update_P_partial(RustDefaultSolverHandle_f32 solver, const uintptr_t* index, const float *values, uintptr_t nvals);

void clarabel_DefaultSolver_f64_update_A_csc(RustDefaultSolverHandle_f64 solver, const CscMatrix<double> *A);
void clarabel_DefaultSolver_f32_update_A_csc(RustDefaultSolverHandle_f32 solver, const CscMatrix<float> *A);
void clarabel_DefaultSolver_f64_update_A(RustDefaultSolverHandle_f64 solver, const double *Anzval, uintptr_t nnzA);
void clarabel_DefaultSolver_f32_update_A(RustDefaultSolverHandle_f32 solver, const float  *Anzval, uintptr_t nnzA);
void clarabel_DefaultSolver_f64_update_A_partial(RustDefaultSolverHandle_f64 solver, const uintptr_t* index, const double *values, uintptr_t nvals);
void clarabel_DefaultSolver_f32_update_A_partial(RustDefaultSolverHandle_f32 solver, const uintptr_t* index, const float *values, uintptr_t nvals);

void clarabel_DefaultSolver_f64_update_q(RustDefaultSolverHandle_f64 solver, const double *values, uintptr_t n);
void clarabel_DefaultSolver_f32_update_q(RustDefaultSolverHandle_f32 solver, const float  *values, uintptr_t n);
void clarabel_DefaultSolver_f64_update_q_partial(RustDefaultSolverHandle_f64 solver, const uintptr_t* index, const double *values, uintptr_t nvals);
void clarabel_DefaultSolver_f32_update_q_partial(RustDefaultSolverHandle_f32 solver, const uintptr_t* index, const float *values, uintptr_t nvals);

void clarabel_DefaultSolver_f64_update_b(RustDefaultSolverHandle_f64 solver, const double *values, uintptr_t n);
void clarabel_DefaultSolver_f32_update_b(RustDefaultSolverHandle_f32 solver, const float  *values, uintptr_t n);
void clarabel_DefaultSolver_f64_update_b_partial(RustDefaultSolverHandle_f64 solver, const uintptr_t* index, const double *values, uintptr_t nvals);
void clarabel_DefaultSolver_f32_update_b_partial(RustDefaultSolverHandle_f32 solver, const uintptr_t* index, const float *values, uintptr_t nvals);

#ifdef FEATURE_SERDE
void clarabel_DefaultSolver_f64_save_to_file(RustDefaultSolverHandle_f64 solver, const char *filename);
void clarabel_DefaultSolver_f32_save_to_file(RustDefaultSolverHandle_f32 solver, const char *filename);
RustDefaultSolverHandle_f64 clarabel_DefaultSolver_f64_load_from_file( const char *filename);
RustDefaultSolverHandle_f32 clarabel_DefaultSolver_f32_load_from_file( const char *filename);
#endif

void clarabel_DefaultSolver_f64_print_to_stdout(RustDefaultSolverHandle_f64 solver);
void clarabel_DefaultSolver_f32_print_to_stdout(RustDefaultSolverHandle_f32 solver);
void clarabel_DefaultSolver_f64_print_to_file(RustDefaultSolverHandle_f64 solver, const char *filename);
void clarabel_DefaultSolver_f32_print_to_file(RustDefaultSolverHandle_f32 solver, const char *filename);
void clarabel_DefaultSolver_f64_print_to_buffer(RustDefaultSolverHandle_f64 solver);
void clarabel_DefaultSolver_f32_print_to_buffer(RustDefaultSolverHandle_f32 solver);
const char* clarabel_DefaultSolver_f64_get_print_buffer(RustDefaultSolverHandle_f64 solver);
const char* clarabel_DefaultSolver_f32_get_print_buffer(RustDefaultSolverHandle_f32 solver);
void clarabel_free_print_buffer(const char *s);

} // extern "C"



// Convert unique_ptr P, A to CscMatrix objects, then init the solver
// The CscMatrix objects are only used to pass the information needed to Rust.
// The colptr and rowptr are stored in the ConvertedCscMatrix objects, which are kept alive by the unique_ptr.
// No conversion is needed for nzval, so the Eigen sparse matrices must be kept alive until the solver is destroyed.
template<>
inline DefaultSolver<double>::DefaultSolver(const Eigen::SparseMatrix<double, Eigen::ColMajor> &P,
                                            const Eigen::Ref<Eigen::VectorX<double>> &q,
                                            const Eigen::SparseMatrix<double, Eigen::ColMajor> &A,
                                            const Eigen::Ref<Eigen::VectorX<double>> &b,
                                            const std::vector<SupportedConeT<double>> &cones,
                                            const DefaultSettings<double> &settings)
{
    // Rust wrapper will assume the pointers represent matrices with the right dimensions.
    // segfault will occur if the dimensions are incorrect
    check_dimensions(P, q, A, b, cones);

    ConvertedCscMatrix matrix_P = DefaultSolver<double>::eigen_sparse_to_clarabel(P);
    ConvertedCscMatrix matrix_A = DefaultSolver<double>::eigen_sparse_to_clarabel(A);
    CscMatrix<double> p(matrix_P.m, matrix_P.n, matrix_P.colptr.data(), matrix_P.rowval.data(), matrix_P.nzval);
    CscMatrix<double> a(matrix_A.m, matrix_A.n, matrix_A.colptr.data(), matrix_A.rowval.data(), matrix_A.nzval);

    this->handle = clarabel_DefaultSolver_f64_new(&p, q.data(), &a, b.data(), cones.size(), cones.data(), &settings);
}

template<>
inline DefaultSolver<float>::DefaultSolver(const Eigen::SparseMatrix<float, Eigen::ColMajor> &P,
                                           const Eigen::Ref<Eigen::VectorX<float>> &q,
                                           const Eigen::SparseMatrix<float, Eigen::ColMajor> &A,
                                           const Eigen::Ref<Eigen::VectorX<float>> &b,
                                           const std::vector<SupportedConeT<float>> &cones,
                                           const DefaultSettings<float> &settings)
{
    // Rust wrapper will assume the pointers represent matrices with the right dimensions.
    // segfault will occur if the dimensions are incorrect
    check_dimensions(P, q, A, b, cones);

    ConvertedCscMatrix matrix_P = DefaultSolver<float>::eigen_sparse_to_clarabel(P);
    ConvertedCscMatrix matrix_A = DefaultSolver<float>::eigen_sparse_to_clarabel(A);
    CscMatrix<float> p(matrix_P.m, matrix_P.n, matrix_P.colptr.data(), matrix_P.rowval.data(), matrix_P.nzval);
    CscMatrix<float> a(matrix_A.m, matrix_A.n, matrix_A.colptr.data(), matrix_A.rowval.data(), matrix_A.nzval);

    this->handle = clarabel_DefaultSolver_f32_new(&p, q.data(), &a, b.data(), cones.size(), cones.data(), &settings);
}

template<>
inline DefaultSolver<double>::DefaultSolver(void* handle){
    this->handle = handle;
}

template<>
inline DefaultSolver<float>::DefaultSolver(void* handle){
    this->handle = handle;
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
    auto solution = clarabel_DefaultSolver_f64_solution(handle);
    return std::move(DefaultSolution<double>(solution));
}

template<>
inline DefaultSolution<float> DefaultSolver<float>::solution() const
{
    auto solution = clarabel_DefaultSolver_f32_solution(handle);
    return std::move(DefaultSolution<float>(solution));
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


// update P

template<>
inline void DefaultSolver<double>::update_P(const Eigen::SparseMatrix<double, Eigen::ColMajor> &P){
    ConvertedCscMatrix matrix_P = DefaultSolver<double>::eigen_sparse_to_clarabel(P);
    CscMatrix<double> mat(matrix_P.m, matrix_P.n, matrix_P.colptr.data(), matrix_P.rowval.data(), matrix_P.nzval);
     clarabel_DefaultSolver_f64_update_P_csc(this->handle,&mat);
}

template<>
inline void DefaultSolver<float>::update_P(const Eigen::SparseMatrix<float, Eigen::ColMajor> &P){
    ConvertedCscMatrix matrix_P = DefaultSolver<float>::eigen_sparse_to_clarabel(P);
    CscMatrix<float> mat(matrix_P.m, matrix_P.n, matrix_P.colptr.data(), matrix_P.rowval.data(), matrix_P.nzval);
    clarabel_DefaultSolver_f32_update_P_csc(this->handle,&mat);
}

template<>
inline void DefaultSolver<double>::update_P(const Eigen::Ref<Eigen::VectorX<double>> &nzval){
     clarabel_DefaultSolver_f64_update_P(this->handle,nzval.data(), nzval.size());
}

template<>
inline void DefaultSolver<float>::update_P(const Eigen::Ref<Eigen::VectorX<float>> &nzval){
    clarabel_DefaultSolver_f32_update_P(this->handle,nzval.data(), nzval.size());
}

template<>
inline void DefaultSolver<double>::update_P(const double *nzval, uintptr_t nnz){
     clarabel_DefaultSolver_f64_update_P(this->handle, nzval, nnz);
}

template<>
inline void DefaultSolver<float>::update_P(const float *nzval, uintptr_t nnz){
    clarabel_DefaultSolver_f32_update_P(this->handle, nzval, nnz);
}

template<>
inline void DefaultSolver<double>::update_P(const Eigen::Ref<Eigen::VectorX<uintptr_t>> &index, const Eigen::Ref<Eigen::VectorX<double>> &values){
    if(index.size() != values.size()){
        throw std::invalid_argument("index and values must have the same size");
    }
     clarabel_DefaultSolver_f64_update_P_partial(this->handle, index.data(), values.data(), index.size());
}

template<>
inline void DefaultSolver<float>::update_P(const Eigen::Ref<Eigen::VectorX<uintptr_t>> &index, const Eigen::Ref<Eigen::VectorX<float>> &values){
    if(index.size() != values.size()){
        throw std::invalid_argument("index and values must have the same size");
    }
     clarabel_DefaultSolver_f32_update_P_partial(this->handle, index.data(), values.data(), index.size());
}

template<>
inline void DefaultSolver<double>::update_P(const uintptr_t* index, const double *values, uintptr_t nvals){
     clarabel_DefaultSolver_f64_update_P_partial(this->handle, index, values, nvals);
}

template<>
inline void DefaultSolver<float>::update_P(const uintptr_t* index, const float *values, uintptr_t nvals){
     clarabel_DefaultSolver_f32_update_P_partial(this->handle, index, values, nvals);
}


// update A

template<>
inline void DefaultSolver<double>::update_A(const Eigen::SparseMatrix<double, Eigen::ColMajor> &A){
    ConvertedCscMatrix matrix_A = DefaultSolver<double>::eigen_sparse_to_clarabel(A);
    CscMatrix<double> mat(matrix_A.m, matrix_A.n, matrix_A.colptr.data(), matrix_A.rowval.data(), matrix_A.nzval);
     clarabel_DefaultSolver_f64_update_A_csc(this->handle,&mat);
}

template<>
inline void DefaultSolver<float>::update_A(const Eigen::SparseMatrix<float, Eigen::ColMajor> &A){
    ConvertedCscMatrix matrix_A = DefaultSolver<float>::eigen_sparse_to_clarabel(A);
    CscMatrix<float> mat(matrix_A.m, matrix_A.n, matrix_A.colptr.data(), matrix_A.rowval.data(), matrix_A.nzval);
    clarabel_DefaultSolver_f32_update_A_csc(this->handle,&mat);
}

template<>
inline void DefaultSolver<double>::update_A(const Eigen::Ref<Eigen::VectorX<double>> &nzval){
     clarabel_DefaultSolver_f64_update_A(this->handle,nzval.data(), nzval.size());
}

template<>
inline void DefaultSolver<float>::update_A(const Eigen::Ref<Eigen::VectorX<float>> &nzval){
    clarabel_DefaultSolver_f32_update_A(this->handle,nzval.data(), nzval.size());
}

template<>
inline void DefaultSolver<double>::update_A(const double *nzval, uintptr_t nnz){
     clarabel_DefaultSolver_f64_update_A(this->handle, nzval, nnz);
}

template<>
inline void DefaultSolver<float>::update_A(const float *nzval, uintptr_t nnz){
    clarabel_DefaultSolver_f32_update_A(this->handle, nzval, nnz);
}

template<>
inline void DefaultSolver<double>::update_A(const Eigen::Ref<Eigen::VectorX<uintptr_t>> &index, const Eigen::Ref<Eigen::VectorX<double>> &values){
    if(index.size() != values.size()){
        throw std::invalid_argument("index and values must have the same size");
    }
     clarabel_DefaultSolver_f64_update_A_partial(this->handle, index.data(), values.data(), index.size());
}

template<>
inline void DefaultSolver<float>::update_A(const Eigen::Ref<Eigen::VectorX<uintptr_t>> &index, const Eigen::Ref<Eigen::VectorX<float>> &values){
    if(index.size() != values.size()){
        throw std::invalid_argument("index and values must have the same size");
    }
     clarabel_DefaultSolver_f32_update_A_partial(this->handle, index.data(), values.data(), index.size());
}

template<>
inline void DefaultSolver<double>::update_A(const uintptr_t* index, const double *values, uintptr_t nvals){
     clarabel_DefaultSolver_f64_update_A_partial(this->handle, index, values, nvals);
}

template<>
inline void DefaultSolver<float>::update_A(const uintptr_t* index, const float *values, uintptr_t nvals){
     clarabel_DefaultSolver_f32_update_A_partial(this->handle, index, values, nvals);
}


// update q

template<>
inline void DefaultSolver<double>::update_q(const Eigen::Ref<Eigen::VectorX<double>> &values){
     clarabel_DefaultSolver_f64_update_q(this->handle, values.data(), values.size());
}

template<>
inline void DefaultSolver<float>::update_q(const Eigen::Ref<Eigen::VectorX<float>> &values){
    clarabel_DefaultSolver_f32_update_q(this->handle, values.data(), values.size());
}

template<>
inline void DefaultSolver<double>::update_q(const double *values, uintptr_t n){
     clarabel_DefaultSolver_f64_update_q(this->handle, values, n);
}

template<>
inline void DefaultSolver<float>::update_q(const float *values, uintptr_t n){
    clarabel_DefaultSolver_f32_update_q(this->handle, values, n);
}

template<>
inline void DefaultSolver<double>::update_q(const Eigen::Ref<Eigen::VectorX<uintptr_t>> &index, const Eigen::Ref<Eigen::VectorX<double>> &values){
    if(index.size() != values.size()){
        throw std::invalid_argument("index and values must have the same size");
    }
     clarabel_DefaultSolver_f64_update_q_partial(this->handle, index.data(), values.data(), index.size());
}

template<>
inline void DefaultSolver<float>::update_q(const Eigen::Ref<Eigen::VectorX<uintptr_t>> &index, const Eigen::Ref<Eigen::VectorX<float>> &values){
    if(index.size() != values.size()){
        throw std::invalid_argument("index and values must have the same size");
    }
     clarabel_DefaultSolver_f32_update_q_partial(this->handle, index.data(), values.data(), index.size());
}

template<>
inline void DefaultSolver<double>::update_q(const uintptr_t* index, const double *values, uintptr_t nvals){
     clarabel_DefaultSolver_f64_update_q_partial(this->handle, index, values, nvals);
}

template<>
inline void DefaultSolver<float>::update_q(const uintptr_t* index, const float *values, uintptr_t nvals){
     clarabel_DefaultSolver_f32_update_q_partial(this->handle, index, values, nvals);
}

// update b

template<>
inline void DefaultSolver<double>::update_b(const Eigen::Ref<Eigen::VectorX<double>> &values){
     clarabel_DefaultSolver_f64_update_b(this->handle, values.data(), values.size());
}

template<>
inline void DefaultSolver<float>::update_b(const Eigen::Ref<Eigen::VectorX<float>> &values){
    clarabel_DefaultSolver_f32_update_b(this->handle, values.data(), values.size());
}

template<>
inline void DefaultSolver<double>::update_b(const double *values, uintptr_t n){
     clarabel_DefaultSolver_f64_update_b(this->handle, values, n);
}

template<>
inline void DefaultSolver<float>::update_b(const float *values, uintptr_t n){
    clarabel_DefaultSolver_f32_update_b(this->handle, values, n);
}

template<>
inline void DefaultSolver<double>::update_b(const Eigen::Ref<Eigen::VectorX<uintptr_t>> &index, const Eigen::Ref<Eigen::VectorX<double>> &values){
    if(index.size() != values.size()){
        throw std::invalid_argument("index and values must have the same size");
    }
     clarabel_DefaultSolver_f64_update_b_partial(this->handle, index.data(), values.data(), index.size());
}

template<>
inline void DefaultSolver<float>::update_b(const Eigen::Ref<Eigen::VectorX<uintptr_t>> &index, const Eigen::Ref<Eigen::VectorX<float>> &values){
    if(index.size() != values.size()){
        throw std::invalid_argument("index and values must have the same size");
    }
     clarabel_DefaultSolver_f32_update_b_partial(this->handle, index.data(), values.data(), index.size());
}

template<>
inline void DefaultSolver<double>::update_b(const uintptr_t* index, const double *values, uintptr_t nvals){
     clarabel_DefaultSolver_f64_update_b_partial(this->handle, index, values, nvals);
}

template<>
inline void DefaultSolver<float>::update_b(const uintptr_t* index, const float *values, uintptr_t nvals){
     clarabel_DefaultSolver_f32_update_b_partial(this->handle, index, values, nvals);
}

#ifdef FEATURE_SERDE
template<>
inline void DefaultSolver<double>::save_to_file(const std::string &filename){
    clarabel_DefaultSolver_f64_save_to_file(this->handle, filename.c_str());
}

template<>
inline void DefaultSolver<float>::save_to_file(const std::string &filename){
    clarabel_DefaultSolver_f32_save_to_file(this->handle, filename.c_str());
}

template<>
inline DefaultSolver<double> DefaultSolver<double>::load_from_file(const std::string &filename){
    RustDefaultSolverHandle_f64 handle = clarabel_DefaultSolver_f64_load_from_file(filename.c_str());
    return DefaultSolver<double>(handle);
}

template<>
inline DefaultSolver<float> DefaultSolver<float>::load_from_file(const std::string &filename){
    RustDefaultSolverHandle_f32 handle = clarabel_DefaultSolver_f32_load_from_file(filename.c_str());
    return DefaultSolver<float>(handle);
}
#endif // FEATURE_SERDE

// print configurations

template<>
inline void DefaultSolver<double>::print_to_stdout(){
    clarabel_DefaultSolver_f64_print_to_stdout(this->handle);
}
template<>
inline void DefaultSolver<float>::print_to_stdout(){
    clarabel_DefaultSolver_f32_print_to_stdout(this->handle);
}
template<>
inline void DefaultSolver<double>::print_to_file(const std::string &filename){
    clarabel_DefaultSolver_f64_print_to_file(this->handle, filename.c_str());
}
template<>
inline void DefaultSolver<float>::print_to_file(const std::string &filename){
    clarabel_DefaultSolver_f32_print_to_file(this->handle, filename.c_str());
}
template<>
inline void DefaultSolver<double>::print_to_buffer(){
    clarabel_DefaultSolver_f64_print_to_buffer(this->handle);
}
template<>
inline void DefaultSolver<float>::print_to_buffer(){
    clarabel_DefaultSolver_f32_print_to_buffer(this->handle);
}
template<>
inline std::string DefaultSolver<double>::get_print_buffer(){
    const char* buffer = clarabel_DefaultSolver_f64_get_print_buffer(this->handle);
    std::string str(buffer);
    clarabel_free_print_buffer(buffer);
    return str;
}
template<>
inline std::string DefaultSolver<float>::get_print_buffer(){
    const char* buffer = clarabel_DefaultSolver_f32_get_print_buffer(this->handle);
    std::string str(buffer);
    clarabel_free_print_buffer(buffer);
    return str;
}

} // namespace clarabel