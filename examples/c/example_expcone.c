#include <math.h>
#include "clarabel.h"
#include "utils.h"

int main(void)
{
    // 3 x 3 zero matrix
    CscMatrix *P = CscMatrix_new(
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
    CscMatrix *A = CscMatrix_new(
        5,
        3,
        (uintptr_t[]){0, 1, 3, 5},
        (uintptr_t[]){0, 1, 3, 2, 4},
        (double[]){-1.0, -1.0, 1.0, -1.0, 1.0}
    );

    double b[5] = {0., 0., 0., 1., exp(5.0)};

    SupportedConeT cones[2] =
    {
        ExponentialConeT(),
        ZeroConeT(2)
    };

    // Settings
    DefaultSettings settings = DefaultSettingsBuilder_default();
    settings.verbose = true;

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
    CscMatrix_free(P);
    CscMatrix_free(A);

    return 0;
}