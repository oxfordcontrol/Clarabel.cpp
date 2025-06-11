// #define FEATURE_SDP
#include "utils.h"
#include <string.h>
#include <clarabel.h>
#include <math.h>
#include <stdio.h>

#define STRING(string) #string
#define TO_STRING(string) STRING(string)

int main(void)
{
#ifndef FEATURE_SERDE
    printf("This example requires JSON serde support.\n");
    return 1;
#else
    
    char filename[1024];
    strcpy(filename, TO_STRING(EXAMPLES_ROOT_DIR));
    strcat(filename, "/data/hs35.json");

    ClarabelDefaultSolver* solver = clarabel_DefaultSolver_load_from_file(filename);
    clarabel_DefaultSolver_solve(solver);

    // write it back to a file
    // char filename_out[1024];
    // strcpy(filename_out, TO_STRING(EXAMPLES_ROOT_DIR));
    // strcat(filename_out, "/data/output_c.json");
    // clarabel_DefaultSolver_save_to_file(solver, filename_out);

    clarabel_DefaultSolver_free(solver);
    return 0;
#endif
}
