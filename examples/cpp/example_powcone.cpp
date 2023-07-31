#include "utils.h"

#include <Clarabel>
#include <cstdint>
#include <memory>
#include <vector>

using namespace clarabel;
using namespace std;

// Power Cone Example
//
//  solve the following power cone problem
//  max  x1^0.6 y^0.4 + x2^0.1
//  s.t. x1, y, x2 >= 0
//       x1 + 2y  + 3x2 == 3
//  which is equivalent to
//  max z1 + z2
//  s.t. (x1, y, z1) in K_pow(0.6)
//       (x2, 1, z2) in K_pow(0.1)
//       x1 + 2y + 3x2 == 3

int main()
{
    // 6 x 6 zero matrix
    uintptr_t P_colptr[] = { 0, 0, 0, 0, 0, 0, 0 };
    CscMatrix<double> P(
        6,        // row
        6,        // col
        P_colptr, // colptr
        nullptr,  // rowval
        nullptr   // nzval
    );

    double q[6] = { 0., 0., -1., 0., 0., -1. };

    /* From dense matrix:
    * [[-1., 0., 0., 0., 0., 0.],
    *  [0., -1., 0., 0., 0., 0.],
    *  [0., 0., -1., 0., 0., 0.],
    *  [0., 0., 0., -1., 0., 0.],
    *  [0., 0., 0., 0., -1., 0.],
    *  [0., 0., 0., 0., 0., -1.],
    *  [1., 2., 0., 3., 0., 0.],
    *  [0., 0., 0., 0., 1., 0.]]
    */

    uintptr_t A_colptr[]{ 0, 2, 4, 5, 7, 9, 10 };
    uintptr_t A_rowptr[]{ 0, 6, 1, 6, 2, 3, 6, 4, 7, 5 };
    double A_nzvals[]{ -1.0, 1.0, -1.0, 2.0, -1.0, -1.0, 3.0, -1.0, 1.0, -1.0 };

    CscMatrix<double> A(
        8,        // row
        6,        // col
        A_colptr, // colptr
        A_rowptr, // rowval
        A_nzvals  // nzval
    );

    double b[8] = { 0., 0., 0., 0., 0., 0., 3., 1. };

    vector<SupportedConeT<double>> cones
    {
        PowerConeT<double>(0.6),
        PowerConeT<double>(0.1),
        ZeroConeT<double>(1),
        ZeroConeT<double>(1),
    };

    // Settings
    DefaultSettings<double> settings = DefaultSettingsBuilder<double>::default_settings()
        .verbose(true)
        .max_iter(100)
        .build();

    // Build solver
    DefaultSolver<double> solver(
        &P, // P
        q,  // q
        &A, // A
        b,  // b
        cones,
        &settings
    );

    // Solve
    solver.solve();

    // Get solution
    DefaultSolution<double> solution = solver.solution();
    utils::print_solution(solution);

    return 0;
}