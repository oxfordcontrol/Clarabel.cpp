#ifndef UTILS_H
#define UTILS_H

#include <Clarabel>
#include <cstddef>
#include <cstdio>

namespace utils
{
    template<typename T>
    void print_array(Eigen::Map<Eigen::VectorX<T>> &vec)
    {
        printf("[");
        size_t n = vec.size();
        for (size_t i = 0; i < n; i++)
        {
            printf("%.10f", vec.data()[i]);
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
        print_array(solution.x);
        printf("Multipliers (z)\t = ");
        print_array(solution.z);
        printf("Slacks (s)\t = ");
        print_array(solution.s);
    }

}

#endif /* UTILS_H */