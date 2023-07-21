#include <math.h>
#include "clarabel.h"
#include "utils.h"

int main(void)
{
    // 3 x 3 zero matrix
    ClarabelCscMatrix_f64 *P = clarabel_CscMatrix_f64_new(
        3,
        3,
        (uintptr_t[]){0, 0, 0, 0},
        NULL,
        NULL
    );

    double q[3] = {-1.0, 0.0, 0.0};

    /* From dense matrix:
     * [-1.0, 0., 0.],
     * [0., -1., 0.],
     * [0., 0., -1.],
     * [0., 1., 0.],
     * [0., 0., 1.],
     */
    ClarabelCscMatrix_f64 *A = clarabel_CscMatrix_f64_new(
        5,
        3,
        (uintptr_t[]){0, 1, 3, 5},
        (uintptr_t[]){0, 1, 3, 2, 4},
        (double[]){-1.0, -1.0, 1.0, -1.0, 1.0}
    );

    double b[5] = {0., 0., 0., 1., exp(5.0)};

    ClarabelSupportedConeT_f64 cones[2] =
    {
        ClarabelExponentialConeT_f64(),
        ClarabelZeroConeT_f64(2)
    };

    // Settings
    ClarabelDefaultSettings_f64 settings = clarabel_DefaultSettingsBuilder_f64_default();
    settings.verbose = true;

    // Build solver
    ClarabelDefaultSolver_f64 *solver = clarabel_DefaultSolver_f64_new(
        P, // P
        q, // q
        A, // A
        b, // b
        2, // n_cones
        cones,
        &settings
    );

    // Solve
    clarabel_DefaultSolver_f64_solve(solver);

    // Get solution
    ClarabelDefaultSolution_f64 solution = clarabel_DefaultSolver_f64_solution(solver);
    print_solution_f64(&solution);

    // Free the matrices and the solver
    clarabel_DefaultSolver_f64_free(solver);
    clarabel_CscMatrix_f64_free(P);
    clarabel_CscMatrix_f64_free(A);

    return 0;
}