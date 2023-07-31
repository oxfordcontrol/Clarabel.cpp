#include "utils.h"

#include <Eigen.h>
#include <Eigen/Dense>
#include <Eigen/Sparse>
#include <clarabel.h>
#include <cstdint>
#include <memory>
#include <vector>


using namespace clarabel;
using namespace std;
using namespace Eigen;

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
    MatrixXd P_dense = MatrixXd::Zero(6, 6);
    SparseMatrix<double> P = P_dense.sparseView();
    P.makeCompressed();

    double q[6] = { 0., 0., -1., 0., 0., -1. };

    MatrixXd A_dense(8, 6);
    A_dense <<
        -1.,  0.,  0.,  0.,  0.,  0.,
         0., -1.,  0.,  0.,  0.,  0.,
         0.,  0., -1.,  0.,  0.,  0.,
         0.,  0.,  0., -1.,  0.,  0.,
         0.,  0.,  0.,  0., -1.,  0.,
         0.,  0.,  0.,  0.,  0., -1.,
         1.,  2.,  0.,  3.,  0.,  0.,
         0.,  0.,  0.,  0.,  1.,  0.;
          
    SparseMatrix<double> A = A_dense.sparseView();
    A.makeCompressed();

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
    clarabel::eigen::DefaultSolver<double> solver(P, q, A, b, cones, &settings);

    // Solve
    solver.solve();

    // Get solution
    DefaultSolution<double> solution = solver.solution();
    utils::print_solution(solution);

    return 0;
}