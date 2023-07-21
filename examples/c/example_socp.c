#include "clarabel.h"
#include "utils.h"

int main(void)
{
    // SOCP Example

    /* From dense matrix:
     * [[0., 0.],
     *  [0., 2.]]
     */
    ClarabelCscMatrix_f64 *P = clarabel_CscMatrix_f64_new(
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
    ClarabelCscMatrix_f64 *A = clarabel_CscMatrix_f64_new(
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
    ClarabelDefaultSettings_f64 settings = clarabel_DefaultSettingsBuilder_f64_default();

    // Build solver
    ClarabelDefaultSolver_f64 *solver = clarabel_DefaultSolver_f64_new(
        P, // P
        q, // q
        A, // A
        b, // b
        1, // n_cones
        cones,
        &settings
    );

    // Solve
    clarabel_DefaultSolver_f64_solve(solver);

    // Get solution
    ClarabelDefaultSolution_f64 solution = clarabel_DefaultSolver_f64_solution(solver);
    print_solution_f64(&solution);

    // Free the matrices and the solver
    clarabel_DefaultSolver_f64_free(solver);
    clarabel_CscMatrix_f64_free(P);
    clarabel_CscMatrix_f64_free(A);

    return 0;
}