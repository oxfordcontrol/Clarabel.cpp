#include "clarabel.h"
#include "utils.h"

int main(void)
{
    // QP Example

    /* From dense matrix:
     * [[6., 0.],
     *  [0., 4.]]
     */
    ClarabelCscMatrix *P = CscMatrix_new(
        2,                      // m
        2,                      // n
        (uintptr_t[]){0, 1, 2}, // colptr
        (uintptr_t[]){0, 1},    // rowval
        (double[]){6., 4.}      // nzval
    );

    double q[2] = {-1., -4.};

    /* From dense matrix:
     * [[ 1., -2.], // <-- LHS of equality constraint (lower bound)
     *  [ 1.,  0.], // <-- LHS of inequality constraint (upper bound)
     *  [ 0.,  1.], // <-- LHS of inequality constraint (upper bound)
     *  [-1.,  0.], // <-- LHS of inequality constraint (lower bound)
     *  [ 0., -1.]] // <-- LHS of inequality constraint (lower bound)
     */
    ClarabelCscMatrix *A = CscMatrix_new(
        5,                                    // m
        2,                                    // n
        (uintptr_t[]){0, 3, 6},               // colptr
        (uintptr_t[]){0, 1, 3, 0, 2, 4},      // rowval
        (double[]){1., 1., -1., -2., 1., -1.} // nzval
    );

    double b[5] = {0., 1., 1., 1., 1.};

    ClarabelSupportedConeT cones[2] =
    {
        ClarabelZeroConeT(1),
        ClarabelNonnegativeConeT(4)
    };

    // Settings
    ClarabelDefaultSettings settings = DefaultSettingsBuilder_default();

    // Build solver
    ClarabelDefaultSolver *solver = DefaultSolver_new(
        P, // P
        q, // q
        A, // A
        b, // b
        2, // n_cones
        cones,
        &settings
    );

    // Solve
    DefaultSolver_solve(solver);

    // Get solution
    ClarabelDefaultSolution solution = DefaultSolver_solution(solver);
    print_solution(&solution);

    // Free the matrices and the solver
    DefaultSolver_free(solver);
    CscMatrix_free(P);
    CscMatrix_free(A);

    return 0;
}
