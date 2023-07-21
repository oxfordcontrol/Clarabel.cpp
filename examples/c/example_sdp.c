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
    CscMatrix_f64 *P = clarabel_CscMatrix_f64_new(
        6,
        6,
        (uintptr_t[]){0, 0, 0, 0, 0, 0, 0},
        NULL,
        NULL
    );

    double c[6] = {0., 0., -1., 0., 0., -1.};

    double sqrt2 = sqrt(2.0);
    /* From dense matrix:
     * [[-1., 0., 0., 0., 0., 0.],
     *  [0., -sqrt2., 0., 0., 0., 0.],
     *  [0., 0., -1., 0., 0., 0.],
     *  [0., 0., 0., -sqrt2., 0., 0.],
     *  [0., 0., 0., 0., -sqrt2., 0.],
     *  [0., 0., 0., 0., 0., -1.],
     *  [1., 4., 3., 8., 10., 6.]]
     */
    CscMatrix_f64 *A = clarabel_CscMatrix_f64_new(
        7,
        6,
        (uintptr_t[]){0, 2, 4, 6, 8, 10, 12},
        (uintptr_t[]){0, 6, 1, 6, 2, 6, 3, 6, 4, 6, 5, 6},
        (double[]){-1.0, 1.0, -sqrt2, 4.0, -1.0, 3.0, -sqrt2, 8.0, -sqrt2, 10.0, -1.0, 6.0}
    );

    double b[7] = {0., 0., 0., 0., 0., 0., 1.};

    SupportedConeT_f64 cones[2] =
    {
        PSDTriangleConeT_f64(n),
        ZeroConeT_f64(1),
    };

    // Settings
    DefaultSettings_f64 settings = clarabel_DefaultSettingsBuilder_f64_default();

    // Build solver
    DefaultSolver_f64 *solver = clarabel_DefaultSolver_f64_new(
        P, // P
        c, // q
        A, // A
        b, // b
        2, // n_cones
        cones,
        &settings
    );

    // Solve
    clarabel_DefaultSolver_f64_solve(solver);

    // Get solution
    DefaultSolution_f64 solution = clarabel_DefaultSolver_f64_solution(solver);
    print_solution_f64(&solution);

    // Free the matrices and the solver
    clarabel_DefaultSolver_f64_free(solver);
    clarabel_CscMatrix_f64_free(P);
    clarabel_CscMatrix_f64_free(A);

    return 0;
#endif
}
