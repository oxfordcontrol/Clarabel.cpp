#include "clarabel.h"
#include "utils.h"

int main(void)
{
    // 2 x 2 zero matrix
    ClarabelCscMatrix *P = CscMatrix_new(
        2,
        2,
        (uintptr_t[]){0, 0, 0},
        NULL,
        NULL
    );

    double q[2] = {1.0, -1.0};

    // a 2-d box constraint, separated into 4 inequalities.
    // A = [I; -I]
    ClarabelCscMatrix *A = CscMatrix_new(
        4,
        2,                            // row, col
        (uintptr_t[]){0, 2, 4},          // colptr
        (uintptr_t[]){0, 2, 1, 3},       // rowval
        (double[]){1.0, -1.0, 1.0, -1.0} // nzval
    );

    double b[4] = {1.0, 1.0, 1.0, 1.0};

    ClarabelSupportedConeT cones[1] =
    {
        ClarabelNonnegativeConeT(4)
    };

    // Settings
    ClarabelDefaultSettings settings = DefaultSettingsBuilder_default();
    settings.equilibrate_enable = true;
    settings.equilibrate_max_iter = 50;

    // Build solver
    ClarabelDefaultSolver *solver = DefaultSolver_new(
        P, // P
        q, // q
        A, // A
        b, // b
        1, // n_cones
        cones,
        &settings
    );

    // Solve
    DefaultSolver_solve(solver);

    // Get solution
    ClarabelDefaultSolution solution = DefaultSolver_solution(solver);
    print_solution(&solution);

    // Free the matrices and the solver
    DefaultSolver_free(solver);
    CscMatrix_free(P);
    CscMatrix_free(A);

    return 0;
}