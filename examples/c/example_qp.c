// To use float instead of double:
// #define CLARABEL_USE_FLOAT

#include "clarabel.h"
#include "utils.h"

int main(void)
{
    // QP Example

    /* From dense matrix:
     * [[6., 0.],
     *  [0., 4.]]
     */
    ClarabelFloat A_nzvalues[] = {6., 4.};
    ClarabelCscMatrix P;
    clarabel_CscMatrix_init(
        &P,
        2,                      // m
        2,                      // n
        (uintptr_t[]){0, 1, 2}, // colptr
        (uintptr_t[]){0, 1},    // rowval
        A_nzvalues              // nzval
    );

    ClarabelFloat q[2] = {-1., -4.};

    /* From dense matrix:
     * [[ 1., -2.], // <-- LHS of equality constraint (lower bound)
     *  [ 1.,  0.], // <-- LHS of inequality constraint (upper bound)
     *  [ 0.,  1.], // <-- LHS of inequality constraint (upper bound)
     *  [-1.,  0.], // <-- LHS of inequality constraint (lower bound)
     *  [ 0., -1.]] // <-- LHS of inequality constraint (lower bound)
     */
    ClarabelFloat B_nzvalues[] = {1., 1., -1., -2., 1., -1.};
    ClarabelCscMatrix A;
    clarabel_CscMatrix_init(
        &A,
        5,                               // m
        2,                               // n
        (uintptr_t[]){0, 3, 6},          // colptr
        (uintptr_t[]){0, 1, 3, 0, 2, 4}, // rowval
        B_nzvalues                       // nzval
    );

    ClarabelFloat b[5] = {0., 1., 1., 1., 1.};

    ClarabelSupportedConeT cones[2] =
    {
        ClarabelZeroConeT(1),
        ClarabelNonnegativeConeT(4)
    };

    // Settings
    ClarabelDefaultSettings settings = clarabel_DefaultSettingsBuilder_default();

    // Build solver
    ClarabelDefaultSolver *solver = clarabel_DefaultSolver_new(
        &P, // P
        q, // q
        &A, // A
        b, // b
        2, // n_cones
        cones,
        &settings
    );

    // Solve
    clarabel_DefaultSolver_solve(solver);

    // Get solution
    ClarabelDefaultSolution solution = clarabel_DefaultSolver_solution(solver);
    print_solution(&solution);

    // Free the matrices and the solver
    clarabel_DefaultSolver_free(solver);

    return 0;
}
