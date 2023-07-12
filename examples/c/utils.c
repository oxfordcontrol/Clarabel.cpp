#include "utils.h"
#include <stdio.h>

static void print_array(double *array, size_t n)
{
    printf("[");
    for (size_t i = 0; i < n; i++)
    {
        printf("%.10f", array[i]);
        if (i < n - 1)
        {
            printf(", ");
        }
    }
    printf("]\n");
}

void print_solution(DefaultSolution_f64 *solution)
{
    printf("Solution (x)\t = ");
    print_array(solution->x, solution->x_length);
    printf("Multipliers (z)\t = ");
    print_array(solution->z, solution->z_length);
    printf("Slacks (s)\t = ");
    print_array(solution->s, solution->s_length);
}