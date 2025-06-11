#include "utils.h"
#include <stdio.h>
#include <clarabel.h>


int custom_callback(ClarabelDefaultInfo *info, void* _userdata)
{
    // This function is called at each iteration of the solver.
    // You can use it to monitor the progress of the solver or
    // to implement custom stopping criteria.

    // For example, we can print the current iteration number:
    printf("Custom callback at iteration %d: ", info->iterations);

    // Return 0 to continue.   Anything else to stop.
    if (info->iterations < 3) {
        printf("tick\n");
        return 0; //continue
    }
    else {
        printf("BOOM!\n");
        return 1; // stop
    }

}

int main(void)
{
    // 2 x 2 zero matrix
    ClarabelCscMatrix P;
    clarabel_CscMatrix_init(
        &P,
        2,                        // row
        2,                        // col
        (uintptr_t[]){ 0, 0, 0 }, // colptr
        NULL,                     // rowval
        NULL                      // nzval
    );

    ClarabelFloat q[2] = { 1.0, -1.0 };

    // a 2-d box constraint, separated into 4 inequalities.
    // A = [I; -I]
    ClarabelCscMatrix A;
    clarabel_CscMatrix_init(
        &A,
        4,                                        // row
        2,                                        // col
        (uintptr_t[]){ 0, 2, 4 },                 // colptr
        (uintptr_t[]){ 0, 2, 1, 3 },              // rowval
        (ClarabelFloat[]){ 1.0, -1.0, 1.0, -1.0 } // nzval
    );

    ClarabelFloat b[4] = { 1.0, 1.0, 1.0, 1.0 };

    ClarabelSupportedConeT cones[1] = { ClarabelNonnegativeConeT(4) };

    // Settings
    ClarabelDefaultSettings settings = clarabel_DefaultSettings_default();
    settings.equilibrate_enable = true;
    settings.equilibrate_max_iter = 50;

    // Build solver
    ClarabelDefaultSolver *solver = clarabel_DefaultSolver_new(
        &P, // P
        q,  // q
        &A, // A
        b,  // b
        1,  // n_cones
        cones, &settings
    );

    // configure a custom callback function 
    clarabel_DefaultSolver_set_termination_callback(solver,custom_callback, NULL);

    // Solve
    clarabel_DefaultSolver_solve(solver);

    // turn off the callback
    clarabel_DefaultSolver_unset_termination_callback(solver);

    // Solve again
    clarabel_DefaultSolver_solve(solver);

    // Get solution
    ClarabelDefaultSolution solution = clarabel_DefaultSolver_solution(solver);
    print_solution(&solution);

    // Free the solver
    clarabel_DefaultSolver_free(solver);

    return 0;
}

