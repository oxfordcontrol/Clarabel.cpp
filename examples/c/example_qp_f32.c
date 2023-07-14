#include "clarabel.h"
#include "utils.h"

int main(void)
{
    // QP Example

    // let P = CscMatrix::identity(2);    // For P = I
    // let P = CscMatrix::zeros((2,2));   // For P = 0

    // direct from sparse data
    CscMatrix_f32 *_P = CscMatrix_f32_new(
        2,                      // m
        2,                      // n
        (uintptr_t[]){0, 1, 2}, // colptr
        (uintptr_t[]){0, 1},    // rowval
        (float[]){6., 4.}      // nzval
    );

    // or an easier way for small problems...
    CscMatrix_f32 *P = CscMatrix_f32_from(
        2, 2,
        (float[2][2])
        {
            {6., 0.},
            {0., 4.}
        }
    );

    float q[2] = {-1., -4.};

    // direct from sparse data
    CscMatrix_f32 *_A = CscMatrix_f32_new(
        5,                                    // m
        2,                                    // n
        (uintptr_t[]){0, 3, 6},               // colptr
        (uintptr_t[]){0, 1, 3, 0, 2, 4},      // rowval
        (float[]){1., 1., -1., -2., 1., -1.} // nzval
    );

    // or an easier way for small problems...
    CscMatrix_f32 *A = CscMatrix_f32_from(
        5, 2,
        (float[5][2])
        {
            { 1., -2.}, // <-- LHS of equality constraint (lower bound)
            { 1.,  0.}, // <-- LHS of inequality constraint (upper bound)
            { 0.,  1.}, // <-- LHS of inequality constraint (upper bound)
            {-1.,  0.}, // <-- LHS of inequality constraint (lower bound)
            { 0., -1.}  // <-- LHS of inequality constraint (lower bound)
        }
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
    free_DefaultSolver_f32(solver);
    free_CscMatrix_f32(_P);
    free_CscMatrix_f32(P);
    free_CscMatrix_f32(_A);
    free_CscMatrix_f32(A);

    return 0;
}
