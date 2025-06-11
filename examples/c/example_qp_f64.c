#include "utils.h"

#include <clarabel.h>

int main(void)
{
    // QP Example

    /* From dense matrix:
     * [[6., 0.],
     *  [0., 4.]]
     */
    ClarabelCscMatrix_f64 P;
    clarabel_CscMatrix_f64_init(
        &P,
        2,                        // row
        2,                        // col
        (uintptr_t[]){ 0, 1, 2 }, // colptr
        (uintptr_t[]){ 0, 1 },    // rowval
        (double[]){ 6., 4. }      // nzval
    );

    double q[2] = { -1., -4. };

    /* From dense matrix:
     * [[ 1., -2.], // <-- LHS of equality constraint (lower bound)
     *  [ 1.,  0.], // <-- LHS of inequality constraint (upper bound)
     *  [ 0.,  1.], // <-- LHS of inequality constraint (upper bound)
     *  [-1.,  0.], // <-- LHS of inequality constraint (lower bound)
     *  [ 0., -1.]] // <-- LHS of inequality constraint (lower bound)
     */
    ClarabelCscMatrix_f64 A;
    clarabel_CscMatrix_f64_init(
        &A,
        5,                                      // row
        2,                                      // col
        (uintptr_t[]){ 0, 3, 6 },               // colptr
        (uintptr_t[]){ 0, 1, 3, 0, 2, 4 },      // rowval
        (double[]){ 1., 1., -1., -2., 1., -1. } // nzval
    );

    double b[5] = { 0., 1., 1., 1., 1. };

    ClarabelSupportedConeT_f64 cones[2] = { ClarabelZeroConeT_f64(1), ClarabelNonnegativeConeT_f64(4) };

    // Settings
    ClarabelDefaultSettings_f64 settings = clarabel_DefaultSettings_f64_default();

    // Build solver
    ClarabelDefaultSolver_f64 *solver = clarabel_DefaultSolver_f64_new(
        &P, // P
        q,  // q
        &A, // A
        b,  // b
        2,  // n_cones
        cones, &settings
    );

    // Solve
    clarabel_DefaultSolver_f64_solve(solver);

    // Get solution
    ClarabelDefaultSolution_f64 solution = clarabel_DefaultSolver_f64_solution(solver);
    print_solution_f64(&solution);

    // Free the solver
    clarabel_DefaultSolver_f64_free(solver);

    return 0;
}
