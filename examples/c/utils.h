#ifndef UTILS_H
#define UTILS_H

#include <Clarabel.h>

void print_solution_f64(ClarabelDefaultSolution_f64 *solution);

void print_solution_f32(ClarabelDefaultSolution_f32 *solution);

#ifdef CLARABEL_USE_FLOAT
#define print_solution(...) print_solution_f32(__VA_ARGS__)
#else
#define print_solution(...) print_solution_f64(__VA_ARGS__)
#endif

#endif /* UTILS_H */