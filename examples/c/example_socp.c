#include "utils.h"

#include <clarabel.h>

int main(void)
{
    // SOCP Example

    /* From dense matrix:
     * [[0., 0.],
     *  [0., 2.]]
     */
    ClarabelCscMatrix P;
    clarabel_CscMatrix_init(
        &P,
        2,                        // row
        2,                        // col
        (uintptr_t[]){ 0, 0, 1 }, // colptr
        (uintptr_t[]){ 1 },       // rowval
        (ClarabelFloat[]){ 2.0 }  // nzval
    );

    ClarabelFloat q[2] = { 0., 0. };

    /* From dense matrix:
     * [[ 0.,  0.],
     *  [-2.,  0.],
     *  [ 0., -1.]]
     */
    ClarabelCscMatrix A;
    clarabel_CscMatrix_init(
        &A,
        3,                              // row
        2,                              // col
        (uintptr_t[]){ 0, 1, 2 },       // colptr
        (uintptr_t[]){ 1, 2 },          // rowval
        (ClarabelFloat[]){ -2.0, -1.0 } // nzval
    );

    ClarabelFloat b[3] = { 1., -2., -2. };

    ClarabelSupportedConeT cones[1] = { ClarabelSecondOrderConeT(3) };

    // Settings
    ClarabelDefaultSettings settings = clarabel_DefaultSettings_default();

    // Build solver
    ClarabelDefaultSolver *solver = clarabel_DefaultSolver_new(
        &P, // P
        q,  // q
        &A, // A
        b,  // b
        1,  // n_cones
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