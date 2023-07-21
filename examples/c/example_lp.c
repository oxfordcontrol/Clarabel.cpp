#include "clarabel.h"
#include "utils.h"

int main(void)
{
    // 2 x 2 zero matrix
    ClarabelCscMatrix_f64 *P = clarabel_CscMatrix_f64_new(
        2,
        2,
        (uintptr_t[]){0, 0, 0},
        NULL,
        NULL
    );

    double q[2] = {1.0, -1.0};

    // a 2-d box constraint, separated into 4 inequalities.
    // A = [I; -I]
    ClarabelCscMatrix_f64 *A = clarabel_CscMatrix_f64_new(
        4,
        2,                               // row, col
        (uintptr_t[]){0, 2, 4},          // colptr
        (uintptr_t[]){0, 2, 1, 3},       // rowval
        (double[]){1.0, -1.0, 1.0, -1.0} // nzval
    );

    double b[4] = {1.0, 1.0, 1.0, 1.0};

    ClarabelSupportedConeT_f64 cones[1] =
    {
        ClarabelNonnegativeConeT_f64(4)
    };

    // Settings
    ClarabelDefaultSettings_f64 settings = DefaultSettingsBuilder_default();
    settings.equilibrate_enable = true;
    settings.equilibrate_max_iter = 50;

    // Build solver
    ClarabelDefaultSolver_f64 *solver = DefaultSolver_new(
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
    ClarabelDefaultSolution_f64 solution = DefaultSolver_solution(solver);
    print_solution_f64(&solution);

    // Free the matrices and the solver
    DefaultSolver_free(solver);
    clarabel_CscMatrix_f64_free(P);
    clarabel_CscMatrix_f64_free(A);

    return 0;
}