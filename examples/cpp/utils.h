#ifndef UTILS_H
#define UTILS_H

#include "clarabel.h"
#include <cstddef>
#include <cstdio>

namespace utils
{
    template<typename T>
    void print_array(T *array, size_t n)
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

    template<typename T>
    void print_solution(clarabel::DefaultSolution<T> &solution)
    {
        printf("Solution (x)\t = ");
        print_array(solution.x, solution.x_length);
        printf("Multipliers (z)\t = ");
        print_array(solution.z, solution.z_length);
        printf("Slacks (s)\t = ");
        print_array(solution.s, solution.s_length);
    }
}

#endif /* UTILS_H */