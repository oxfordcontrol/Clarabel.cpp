#include <stdio.h>
#include "clarabel.h"

int main(void)
{
    printf("Hello, world!\n");

    CscMatrix_f64 *P = CscMatrix_f64_zeros(2, 2);

    double q[2] = {1.0, -1.0};

    // a 2-d box constraint, separated into 4 inequalities.
    // A = [I; -I]
    CscMatrix_f64 *_A = CscMatrix_f64_new(
        4, 2,                            // row, col
        (uintptr_t[]){0, 2, 4},          // colptr
        (uintptr_t[]){0, 2, 1, 3},       // rowval
        (double[]){1.0, -1.0, 1.0, -1.0} // nzval
    );

    // easier way - use the From trait to construct A:
    CscMatrix_f64 *A = CscMatrix_f64_from(
        4, 2,
        (double[4][2]){
            {1.0, 0.0},
            {0.0, 1.0},
            {-1.0, 0.0},
            {0.0, -1.0}});

    double b[4] = {1.0, 1.0, 1.0, 1.0};

    SupportedConeT_f64 cones[1] =
        {
            NonnegativeConeT_f64(4)};

    // Settings
    DefaultSettings_f64 settings = DefaultSettingsBuilder_f64_default();
    settings.equilibrate_enable = true;
    settings.equilibrate_max_iter = 50;

    // Build solver
    DefaultSolver *solver = DefaultSolver_f64_new(
        P, // P
        q, // q
        A, // A
        b, // b
        1, // n_cones
        cones,
        &settings);

    // Solve
    DefaultSolver_solve(solver);

    // Free the matrices and the solver
    free_DefaultSolver(solver);
    free_CscMatrix_f64(P);
    free_CscMatrix_f64(A);
    free_CscMatrix_f64(_A);

    return 0;
}