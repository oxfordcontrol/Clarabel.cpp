#include "clarabel.h"
#include "utils.h"

int main(void)
{
    // SOCP Example

    CscMatrix *P = CscMatrix_from(
        2, 2,
        (double[2][2])
        {
            {0., 0.},
            {0., 2.}
        }
    );

    double q[2] = {0., 0.};

    CscMatrix *A = CscMatrix_from(
        3, 2,
        (double[3][2])
        {
            { 0.,  0.},  
            {-2.,  0.}, 
            { 0., -1.}, 
        }
    );

    double b[3] = {1., -2., -2.};

    SupportedConeT cones[1] =
    {
        SecondOrderConeT(3)
    };

    // Settings
    DefaultSettings settings = DefaultSettingsBuilder_default();

    // Build solver
    DefaultSolver *solver = DefaultSolver_new(
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
    DefaultSolution solution = DefaultSolver_solution(solver);
    print_solution(&solution);

    // Free the matrices and the solver
    DefaultSolver_free(solver);
    CscMatrix_free(P);
    CscMatrix_free(A);

    return 0;
}