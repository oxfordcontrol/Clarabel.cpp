// #define FEATURE_SDP
#include "utils.h"

#include <clarabel.hpp>
#include <Eigen/Eigen>
#include <cmath>
#include <vector>

using namespace clarabel;
using namespace std;
using namespace Eigen;

int main(void)
{
#ifndef FEATURE_SDP

    printf("This example requires SDP support.\n");
    return 1;

#else

    // SDP Example
    int n = 3;
    int nvec = (n * (n + 1)) >> 1;

    // 6 x 6 zero matrix
    SparseMatrix<double> P = MatrixXd::Zero(nvec, nvec).sparseView();
    P.makeCompressed();

    Vector<double, 6> c = { 1., 0., 1., 0., 0., 1. };

    double sqrt2 = sqrt(2.0);
    MatrixXd A_dense(7, 6);
    A_dense <<
        -1.,     0.,  0.,     0.,     0.,  0.,
         0., -sqrt2,  0.,     0.,     0.,  0.,
         0.,     0., -1.,     0.,     0.,  0.,
         0.,     0.,  0., -sqrt2,     0.,  0.,
         0.,     0.,  0.,     0., -sqrt2,  0.,
         0.,     0.,  0.,     0.,     0., -1.,
         1.,     4.,  3.,     8.,    10.,  6.;

    SparseMatrix<double> A = A_dense.sparseView();
    A.makeCompressed();

    Vector<double, 7> b = { 0., 0., 0., 0., 0., 0., 1. };

    vector<SupportedConeT<double>> cones
    {
        PSDTriangleConeT<double>(n),
        ZeroConeT<double>(1),
    };

    // Settings
    DefaultSettings<double> settings = DefaultSettings<double>::default_settings();

    // Build solver
    DefaultSolver<double> solver(P, c, A, b, cones, settings);

    // Solve
    solver.solve();

    // Get solution
    DefaultSolution<double> solution = solver.solution();
    utils::print_solution(solution);

    return 0;

#endif // FEATURE_SDP
    
}