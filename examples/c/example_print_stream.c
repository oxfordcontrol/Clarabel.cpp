// #define FEATURE_SDP
#include "utils.h"
#include <string.h>
#include <clarabel.h>
#include <math.h>
#include <stdio.h>


int main(void)
{

    ClarabelCscMatrix P;
    clarabel_CscMatrix_init(
        &P,
        2,                          // row
        2,                          // col
        (uintptr_t[]){ 0, 1, 2 },   // colptr
        (uintptr_t[]){ 0, 1 },      // rowval
        (ClarabelFloat[]){ 6., 4. } // nzval
    );

    ClarabelFloat q[2] = { -1., -4. };
    ClarabelCscMatrix A;
    clarabel_CscMatrix_init(
        &A,
        5,                                             // row
        2,                                             // col
        (uintptr_t[]){ 0, 3, 6 },                      // colptr
        (uintptr_t[]){ 0, 1, 3, 0, 2, 4 },             // rowval
        (ClarabelFloat[]){ 1., 1., -1., -2., 1., -1. } // nzval
    );

    ClarabelFloat b[5] = { 0., 1., 1., 1., 1. };

    ClarabelSupportedConeT cones[2] = { ClarabelZeroConeT(1), ClarabelNonnegativeConeT(4) };

    // Settings
    ClarabelDefaultSettings settings = clarabel_DefaultSettings_default();

    // Build solver
    ClarabelDefaultSolver *solver = clarabel_DefaultSolver_new(
        &P, // P
        q,  // q
        &A, // A
        b,  // b
        2,  // n_cones
        cones, &settings
    );

    // redirect progress output to a file 
    printf("Printing to file ....\n");
    clarabel_DefaultSolver_print_to_file(solver,"clarabel_example_print_stream.txt");

    // solve
    clarabel_DefaultSolver_solve(solver);

    // redirect progress output to a buffer
    printf("Printing to buffer ....\n");
    clarabel_DefaultSolver_print_to_buffer(solver);

    // solve again 
    clarabel_DefaultSolver_solve(solver);

    // fetch and print the buffer 
    printf("Recovering from buffer ....\n");
    const char* buffer = clarabel_DefaultSolver_get_print_buffer(solver);
    printf("%s\n", buffer);
    clarabel_free_print_buffer(buffer);

    // restore the buffer to stdout
    printf("Printing to stdout ....\n");
    clarabel_DefaultSolver_print_to_stdout(solver);
    clarabel_DefaultSolver_solve(solver);

}
