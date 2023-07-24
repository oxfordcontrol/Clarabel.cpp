#include "clarabel.h"
#include "utils.h"ClarabelCscMatrix 

int main(void)
{
    // 2 x 2 zero matrix
    ClarabelCscMatrix P;
    clarabel_CscMatrix_init(
        &P,
        2,
        2,
        (uintptr_t[]){0, 0, 0},
        NULL,
        NULL
    );

    ClarabelFloat q[2] = {1.0, -1.0};

    // a 2-d box constraint, separated into 4 inequalities.
    // A = [I; -I]
    ClarabelFloat A_nzvalues[] = {1.0, -1.0, 1.0, -1.0};
    ClarabelCscMatrix A;
    clarabel_CscMatrix_init(
        &A,
        4,
        2,                         // row, col
        (uintptr_t[]){0, 2, 4},    // colptr
        (uintptr_t[]){0, 2, 1, 3}, // rowval
        A_nzvalues                 // nzval
    );

    ClarabelFloat b[4] = {1.0, 1.0, 1.0, 1.0};

    ClarabelSupportedConeT cones[1] =
    {
        ClarabelNonnegativeConeT(4)
    };

    // Settings
    ClarabelDefaultSettings settings = clarabel_DefaultSettingsBuilder_default();
    settings.equilibrate_enable = true;
    settings.equilibrate_max_iter = 50;

    // Build solver
    ClarabelDefaultSolver *solver = clarabel_DefaultSolver_new(
        &P, // P
        q,  // q
        &A, // A
        b,  // b
        1,  // n_cones
        cones,
        &settings
    );

    // Solve
    clarabel_DefaultSolver_solve(solver);

    // Get solution
    ClarabelDefaultSolution solution = clarabel_DefaultSolver_solution(solver);
    print_solution(&solution);

    // Free the solver
    clarabel_DefaultSolver_free(solver);

    return 0;
}