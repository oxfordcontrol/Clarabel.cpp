#include <stdio.h>
#include "clarabel.h"

int main(void)
{
    printf("Hello, world!\n");

    // Allocate a new CscMatrixF64
    CscMatrixF64 *_A = CscMatrix_new(
        4, 2,                            // row, col
        (uintptr_t[]){0, 2, 4},          // colptr
        (uintptr_t[]){0, 2, 1, 3},       // rowval
        (double[]){1.0, -1.0, 1.0, -1.0} // nzval
    );

    // Delete the CscMatrixF64 object
    CscMatrix_delete(_A);

    return 0;
}