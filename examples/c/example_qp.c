#include "clarabel.h"

int main(void)
{
    // QP Example

    // let P = CscMatrix::identity(2);    // For P = I
    // let P = CscMatrix::zeros((2,2));   // For P = 0

    // direct from sparse data
    CscMatrix_f64 *_P = CscMatrix_f64_new(
        2,                      // m
        2,                      // n
        (uintptr_t[]){0, 1, 2}, // colptr
        (uintptr_t[]){0, 1},    // rowval
        (double[]){6., 4.}      // nzval
    );

    // or an easier way for small problems...
    CscMatrix_f64 *P = CscMatrix_f64_from(
        2, 2,
        (double[2][2])
        {
            {6., 0.},
            {0., 4.}
        }
    );

    double q[2] = {-1., -4.};

    // direct from sparse data
    CscMatrix_f64 *_A = CscMatrix_f64_new(
        5,                                    // m
        2,                                    // n
        (uintptr_t[]){0, 3, 6},               // colptr
        (uintptr_t[]){0, 1, 3, 0, 2, 4},      // rowval
        (double[]){1., 1., -1., -2., 1., -1.} // nzval
    );

    // or an easier way for small problems...
    CscMatrix_f64 *A = CscMatrix_f64_from(
        5, 2,
        (double[5][2])
        {
            { 1., -2.}, // <-- LHS of equality constraint (lower bound)
            { 1.,  0.}, // <-- LHS of inequality constraint (upper bound)
            { 0.,  1.}, // <-- LHS of inequality constraint (upper bound)
            {-1.,  0.}, // <-- LHS of inequality constraint (lower bound)
            { 0., -1.}  // <-- LHS of inequality constraint (lower bound)
        }
    );

    double b[5] = {0., 1., 1., 1., 1.};

    SupportedConeT_f64 cones[2] =
    {
        ZeroConeT_f64(1),
        NonnegativeConeT_f64(4)
    };

    // Settings
    DefaultSettings_f64 settings = DefaultSettingsBuilder_f64_default();

    // Build solver
    DefaultSolver *solver = DefaultSolver_f64_new(
        P, // P
        q, // q
        A, // A
        b, // b
        2, // n_cones
        cones,
        &settings);

    // Solve
    DefaultSolver_solve(solver);

    // Free the matrices and the solver
    free_DefaultSolver(solver);
    free_CscMatrix_f64(_P);
    free_CscMatrix_f64(P);
    free_CscMatrix_f64(_A);
    free_CscMatrix_f64(A);

    return 0;
}
