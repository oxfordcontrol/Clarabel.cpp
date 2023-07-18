#include <math.h>
#include "clarabel.h"
#include "utils.h"

int main(void)
{
    CscMatrix *P = CscMatrix_zeros(3, 3);
    double q[3] = {-1.0, 0.0, 0.0};

    CscMatrix *A = CscMatrix_from(
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