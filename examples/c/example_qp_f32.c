#include "clarabel.h"
#include "utils.h"

int main(void)
{
    // QP Example

    /* From dense matrix:
     * [[6., 0.],
     *  [0., 4.]]
     */
    ClarabelCscMatrix_f32 *P = clarabel_CscMatrix_f32_new(
        2,                      // m
        2,                      // n
        (uintptr_t[]){0, 1, 2}, // colptr
        (uintptr_t[]){0, 1},    // rowval
        (float[]){6., 4.}       // nzval
    );

    float q[2] = {-1., -4.};

    /* From dense matrix:
     * [[ 1., -2.], // <-- LHS of equality constraint (lower bound)
     *  [ 1.,  0.], // <-- LHS of inequality constraint (upper bound)
     *  [ 0.,  1.], // <-- LHS of inequality constraint (upper bound)
     *  [-1.,  0.], // <-- LHS of inequality constraint (lower bound)
     *  [ 0., -1.]] // <-- LHS of inequality constraint (lower bound)
     */
    ClarabelCscMatrix_f32 *A = clarabel_CscMatrix_f32_new(
        5,                                    // m
        2,                                    // n
        (uintptr_t[]){0, 3, 6},               // colptr
        (uintptr_t[]){0, 1, 3, 0, 2, 4},      // rowval
        (float[]){1., 1., -1., -2., 1., -1.} // nzval
    );

    float b[5] = {0., 1., 1., 1., 1.};

    ClarabelSupportedConeT_f32 cones[2] =
    {
        ClarabelZeroConeT_f32(1),
        ClarabelNonnegativeConeT_f32(4)
    };

    // Settings
    ClarabelDefaultSettings_f32 settings = clarabel_DefaultSettingsBuilder_f32_default();

    // Build solver
    ClarabelDefaultSolver_f32 *solver = clarabel_DefaultSolver_f32_new(
        P, // P
        q, // q
        A, // A
        b, // b
        2, // n_cones
        cones,
        &settings
    );

    // Solve
    clarabel_DefaultSolver_f32_solve(solver);

    // Get solution
    ClarabelDefaultSolution_f32 solution = clarabel_DefaultSolver_f32_solution(solver);
    print_solution_f32(&solution);

    // Free the matrices and the solver
    clarabel_DefaultSolver_f32_free(solver);
    clarabel_CscMatrix_f32_free(P);
    clarabel_CscMatrix_f32_free(A);

    return 0;
}
