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
    CscMatrix_f64 *P = CscMatrix_f64_zeros(6, 6);
    double q[6] = {0., 0., -1., 0., 0., -1.};

    CscMatrix_f64 *A = CscMatrix_f64_from(
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

    SupportedConeT_f64 cones[4] =
    {
        PowerConeT_f64(0.6),
        PowerConeT_f64(0.1),
        ZeroConeT_f64(1),
        ZeroConeT_f64(1)
    };

    // Settings
    DefaultSettings_f64 settings = DefaultSettingsBuilder_f64_default();
    settings.verbose = true;
    settings.max_iter = 100;

    // Build solver
    DefaultSolver *solver = DefaultSolver_f64_new(
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
    DefaultSolution_f64 solution = DefaultSolver_f64_solution(solver);
    print_solution(&solution);

    // Free the matrices and the solver
    free_DefaultSolver(solver);
    free_CscMatrix_f64(P);
    free_CscMatrix_f64(A);

    return 0;
}