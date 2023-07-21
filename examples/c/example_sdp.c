// #define FEATURE_SDP
#include "clarabel.h"
#include "utils.h"
#include <math.h>
#include <stdio.h>

int main(void)
{
#ifndef FEATURE_SDP
    printf("This example requires SDP support.\n");
    return 1;
#else
    int n = 3;
    int nvec = (n * (n + 1)) >> 1;

    // SDP Example

    // 6 x 6 zero matrix
    CscMatrix *P = clarabel_CscMatrix_new(
        6,
        6,
        (uintptr_t[]){0, 0, 0, 0, 0, 0, 0},
        NULL,
        NULL
    );

    ClarabelFloat c[6] = {0., 0., -1., 0., 0., -1.};

    ClarabelFloat sqrt2 = sqrt(2.0);
    /* From dense matrix:
     * [[-1., 0., 0., 0., 0., 0.],
     *  [0., -sqrt2., 0., 0., 0., 0.],
     *  [0., 0., -1., 0., 0., 0.],
     *  [0., 0., 0., -sqrt2., 0., 0.],
     *  [0., 0., 0., 0., -sqrt2., 0.],
     *  [0., 0., 0., 0., 0., -1.],
     *  [1., 4., 3., 8., 10., 6.]]
     */
    ClarabelFloat A_nzvalues[] = {-1.0, 1.0, -sqrt2, 4.0, -1.0, 3.0, -sqrt2, 8.0, -sqrt2, 10.0, -1.0, 6.0};
    CscMatrix *A = clarabel_CscMatrix_new(
        7,
        6,
        (uintptr_t[]){0, 2, 4, 6, 8, 10, 12},
        (uintptr_t[]){0, 6, 1, 6, 2, 6, 3, 6, 4, 6, 5, 6},
        A_nzvalues
    );

    ClarabelFloat b[7] = {0., 0., 0., 0., 0., 0., 1.};

    SupportedConeT cones[2] =
    {
        PSDTriangleConeT(n),
        ZeroConeT(1),
    };

    // Settings
    DefaultSettings settings = clarabel_DefaultSettingsBuilder_default();

    // Build solver
    DefaultSolver *solver = clarabel_DefaultSolver_new(
        P, // P
        c, // q
        A, // A
        b, // b
        2, // n_cones
        cones,
        &settings
    );

    // Solve
    clarabel_DefaultSolver_solve(solver);

    // Get solution
    DefaultSolution solution = clarabel_DefaultSolver_solution(solver);
    print_solution(&solution);

    // Free the matrices and the solver
    clarabel_DefaultSolver_free(solver);
    clarabel_CscMatrix_free(P);
    clarabel_CscMatrix_free(A);

    return 0;
#endif
}
