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
    // 6 x 6 zero matrix
    ClarabelCscMatrix *P = clarabel_CscMatrix_new(
        6,
        6,
        (uintptr_t[]){0, 0, 0, 0, 0, 0, 0},
        NULL,
        NULL
    );

    ClarabelFloat q[6] = {0., 0., -1., 0., 0., -1.};

    /* From dense matrix:
    * [[-1., 0., 0., 0., 0., 0.],
    *  [0., -1., 0., 0., 0., 0.],
    *  [0., 0., -1., 0., 0., 0.],
    *  [0., 0., 0., -1., 0., 0.],
    *  [0., 0., 0., 0., -1., 0.],
    *  [0., 0., 0., 0., 0., -1.],
    *  [1., 2., 0., 3., 0., 0.],
    *  [0., 0., 0., 0., 1., 0.]]
    */
    ClarabelFloat A_nzvalues[] = {-1.0, 1.0, -1.0, 2.0, -1.0, -1.0, 3.0, -1.0, 1.0, -1.0};
    ClarabelCscMatrix *A = clarabel_CscMatrix_new(
        8,
        6,
        (uintptr_t[]){0, 2, 4, 5, 7, 9, 10},
        (uintptr_t[]){0, 6, 1, 6, 2, 3, 6, 4, 7, 5},
        A_nzvalues
    );

    ClarabelFloat b[8] = {0., 0., 0., 0., 0., 0., 3., 1.};

    ClarabelSupportedConeT cones[4] =
    {
        ClarabelPowerConeT(0.6),
        ClarabelPowerConeT(0.1),
        ClarabelZeroConeT(1),
        ClarabelZeroConeT(1)
    };

    // Settings
    ClarabelDefaultSettings settings = clarabel_DefaultSettingsBuilder_default();
    settings.verbose = true;
    settings.max_iter = 100;

    // Build solver
    ClarabelDefaultSolver *solver = clarabel_DefaultSolver_new(
        P, // P
        q, // q
        A, // A
        b, // b
        4, // n_cones
        cones,
        &settings
    );

    // Solve
    clarabel_DefaultSolver_solve(solver);

    // Get solution
    ClarabelDefaultSolution solution = clarabel_DefaultSolver_solution(solver);
    print_solution(&solution);

    // Free the matrices and the solver
    clarabel_DefaultSolver_free(solver);
    clarabel_CscMatrix_free(P);
    clarabel_CscMatrix_free(A);

    return 0;
}