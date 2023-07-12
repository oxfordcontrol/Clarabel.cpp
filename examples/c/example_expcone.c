#include <math.h>
#include "clarabel.h"
#include "utils.h"

int main(void)
{
    CscMatrix_f64 *P = CscMatrix_f64_zeros(3, 3);
    double q[3] = {-1.0, 0.0, 0.0};

    CscMatrix_f64 *A = CscMatrix_f64_from(
        5, 3,
        (double[5][3])
        {
            {-1.0, 0., 0. },
            { 0., -1., 0. },
            { 0., 0., -1. },
            { 0., 1., 0. },
            { 0., 0., 1. },
        }
    );

    double b[5] = {0., 0., 0., 1., exp(5.0)};

    SupportedConeT_f64 cones[2] =
    {
        ExponentialConeT_f64(),
        ZeroConeT_f64(2)
    };

    // Settings
    DefaultSettings_f64 settings = DefaultSettingsBuilder_f64_default();
    settings.verbose = true;

    // Build solver
    DefaultSolver *solver = DefaultSolver_f64_new(
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
    DefaultSolution_f64 solution = DefaultSolver_f64_solution(solver);
    print_solution(&solution);

    // Free the matrices and the solver
    free_DefaultSolver(solver);
    free_CscMatrix_f64(P);
    free_CscMatrix_f64(A);

    return 0;
}