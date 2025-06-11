#include "utils.h"

#include <clarabel.h>
#include <math.h>

int main(void)
{
    // 3 x 3 zero matrix
    ClarabelCscMatrix P;
    clarabel_CscMatrix_init(
        &P,
        3,                           // row
        3,                           // col
        (uintptr_t[]){ 0, 0, 0, 0 }, // colptr
        NULL,                        // rowval
        NULL                         // nzval
    );

    ClarabelFloat q[3] = { -1.0, 0.0, 0.0 };

    /* From dense matrix:
     * [-1.0, 0., 0.],
     * [0., -1., 0.],
     * [0., 0., -1.],
     * [0., 1., 0.],
     * [0., 0., 1.],
     */
    ClarabelCscMatrix A;
    clarabel_CscMatrix_init(
        &A,
        5,                                              // row
        3,                                              // col
        (uintptr_t[]){ 0, 1, 3, 5 },                    // colptr
        (uintptr_t[]){ 0, 1, 3, 2, 4 },                 // rowval
        (ClarabelFloat[]){ -1.0, -1.0, 1.0, -1.0, 1.0 } // nzval
    );

    ClarabelFloat b[5] = { 0., 0., 0., 1., exp(5.0) };

    ClarabelSupportedConeT cones[2] = { ClarabelExponentialConeT(), ClarabelZeroConeT(2) };

    // Settings
    ClarabelDefaultSettings settings = clarabel_DefaultSettings_default();
    settings.verbose = true;

    // Build solver
    ClarabelDefaultSolver *solver = clarabel_DefaultSolver_new(
        &P, // P
        q,  // q
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
}