// #define FEATURE_SDP
#include "utils.h"

#include <clarabel.h>
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
    ClarabelCscMatrix P;
    clarabel_CscMatrix_init(
        &P,
        nvec,                                 // row
        nvec,                                 // col
        (uintptr_t[]){ 0, 0, 0, 0, 0, 0, 0 }, // colptr
        NULL,                                 // rowval
        NULL                                  // nzval
    );

    ClarabelFloat c[6] = { 1., 0., 1., 0., 0., 1. };

    /* From dense matrix:
     * [[-1., 0., 0., 0., 0., 0.],
     *  [0., -sqrt2., 0., 0., 0., 0.],
     *  [0., 0., -1., 0., 0., 0.],
     *  [0., 0., 0., -sqrt2., 0., 0.],
     *  [0., 0., 0., 0., -sqrt2., 0.],
     *  [0., 0., 0., 0., 0., -1.],
     *  [1., 4., 3., 8., 10., 6.]]
     */
    ClarabelFloat sqrt2 = sqrt(2.0);
    ClarabelCscMatrix A;
    clarabel_CscMatrix_init(
        &A,
        7,                                                                                           // row
        6,                                                                                           // col
        (uintptr_t[]){ 0, 2, 4, 6, 8, 10, 12 },                                                      // colptr
        (uintptr_t[]){ 0, 6, 1, 6, 2, 6, 3, 6, 4, 6, 5, 6 },                                         // rowval
        (ClarabelFloat[]){ -1.0, 1.0, -sqrt2, 4.0, -1.0, 3.0, -sqrt2, 8.0, -sqrt2, 10.0, -1.0, 6.0 } // nzval
    );

    ClarabelFloat b[7] = { 0., 0., 0., 0., 0., 0., 1. };

    ClarabelSupportedConeT cones[2] = {
        ClarabelPSDTriangleConeT(n),
        ClarabelZeroConeT(1),
    };

    // Settings
    ClarabelDefaultSettings settings = clarabel_DefaultSettings_default();

    // Build solver
    ClarabelDefaultSolver *solver = clarabel_DefaultSolver_new(
        &P, // P
        c,  // q
        &A, // A
        b,  // b
        2,  // n_cones
        cones, &settings
    );

    // Solve
    clarabel_DefaultSolver_solve(solver);

    // Get solution
    ClarabelDefaultSolution solution = clarabel_DefaultSolver_solution(solver);
    print_solution(&solution);

    // Free the solver
    clarabel_DefaultSolver_free(solver);

    return 0;
#endif
}
