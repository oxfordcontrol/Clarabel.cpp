#include "clarabel.h"
#include "utils.h"

int main(void)
{
    // QP Example

    /* From dense matrix:
     * [[6., 0.],
     *  [0., 4.]]
     */
    CscMatrix_f32 *P = CscMatrix_f32_new(
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
    CscMatrix_f32 *A = CscMatrix_f32_new(
        5,                                    // m
        2,                                    // n
        (uintptr_t[]){0, 3, 6},               // colptr
        (uintptr_t[]){0, 1, 3, 0, 2, 4},      // rowval
        (float[]){1., 1., -1., -2., 1., -1.} // nzval
    );

    float b[5] = {0., 1., 1., 1., 1.};

    SupportedConeT_f32 cones[2] =
    {
        ZeroConeT_f32(1),
        NonnegativeConeT_f32(4)
    };

    // Settings
    DefaultSettings_f32 settings = DefaultSettingsBuilder_f32_default();

    // Build solver
    DefaultSolver_f32 *solver = DefaultSolver_f32_new(
        P, // P
        q, // q
        A, // A
        b, // b
        2, // n_cones
        cones,
        &settings
    );

    // Solve
    DefaultSolver_f32_solve(solver);

    // Get solution
    DefaultSolution_f32 solution = DefaultSolver_f32_solution(solver);
    print_solution_f32(&solution);

    // Free the matrices and the solver
    DefaultSolver_f32_free(solver);
    CscMatrix_f32_free(P);
    CscMatrix_f32_free(A);

    return 0;
}
