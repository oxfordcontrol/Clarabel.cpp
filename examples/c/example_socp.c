#include "clarabel.h"
#include "utils.h"

int main(void)
{
    // SOCP Example

    /* From dense matrix:
     * [[0., 0.],
     *  [0., 2.]]
     */
    ClarabelCscMatrix *P = CscMatrix_new(
        2,
        2,
        (uintptr_t[]){0, 0, 1},
        (uintptr_t[]){1},
        (double[]){2.0}
    );

    double q[2] = {0., 0.};

    /* From dense matrix:
     * [[ 0.,  0.],
     *  [-2.,  0.],
     *  [ 0., -1.]]
     */
    ClarabelCscMatrix *A = CscMatrix_new(
        3,
        2,
        (uintptr_t[]){0, 1, 2},
        (uintptr_t[]){1, 2},
        (double[]){-2.0, -1.0}
    );

    double b[3] = {1., -2., -2.};

    ClarabelSupportedConeT cones[1] =
    {
        ClarabelSecondOrderConeT(3)
    };

    // Settings
    ClarabelDefaultSettings settings = DefaultSettingsBuilder_default();

    // Build solver
    ClarabelDefaultSolver *solver = DefaultSolver_new(
        P, // P
        q, // q
        A, // A
        b, // b
        1, // n_cones
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