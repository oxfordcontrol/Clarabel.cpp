#include "clarabel.h"
#include "utils.h"

// Power Cone Example
//
//  solve the following power cone problem
//  max  x1^0.6 y^0.4 + x2^0.1
//  s.t. x1, y, x2 >= 0
//       x1 + 2y  + 3x2 == 3
//  which is equivalent to
//  max z1 + z2
//  s.t. (x1, y, z1) in K_pow(0.6)
//       (x2, 1, z2) in K_pow(0.1)
//       x1 + 2y + 3x2 == 3

int main(void)
{
    CscMatrix *P = CscMatrix_zeros(6, 6);
    double q[6] = {0., 0., -1., 0., 0., -1.};

    CscMatrix *A = CscMatrix_from(
        8, 6,
        (double[8][6])
        {
            {-1., 0., 0., 0., 0., 0.},
            {0., -1., 0., 0., 0., 0.},
            {0., 0., -1., 0., 0., 0.},
            {0., 0., 0., -1., 0., 0.},
            {0., 0., 0., 0., -1., 0.},
            {0., 0., 0., 0., 0., -1.},
            {1., 2., 0., 3., 0., 0.},
            {0., 0., 0., 0., 1., 0.},
        }
    );

    double b[8] = {0., 0., 0., 0., 0., 0., 3., 1.};

    SupportedConeT cones[4] =
    {
        PowerConeT(0.6),
        PowerConeT(0.1),
        ZeroConeT(1),
        ZeroConeT(1)
    };

    // Settings
    DefaultSettings settings = DefaultSettingsBuilder_default();
    settings.verbose = true;
    settings.max_iter = 100;

    // Build solver
    DefaultSolver *solver = DefaultSolver_new(
        P, // P
        q, // q
        A, // A
        b, // b
        4, // n_cones
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