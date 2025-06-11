// To use float instead of double:
// #define CLARABEL_USE_FLOAT

#include "utils.h"
#include <stdio.h>
#include <clarabel.h>

int main(void)
{
    // QP Example

    /* From dense matrix:
     * [[6., 0.],
     *  [0., 4.]]
     */
    ClarabelCscMatrix P;
    clarabel_CscMatrix_init(
        &P,
        2,                          // row
        2,                          // col
        (uintptr_t[]){ 0, 1, 2 },   // colptr
        (uintptr_t[]){ 0, 1 },      // rowval
        (ClarabelFloat[]){ 6., 4. } // nzval
    );

    ClarabelFloat q[2] = { -1., -4. };

    /* From dense matrix:
     * [[ 1., -2.], // <-- LHS of equality constraint (lower bound)
     *  [ 1.,  0.], // <-- LHS of inequality constraint (upper bound)
     *  [ 0.,  1.], // <-- LHS of inequality constraint (upper bound)
     *  [-1.,  0.], // <-- LHS of inequality constraint (lower bound)
     *  [ 0., -1.]] // <-- LHS of inequality constraint (lower bound)
     */
    ClarabelCscMatrix A;
    clarabel_CscMatrix_init(
        &A,
        5,                                             // row
        2,                                             // col
        (uintptr_t[]){ 0, 3, 6 },                      // colptr
        (uintptr_t[]){ 0, 1, 3, 0, 2, 4 },             // rowval
        (ClarabelFloat[]){ 1., 1., -1., -2., 1., -1. } // nzval
    );

    ClarabelFloat b[5] = { 0., 1., 1., 1., 1. };

    ClarabelSupportedConeT cones[2] = { ClarabelZeroConeT(1), ClarabelNonnegativeConeT(4) };

    // Settings
    ClarabelDefaultSettings settings = clarabel_DefaultSettings_default();

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

    // Get some detailed solve information
    ClarabelDefaultInfo_f64 info = clarabel_DefaultSolver_info(solver);
    printf("primal residual = %e\n", info.res_primal);
    printf("dual residual   = %e\n", info.res_dual);
    printf("# of threads    = %d\n", info.linsolver.threads);
    printf("KKT nonzeros    = %d\n", info.linsolver.nnzA);
    printf("factor nonzeros = %d\n", info.linsolver.nnzL);

    // Free the matrices and the solver
    clarabel_DefaultSolver_free(solver);

    return 0;
}
