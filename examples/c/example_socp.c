#include "clarabel.h"
#include "utils.h"

int main(void)
{
    // SOCP Example

    /* From dense matrix:
     * [[0., 0.],
     *  [0., 2.]]
     */
    ClarabelCscMatrix_f64 *P = CscMatrix_new(
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
    ClarabelCscMatrix_f64 *A = CscMatrix_new(
        3,
        2,
        (uintptr_t[]){0, 1, 2},
        (uintptr_t[]){1, 2},
        (double[]){-2.0, -1.0}
    );

    double b[3] = {1., -2., -2.};

    ClarabelSupportedConeT_f64 cones[1] =
    {
        ClarabelSecondOrderConeT_f64(3)
    };

    // Settings
    ClarabelDefaultSettings_f64 settings = DefaultSettingsBuilder_default();

    // Build solver
    ClarabelDefaultSolver_f64 *solver = DefaultSolver_new(
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
    ClarabelDefaultSolution_f64 solution = DefaultSolver_solution(solver);
    print_solution_f64(&solution);

    // Free the matrices and the solver
    DefaultSolver_free(solver);
    CscMatrix_free(P);
    CscMatrix_free(A);

    return 0;
}