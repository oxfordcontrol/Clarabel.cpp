#include "clarabel.h"
#include "utils.h"

int main(void)
{
    CscMatrix *P = CscMatrix_zeros(2, 2);
    double q[2] = {1.0, -1.0};

    // a 2-d box constraint, separated into 4 inequalities.
    // A = [I; -I]
    CscMatrix *_A = CscMatrix_new(
        4, 2,                            // row, col
        (uintptr_t[]){0, 2, 4},          // colptr
        (uintptr_t[]){0, 2, 1, 3},       // rowval
        (double[]){1.0, -1.0, 1.0, -1.0} // nzval
    );

    // easier way - construct A from a dense matrix
    CscMatrix *A = CscMatrix_from(
        4, 2,
        (double[4][2])
        {
            {1.0, 0.0},
            {0.0, 1.0},
            {-1.0, 0.0},
            {0.0, -1.0}
        }
    );

    double b[4] = {1.0, 1.0, 1.0, 1.0};

    SupportedConeT cones[1] =
    {
        NonnegativeConeT(4)
    };

    // Settings
    DefaultSettings settings = DefaultSettingsBuilder_default();
    settings.equilibrate_enable = true;
    settings.equilibrate_max_iter = 50;

    // Build solver
    DefaultSolver *solver = DefaultSolver_new(
        P, // P
        q, // q
        A, // A
        b, // b
        1, // n_cones
        cones,
        &settings);

    // Solve
    DefaultSolver_solve(solver);

    // Get solution
    DefaultSolution solution = DefaultSolver_solution(solver);
    print_solution(&solution);

    // Free the matrices and the solver
    DefaultSolver_free(solver);
    CscMatrix_free(P);
    CscMatrix_free(A);
    CscMatrix_free(_A);

    return 0;
}