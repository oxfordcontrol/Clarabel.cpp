#include "clarabel.h"
#include "utils.h"

int main(void)
{
    // SOCP Example

    /* From dense matrix:
     * [[0., 0.],
     *  [0., 2.]]
     */
    ClarabelFloat P_nzvalues[] = {2.0};
    ClarabelCscMatrix *P = clarabel_CscMatrix_new(
        2,
        2,
        (uintptr_t[]){0, 0, 1},
        (uintptr_t[]){1},
        P_nzvalues
    );

    ClarabelFloat q[2] = {0., 0.};

    /* From dense matrix:
     * [[ 0.,  0.],
     *  [-2.,  0.],
     *  [ 0., -1.]]
     */
    ClarabelFloat A_nzvalues[] = {-2.0, -1.0};
    ClarabelCscMatrix *A = clarabel_CscMatrix_new(
        3,
        2,
        (uintptr_t[]){0, 1, 2},
        (uintptr_t[]){1, 2},
        A_nzvalues
    );

    ClarabelFloat b[3] = {1., -2., -2.};

    ClarabelSupportedConeT cones[1] =
    {
        ClarabelSecondOrderConeT(3)
    };

    // Settings
    ClarabelDefaultSettings settings = clarabel_DefaultSettingsBuilder_default();

    // Build solver
    ClarabelDefaultSolver *solver = clarabel_DefaultSolver_new(
        P, // P
        q, // q
        A, // A
        b, // b
        1, // n_cones
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
    clarabel_CscMatrix_free(P);
    clarabel_CscMatrix_free(A);

    return 0;
}