#include "clarabel.h"
#include "utils.h"

int main(void)
{
    // QP Example

    // let P = CscMatrix::identity(2);    // For P = I
    // let P = CscMatrix::zeros((2,2));   // For P = 0

    // direct from sparse data
    CscMatrix *_P = CscMatrix_new(
        2,                      // m
        2,                      // n
        (uintptr_t[]){0, 1, 2}, // colptr
        (uintptr_t[]){0, 1},    // rowval
        (double[]){6., 4.}      // nzval
    );

    // or an easier way for small problems...
    CscMatrix *P = CscMatrix_from(
        2, 2,
        (double[2][2])
        {
            {6., 0.},
            {0., 4.}
        }
    );

    double q[2] = {-1., -4.};

    // direct from sparse data
    CscMatrix *_A = CscMatrix_new(
        5,                                    // m
        2,                                    // n
        (uintptr_t[]){0, 3, 6},               // colptr
        (uintptr_t[]){0, 1, 3, 0, 2, 4},      // rowval
        (double[]){1., 1., -1., -2., 1., -1.} // nzval
    );

    // or an easier way for small problems...
    CscMatrix *A = CscMatrix_from(
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

    SupportedConeT cones[2] =
    {
        ZeroConeT(1),
        NonnegativeConeT(4)
    };

    // Settings
    DefaultSettings settings = DefaultSettingsBuilder_default();

    // Build solver
    DefaultSolver *solver = DefaultSolver_new(
        P, // P
        q, // q
        A, // A
        b, // b
        2, // n_cones
        cones,
        &settings
    );

    // Solve
    DefaultSolver_solve(solver);

    // Get solution
    DefaultSolution solution = DefaultSolver_solution(solver);
    print_solution(&solution);

    // Free the matrices and the solver
    DefaultSolver_free(solver);
    CscMatrix_free(_P);
    CscMatrix_free(P);
    CscMatrix_free(_A);
    CscMatrix_free(A);

    return 0;
}
