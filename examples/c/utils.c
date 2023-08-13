#include "utils.h"

#include <stdio.h>

// f64
static void print_array_double(double *array, size_t n)
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

void print_solution_f64(ClarabelDefaultSolution_f64 *solution)
{
    printf("Solution (x)\t = ");
    print_array_double(solution->x, solution->x_length);
    printf("Multipliers (z)\t = ");
    print_array_double(solution->z, solution->z_length);
    printf("Slacks (s)\t = ");
    print_array_double(solution->s, solution->s_length);
}

// f32
static void print_array_float(float *array, size_t n)
{
    printf("[");
    for (size_t i = 0; i < n; i++)
    {
        printf("%.6f", array[i]);
        if (i < n - 1)
        {
            printf(", ");
        }
    }
    printf("]\n");
}

void print_solution_f32(ClarabelDefaultSolution_f32 *solution)
{
    printf("Solution (x)\t = ");
    print_array_float(solution->x, solution->x_length);
    printf("Multipliers (z)\t = ");
    print_array_float(solution->z, solution->z_length);
    printf("Slacks (s)\t = ");
    print_array_float(solution->s, solution->s_length);
}