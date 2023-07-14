#include "clarabel.h"
#include "utils.h"

int main(void)
{
    // SOCP Example

    CscMatrix_f64 *P = CscMatrix_f64_from(
        2, 2,
        (double[2][2])
        {
            {0., 0.},
            {0., 2.}
        }
    );

    double q[2] = {0., 0.};

    CscMatrix_f64 *A = CscMatrix_f64_from(
        3, 2,
        (double[3][2])
        {
            { 0.,  0.},  
            {-2.,  0.}, 
            { 0., -1.}, 
        }
    );

    double b[3] = {1., -2., -2.};

    SupportedConeT_f64 cones[1] =
    {
        SecondOrderConeT_f64(3)
    };

    // Settings
    DefaultSettings_f64 settings = DefaultSettingsBuilder_f64_default();

    // Build solver
    DefaultSolver_f64 *solver = DefaultSolver_f64_new(
        P, // P
        q, // q
        A, // A
        b, // b
        1, // n_cones
        cones,
        &settings
    );

    // Solve
    DefaultSolver_f64_solve(solver);

    // Get solution
    DefaultSolution_f64 solution = DefaultSolver_f64_solution(solver);
    print_solution(&solution);

    // Free the matrices and the solver
    free_DefaultSolver_f64(solver);
    free_CscMatrix_f64(P);
    free_CscMatrix_f64(A);

    return 0;
}